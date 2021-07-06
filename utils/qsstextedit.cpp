#include "qsstextedit.h"

#include <QCompleter>
#include <QStringListModel>

#include "qsshighlighter.h"
#include "fileHelper.h"
#include "path.h"
QssTextEdit::QssTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
    m_pHighlighter = new QssHighlighter(this->document());
    m_pCompleter = new QCompleter(this);
    setCompleter(m_pCompleter);
}

void QssTextEdit::setCompleter(QCompleter *completer)
{
    if(m_pCompleter)
        m_pCompleter->disconnect(this);
    m_pCompleter = completer;

    if(!m_pCompleter)
        return;
    m_pCompleter->setWidget(this);
    QStringList qssKeywords = Utils::FileHelper::readLinesFromFile(Path::getInstance()->qssKeywordFilePath());
    m_pCompleter->setModel(new QStringListModel(qssKeywords, m_pCompleter));
    m_pCompleter->setCompletionMode(QCompleter::PopupCompletion);
    m_pCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    //指定参数为const QString &的QCompleter::activated的信号
    QObject::connect(m_pCompleter, QOverload<const QString &>::of(&QCompleter::activated),
                     this, &QssTextEdit::insertCompletion);
}

void QssTextEdit::insertCompletion(const QString &completion)
{
    if (m_pCompleter->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - m_pCompleter->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}
