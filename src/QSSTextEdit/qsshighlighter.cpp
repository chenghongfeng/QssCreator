#include "qsshighlighter.h"

#include <QRegularExpressionMatchIterator>
#include <QRegularExpressionMatch>

#include <QDebug>
#include "qsstexteditmanager.h"


QssHighlighter::QssHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}

void QssHighlighter::appendKeyword(const QString &keyword, const QTextCharFormat &format)
{
    QStringList keywords;
    keywords.append(keyword);
    appendKeywords(keywords,format);

}

void QssHighlighter::appendKeywords(const QStringList &keywords, const QTextCharFormat &format)
{
    QStringList keywordPatterns;
    for (auto key : keywords) {
        keywordPatterns.append(QStringLiteral("\\b%1\\b").arg(key));
    }
    appendHighLightRule(keywordPatterns,format);
}

void QssHighlighter::appendHighLightRule(const QStringList &rulePatterns, const QTextCharFormat &format)
{

    for (auto pattern : rulePatterns) {
        QssHighlightingRule rule;
        rule.pattern = QRegularExpression(pattern);
        rule.format = format;
        m_qtKeywordHighlightingRules.append(rule);
    }
//    qDebug()<<"__________start print rules______________";
//    for (const QssHighlightingRule &rule : qAsConst(m_qtKeywordHighlightingRules)) {

//        qDebug()<<rule.pattern;
//        qDebug()<<rule.format.foreground().color();

//    }
//    qDebug()<<"____________end print rules______________";
}

void QssHighlighter::updateColorDefKeywords(const QStringList &keywords, const QTextCharFormat &format)
{
    m_colorDefHighlightRules.clear();
    QStringList keywordPatterns;
    for (auto key : keywords) {
        QString keyFixed = key;
        keyFixed.remove(0,1);
        keywordPatterns.append(QStringLiteral("\\$\\b%1\\b").arg(keyFixed));
    }
    for (auto pattern : keywordPatterns) {
        QssHighlightingRule rule;
        rule.pattern = QRegularExpression(pattern);
        rule.format = format;
        m_colorDefHighlightRules.append(rule);
    }
}

void QssHighlighter::setCommentFormat(const QTextCharFormat &format)
{
    qDebug()<<"setCommentFormat...";
    printFormatInfo(format);
    QssHighlightingRule rule;
    singleLineCommentFormat = format;
    printFormatInfo(singleLineCommentFormat);
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    m_qtKeywordHighlightingRules.append(rule);

    multiLineCommentFormat = format;

}

void QssHighlighter::printFormatInfo(const QTextCharFormat &format)
{
    qDebug()<<format.foreground();
}

void QssHighlighter::clearRules()
{
    m_qtKeywordHighlightingRules.clear();
    m_colorDefHighlightRules.clear();
}


void QssHighlighter::highlightBlock(const QString &text)
{
    for (const QssHighlightingRule &rule : qAsConst(m_qtKeywordHighlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
//            qDebug()<<text;
//            qDebug()<<rand();
//            qDebug()<< rule.format;
//            qDebug()<< rule.format.foreground();
//            qDebug()<<rule.pattern;
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    for (const QssHighlightingRule &rule : qAsConst(m_colorDefHighlightRules)) {
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
