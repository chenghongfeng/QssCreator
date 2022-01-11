#ifndef QSSTEXTEDIT_H
#define QSSTEXTEDIT_H
#include <QTextEdit>
#include <QPlainTextEdit>
QT_BEGIN_NAMESPACE
class QCompleter;
class QssHighlighter;
class QStringListModel;
QT_END_NAMESPACE
class LineNumberArea;

class QssTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    QssTextEdit(QWidget *parent = nullptr);
    void initCompleter();
    void setTextFromFile(const QString &fileName);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();



private:
    enum class ReturnOperation{None=0, AddTab, FixBrace};
protected:
    //https://bugreports.qt.io/browse/QTBUG-95361
    //There is a bug about keyPressEvent
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    void initQssKeywordModel();
    ReturnOperation nextLineOperation();
    void doReturnOpreation(ReturnOperation op);

private slots:
    void insertCompletion(const QString &completion);
    QString textUnderCursor() ;
    void slot_updateColorDef();
    //line count
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &rect, int dy);
    //TODO
    void highlightCurrentLine();

private:
    //when press return,program

    QCompleter *m_completer{nullptr};
    QssHighlighter *m_highlighter{nullptr};
    QStringListModel *m_completerWordModel{nullptr};
    LineNumberArea *m_lineNumberArea{nullptr};
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(QssTextEdit *editor) : QWidget(editor), codeEditor(editor)
    {}

    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    QssTextEdit *codeEditor;
};

#endif // QSSTEXTEDIT_H
