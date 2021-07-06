#include "qsstextedit.h"

#include <QCompleter>
#include <QStringListModel>

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
