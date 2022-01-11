#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QFontComboBox>

#include "QssTextEdit/qsshighlighter.h"
#include "textsettingswidget.h"

QT_BEGIN_NAMESPACE
class QFile;
class QFileDialog;
class QStandardItemModel;
class QStringListModel;
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class ColorDefTableModel;
class TabWidget;
class QssTextEdit;
class TextSettingsWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    void initUi();
    void initSignalSlots();
    void initSettings();
    void saveSettings();
    QFont getFontFromConfig();


protected:
    void keyPressEvent(QKeyEvent *e) override;

private slots:
    void on_replaceBtn_clicked();

    void on_saveTextBtn_clicked();

    void on_actionOpenQssFile_triggered();

    void on_actionOpenDefineFile_triggered();

    //void on_actionset_triggered();

    void slot_fontSettingsChanged(const FontSettings &font);

    void on_actionSetQss_triggered();

    void on_actionSaveQssFile_triggered();

    void on_actionSaveColorDefineFile_triggered();

    void on_actionAddNewDef_triggered();

    void on_actionshowSourceText_triggered();

    void on_actionshowColorDefines_triggered();

    void on_actionSaveQssToFile_triggered();

    void on_actionFind_triggered();

    void on_actionFindAndReplace_triggered();

private:
    Ui::MainWindow *ui;
    QString m_strColorDefFile;
    QString m_strQssFile;
    QssTextEdit *m_textEdit { nullptr };
    TabWidget *m_tabWidget { nullptr };
    TextSettingsWidget *m_textSettingsWidget { nullptr };
};
#endif // MAINWINDOW_H
