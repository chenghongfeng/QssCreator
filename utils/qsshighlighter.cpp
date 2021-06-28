#include "qsshighlighter.h"

#include <QRegularExpressionMatchIterator>
#include <QRegularExpressionMatch>


QssHighlighter::QssHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    QssHighlightingRule rule;
    QBrush keywordBrush(QColor("#b58909"));
    m_keywordFormat.setForeground(keywordBrush);
    const QString keywordPatterns[] = {
        QStringLiteral("\\bimage\\b"), QStringLiteral("\\bwidth\\b"), QStringLiteral("\\bheight\\b"),
        QStringLiteral("\\bfont-family\\b"), QStringLiteral("\\bcolor\\b"), QStringLiteral("\\bbackground-color\\b"),
        QStringLiteral("\\bborder\\b"), QStringLiteral("\\bborder-radius\\b"), QStringLiteral("\\bpadding-top\\b"),
        QStringLiteral("\\bpadding-left\\b"), QStringLiteral("\\bpadding-right\\b"), QStringLiteral("\\bleft\\b"),
        QStringLiteral("\\bsubcontrol-origin\\b"), QStringLiteral("\\bsubcontrol-position\\b"), QStringLiteral("\\bmargin-top\\b"),
        QStringLiteral("\\bmargin-bottom\\b"), QStringLiteral("\\bsignals\\b"), QStringLiteral("\\bsigned\\b"),
        QStringLiteral("\\bslots\\b"), QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bstruct\\b"),
        QStringLiteral("\\btemplate\\b"), QStringLiteral("\\btypedef\\b"), QStringLiteral("\\btypename\\b"),
        QStringLiteral("\\bunion\\b"), QStringLiteral("\\bunsigned\\b"), QStringLiteral("\\bvirtual\\b"),
        QStringLiteral("\\bvoid\\b"), QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bbool\\b")
    };
    for (const QString &patten : keywordPatterns) {
        rule.pattern = QRegularExpression(patten);
        rule.format = m_keywordFormat;
        m_qssHighlightingRules.append(rule);
    }

    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    m_qssHighlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}

void QssHighlighter::highlightBlock(const QString &text)
{
    for (const QssHighlightingRule &rule : qAsConst(m_qssHighlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    //设置代码块的state
    // 0表示没有注释或者注释起始和结束都在text中
    // 1表示只有注释起始符或者该块之前有起始符但还没有匹配结束符 下一个Block仍要要寻找结束符
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1) //如果上一块没有注释起始符
    {
        startIndex = text.indexOf(commentStartExpression);
    }
    while (startIndex >= 0)
    {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1)//没有找到注释结束符,本段全部注释
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
