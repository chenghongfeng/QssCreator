#include "qsstextedit.h"

#include <QCompleter>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QDebug>

#include "qsshighlighter.h"
#include "fileHelper.h"
#include "path.h"
QssTextEdit::QssTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    m_highlighter = new QssHighlighter(this->document());
    m_completer = new QCompleter(this);
    initQssKeywordModel();
    //setCompleter(m_completer);
}

void QssTextEdit::initCompleter()
{
    if(!m_completer)
        return;
    m_completer->setWidget(this);
    m_completer->setModel(m_qssKeywordModel);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(m_completer, QOverload<const QString &>::of(&QCompleter::activated),
                     this, &QssTextEdit::insertCompletion);
}

void QssTextEdit::setDefKeyword(const QStringList &defKeywords)
{
    if (!m_qssKeywordModel) return;
    QStringList qssKeywords = Utils::FileHelper::readLinesFromFile(Path::getInstance()->qssKeywordFilePath());
    qssKeywords += defKeywords;
    m_qssKeywordModel->setStringList(qssKeywords);
}

void QssTextEdit::setFile(const QString &fileName)
{
    if (!fileName.isEmpty()) {
        QFile file(fileName);

        if (file.open(QFile::ReadOnly)) {
            QString str = file.readAll();
            this->setPlainText(str);
        }
    }
}

void QssTextEdit::keyPressEvent(QKeyEvent *e)
{
    qDebug()<<e->text();
    if(e->key() == Qt::Key_Dollar)
    {
        qDebug()<<"Qt::Key_Exclam";
    }
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

void QssTextEdit::initQssKeywordModel()
{
    QStringList qssKeywords = Utils::FileHelper::readLinesFromFile(Path::getInstance()->qssKeywordFilePath());
    m_qssKeywordModel = new QStringListModel(qssKeywords, m_completer);
}


void QssTextEdit::insertCompletion(const QString &completion)
{
    if (m_completer->widget() != this)
        return;
    QTextCursor tc = textCursor();
    //tc.removeSelectedText();
    int extra = completion.length() - m_completer->completionPrefix().length();
    while (extra > 1) {
        tc.deletePreviousChar();
        extra--;
    }
    //tc.movePosition(QTextCursor::Left);
    //tc.movePosition(QTextCursor::StartOfWord);
    tc.insertText(completion/*.right(extra)*/);
    setTextCursor(tc);
}

QString QssTextEdit::textUnderCursor()
{
    //

    QTextCursor tc = textCursor();
    tc.select(QTextCursor::BlockUnderCursor);
#ifdef INTERNAL_TEXT
    QString txt = tc.selectedText();
    emit completionPrefixChanged(txt);
#endif
    if(tc.selectedText() == '$')
    {
        return tc.selectedText();
    }
    tc.select(QTextCursor::WordUnderCursor);
    QString text = tc.selectedText();
    tc.movePosition(QTextCursor::StartOfWord);
    tc.select(QTextCursor::BlockUnderCursor);
    if(tc.selectedText() == '$')
    {
        tc.movePosition(QTextCursor::EndOfWord);
        emit completionPrefixChanged('$' + text);
        return '$' + text;
    }
    else{
        tc.movePosition(QTextCursor::EndOfWord);
        emit completionPrefixChanged(text);
        return text;
    }

    //int start = tc.selectionStart();


    return tc.selectedText();
}
