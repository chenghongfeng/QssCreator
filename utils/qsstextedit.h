#ifndef QSSTEXTEDIT_H
#define QSSTEXTEDIT_H

#include <QTextEdit>
QT_BEGIN_NAMESPACE
class QCompleter;
class QssHighlighter;
class QStringListModel;
QT_END_NAMESPACE

class QssTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    QssTextEdit(QWidget *parent = nullptr);
    void setCompleter(QCompleter *completer);
    void setDefKeyword(const QStringList &defKeywords);
private:
    void initQssKeywordModel();

private slots:
    void insertCompletion(const QString &completion);

private:
    QCompleter *m_completer{nullptr};
    QssHighlighter *m_highlighter{nullptr};
    QStringListModel *m_qssKeywordModel{nullptr};
};

#endif // QSSTEXTEDIT_H
