#ifndef QSSHIGHLIGHTER_H
#define QSSHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>


class QssHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    QssHighlighter(QTextDocument *parent = 0);
    void appendKeyWords(const QStringList &keywords, const QTextCharFormat &format);
    void appendHighLightRule(const QStringList &rulePattens, const QTextCharFormat &format);
    void setCommentFormat(const QTextCharFormat &format);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct QssHighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<QssHighlightingRule> m_qssHighlightingRules;

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
