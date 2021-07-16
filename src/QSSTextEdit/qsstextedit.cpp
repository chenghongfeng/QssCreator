#include "qsstextedit.h"

#include <QCompleter>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QScrollBar>

#include "qsshighlighter.h"
#include "fileHelper.h"
#include "path.h"
QssTextEdit::QssTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
    m_highlighter = new QssHighlighter(this->document());
    m_completer = new QCompleter(this);
    initQssKeywordModel();
    setCompleter(m_completer);
}

void QssTextEdit::setCompleter(QCompleter *completer)
{
    if(m_completer)
        m_completer->disconnect(this);
    m_completer = completer;

    if(!m_completer)
        return;
    m_completer->setWidget(this);
    m_completer->setModel(m_qssKeywordModel);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    //指定参数为const QString &的QCompleter::activated的信号
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
            this->setText(str);
        }
    }
}

void QssTextEdit::keyPressEvent(QKeyEvent *e)
{
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
        QTextEdit::keyPressEvent(e);

    const bool ctrlOrShift = e->modifiers().testFlag(Qt::ControlModifier) ||
                             e->modifiers().testFlag(Qt::ShiftModifier);
    if (!m_completer || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    const bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
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
    QTextEdit::focusInEvent(e);
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
    int extra = completion.length() - m_completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

QString QssTextEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}
