#ifndef QSSTEXTEDIT_H
#define QSSTEXTEDIT_H
#include <QTextEdit>
#include <QPlainTextEdit>
QT_BEGIN_NAMESPACE
class QCompleter;
class QssHighlighter;
class QStringListModel;
QT_END_NAMESPACE

class QssTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    QssTextEdit(QWidget *parent = nullptr);
    void initCompleter();
    void setDefKeyword(const QStringList &defKeywords);
    void setFile(const QString &fileName);

signals:
    void completionPrefixChanged(QString text);

protected:
    //https://bugreports.qt.io/browse/QTBUG-95361
    //There is a bug about keyPressEvent
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;
private:
    void initQssKeywordModel();

private slots:
    void insertCompletion(const QString &completion);
    QString textUnderCursor() ;

private:
    QCompleter *m_completer{nullptr};
    QssHighlighter *m_highlighter{nullptr};
    QStringListModel *m_qssKeywordModel{nullptr};
};

#endif // QSSTEXTEDIT_H
