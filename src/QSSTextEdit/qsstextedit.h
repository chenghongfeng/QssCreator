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
    void setFile(const QString &fileName);

protected:
    //待优化 现在只替换后面一部分,导致键入大写部分然后出现小写单词,补全后前面是用户键入的,后面是自动不全的的
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;
private:
    void initQssKeywordModel();

private slots:
    void insertCompletion(const QString &completion);
    QString textUnderCursor() const;

private:
    QCompleter *m_completer{nullptr};
    QssHighlighter *m_highlighter{nullptr};
    QStringListModel *m_qssKeywordModel{nullptr};
};

#endif // QSSTEXTEDIT_H
