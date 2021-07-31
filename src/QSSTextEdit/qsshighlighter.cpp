#include "qsshighlighter.h"

#include <QRegularExpressionMatchIterator>
#include <QRegularExpressionMatch>


QssHighlighter::QssHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{


}

void QssHighlighter::appendKeyWords(const QStringList &keywords, const QTextCharFormat &format)
{
    QStringList keywordPatterns;
    for (auto key : keywords) {
        keywordPatterns.append(QStringLiteral("\\%1\\b").arg(key));
    }
    appendHighLightRule(keywords,format);
}

void QssHighlighter::appendHighLightRule(const QStringList &rulePatterns, const QTextCharFormat &format)
{
    QssHighlightingRule rule;
    for (auto pattern : rulePatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = format;
        m_qssHighlightingRules.append(rule);
    }
}

void QssHighlighter::setCommentFormat(const QTextCharFormat &format)
{
    QssHighlightingRule rule;
    singleLineCommentFormat = format;
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    m_qssHighlightingRules.append(rule);

    multiLineCommentFormat = format;
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
