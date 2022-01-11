#ifndef QSSHIGHLIGHTER_H
#define QSSHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>


class QssHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    QssHighlighter(QTextDocument *parent = 0);
    void appendKeyword(const QString &keyword,  const QTextCharFormat &format);
    void appendKeywords(const QStringList &keywords, const QTextCharFormat &format);
    void appendHighLightRule(const QStringList &rulePattens, const QTextCharFormat &format);
    void updateColorDefKeywords(const QStringList &keywords, const QTextCharFormat &format);
    void setCommentFormat(const QTextCharFormat &format);
    void printFormatInfo(const QTextCharFormat &format);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct QssHighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    //将关键词和colorDef分开,这样方便更新,因为关键词在实际使用中是固定的
    QVector<QssHighlightingRule> m_qtKeywordHighlightingRules;
    QVector<QssHighlightingRule> m_colorDefHighlightRules;

    QRegularExpression m_colorExp;
    QRegularExpression m_defExp;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat m_colorFormat;
    QTextCharFormat m_defFormat;
    QTextCharFormat m_keywordFormat;


    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

};

#endif // QSSHIGHLIGHTER_H
