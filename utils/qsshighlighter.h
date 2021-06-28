#ifndef QSSHIGHLIGHTER_H
#define QSSHIGHLIGHTER_H

#include "utils_global.h"

#include <QSyntaxHighlighter>

#include <QRegularExpression>


class UTILS_EXPORT QssHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    QssHighlighter(QTextDocument *parent = 0);

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
