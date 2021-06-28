#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

class QFontDialog;
namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = nullptr);
    ~ConfigDialog();

private:
    Ui::ConfigDialog *ui;
    QFontDialog *m_fontDialog{};

};

#endif // CONFIGDIALOG_H
