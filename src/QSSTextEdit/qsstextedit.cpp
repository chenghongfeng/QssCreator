#include "qsstextedit.h"

#include <QCompleter>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QDebug>
#include <QPainter>
#include "config.h"

#include "qsshighlighter.h"
#include "fileHelper.h"
#include "path.h"
#include "qsstexteditmanager.h"
QssTextEdit::QssTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    m_lineNumberArea = new LineNumberArea(this);
    connect(this, &QssTextEdit::blockCountChanged, this, &QssTextEdit::updateLineNumberAreaWidth);
    connect(this, &QssTextEdit::updateRequest, this, &QssTextEdit::updateLineNumberArea);
    connect(this, &QssTextEdit::cursorPositionChanged, this, &QssTextEdit::highlightCurrentLine);

    connect(QssTextEditManager::getInstance(), &QssTextEditManager::defsUpdated, this, &QssTextEdit::slot_updateColorDef);
    m_highlighter = new QssHighlighter(this->document());
    m_completer = new QCompleter(this);
    initQssKeywordModel();
}

void QssTextEdit::initCompleter()
{
    if(!m_completer)
        return;
    m_completer->setWidget(this);
    m_completer->setModel(m_completerWordModel);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(m_completer, QOverload<const QString &>::of(&QCompleter::activated),
                     this, &QssTextEdit::insertCompletion);
}

void QssTextEdit::setTextFromFile(const QString &fileName)
{
    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (file.open(QFile::ReadOnly)) {
            QString str = file.readAll();
            this->setPlainText(str);
        }
    }
}

void QssTextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), QColor("#073642"));

//![extraAreaPaintEvent_0]

//![extraAreaPaintEvent_1]
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
//![extraAreaPaintEvent_1]

//![extraAreaPaintEvent_2]
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor("#586e75"));
            painter.drawText(0, top, m_lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

int QssTextEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void QssTextEdit::keyPressEvent(QKeyEvent *e)
{
#ifdef NO_AUTO_COMPLETER
    return QPlainTextEdit::keyPressEvent(e);
#endif
    if (m_completer && m_completer->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }
    else if(e->key() == Qt::Key_Return)
    {
        //https://www.cnblogs.com/cute/p/14875836.html
        ReturnOperation op = nextLineOperation();
        QPlainTextEdit::keyPressEvent(e);
        doReturnOpreation(op);
        return;
    }

    if(e->text() == "(")
    {
        QPlainTextEdit::keyPressEvent(e);
        insertPlainText(")");
        QTextCursor tc = textCursor();
        tc.movePosition(QTextCursor::Left);
        setTextCursor(tc);
        return;
    }

    const bool isShortcut = (e->modifiers().testFlag(Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!m_completer || !isShortcut) // do not process the shortcut when we have a completer
        QPlainTextEdit::keyPressEvent(e);

    const bool ctrlOrShift = e->modifiers().testFlag(Qt::ControlModifier) ||
                             e->modifiers().testFlag(Qt::ShiftModifier);
    if (!m_completer || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    const bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 1
                      || eow.contains(e->text().right(1)))) {
        m_completer->popup()->hide();
        return;
    }

    if (completionPrefix != m_completer->completionPrefix()) {
        m_completer->setCompletionPrefix(completionPrefix);
        m_completer->popup()->setCurrentIndex(m_completer->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(m_completer->popup()->sizeHintForColumn(0)
                + m_completer->popup()->verticalScrollBar()->sizeHint().width());
    m_completer->complete(cr); // popup it up!
}

void QssTextEdit::focusInEvent(QFocusEvent *e)
{
    if (m_completer)
        m_completer->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}

void QssTextEdit::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void QssTextEdit::initQssKeywordModel()
{
    const QStringList qclassKeywords = QssTextEditManager::getInstance()->qtClassKeywords();
    QTextCharFormat format;
    QColor qtClassTextColor(Config::getInstance()->value("Text/QtClassTextColor","#b58900").toString());
    format.setForeground(qtClassTextColor);
    m_highlighter->appendKeywords(qclassKeywords, format);

    const QStringList qssKeywords = QssTextEditManager::getInstance()->qssKeywords();
    QColor qssKeywordTextColor(Config::getInstance()->value("Text/QssKeywordTextColor","#709d06").toString());
    format.setForeground(qssKeywordTextColor);
    m_highlighter->appendKeywords(qssKeywords,format);

    QColor defKeywordTextColor(Config::getInstance()->value("Text/UserDefineWordTextColor","#cb4b16").toString());
    format.setForeground(defKeywordTextColor);
    const QStringList defKeywords = QssTextEditManager::getInstance()->getCurDefs().keys();
    m_highlighter->updateColorDefKeywords(defKeywords, format);

    m_completerWordModel = new QStringListModel(qssKeywords + qclassKeywords + defKeywords, m_completer);
}


QssTextEdit::ReturnOperation QssTextEdit::nextLineOperation()
{
    QTextCursor tc = textCursor();
    tc.movePosition(QTextCursor::Left,QTextCursor::KeepAnchor);
    QString endChar = tc.selectedText();
    if(endChar == "{")
    {
        int pos = tc.position();
        QString text = this->toPlainText();
        int leftBraceIndex = text.indexOf('{',pos + 1);
        int rightBraceIndex = text.indexOf('}',pos + 1);
        //don't fix brace When we can't find leftBrace but rightBrace is existed or leftBrace is in the front of rightBrace
        if((leftBraceIndex == -1 || leftBraceIndex > rightBraceIndex) && rightBraceIndex > 0)
        {
            return ReturnOperation::AddTab;
        }
        return ReturnOperation::FixBrace;
    }
    tc.movePosition(QTextCursor::StartOfLine,QTextCursor::KeepAnchor);
    QString line = tc.selectedText();
    if(line.startsWith("\t"))
    {
        return ReturnOperation::AddTab;
    }
    return ReturnOperation::None;
}

void QssTextEdit::doReturnOpreation(ReturnOperation op)
{
    QTextCursor tempTc;
    switch (op) {
    case ReturnOperation::None:
        break;
    case ReturnOperation::AddTab:
        insertPlainText("\t");
        break;
    case ReturnOperation::FixBrace:
        insertPlainText("\t\n}");
        tempTc = textCursor();
        tempTc.movePosition(QTextCursor::Up);
        tempTc.movePosition(QTextCursor::Right);
        setTextCursor(tempTc);
        break;
    default:
        break;
    }
}


void QssTextEdit::insertCompletion(const QString &completion)
{
    if (m_completer->widget() != this)
        return;
    QTextCursor tc = textCursor();
    tc.beginEditBlock();
    int completionPrefixLength = m_completer->completionPrefix().length();
    //delete completionPrefix
    while (completionPrefixLength > 0) {
        tc.deletePreviousChar();
        completionPrefixLength--;
    }
    //insert completion
    tc.insertText(completion);
    tc.endEditBlock();
    setTextCursor(tc);
}

QString QssTextEdit::textUnderCursor()
{
    //获取当前word
    QTextCursor tc = textCursor();
    tc.movePosition(QTextCursor::MoveOperation::StartOfWord, QTextCursor::MoveMode::KeepAnchor);
    QString selectWord = tc.selectedText();
    //前移一位看看能不能找到$
    tc.movePosition(QTextCursor::MoveOperation::Left, QTextCursor::MoveMode::KeepAnchor);
    if(tc.selectedText() != QString() && tc.selectedText().at(0) == '$')
    {
        selectWord = tc.selectedText();
    }
    return selectWord;
}

void QssTextEdit::slot_updateColorDef()
{
    QTextCharFormat format;
    QColor defKeywordTextColor(Config::getInstance()->value("Text/UserDefineWordTextColor","#cb4b16").toString());
    format.setForeground(defKeywordTextColor);
    QStringList defKeywords = QssTextEditManager::getInstance()->getCurDefs().keys();
    m_highlighter->updateColorDefKeywords(defKeywords, format);

    if (!m_completerWordModel) return;
    QStringList keywords = QssTextEditManager::getInstance()->qssKeywords() + QssTextEditManager::getInstance()->qtClassKeywords();
    keywords += defKeywords;
    m_completerWordModel->setStringList(keywords);
}

void QssTextEdit::updateLineNumberAreaWidth(int /*newBlockCount*/)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void QssTextEdit::highlightCurrentLine()
{

}

void QssTextEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        m_lineNumberArea->scroll(0, dy);
    else
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}
