#ifndef QSSTEXTEDIT_H
#define QSSTEXTEDIT_H

#include <QTextEdit>
QT_BEGIN_NAMESPACE
class QCompleter;
class QssHighlighter;
QT_END_NAMESPACE

class QssTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    QssTextEdit(QWidget *parent = nullptr);
    void setCompleter(QCompleter *completer);

private slots:
    void insertCompletion(const QString &completion);

private:
    QCompleter *m_pCompleter{nullptr};
    QssHighlighter *m_pHighlighter{nullptr};
};

#endif // QSSTEXTEDIT_H
