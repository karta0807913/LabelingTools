#ifndef OPCTIONSWINDOW_H
#define OPCTIONSWINDOW_H

#include <QMainWindow>

namespace Ui {
class OpctionsWindow;
}

class MainWindow;

class OpctionsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OpctionsWindow(QWidget *parent = 0);
    QString getOutPutPath();

    QString frameShortcut;
    QString arrorShortcut;
    QString scalingShortcut;
    QString upShortcut;
    QString downShortcut;
    QString exportShortcut;

    void loadSetting();
    void saveSetting();
    void setRecallFunction(MainWindow *x);

    ~OpctionsWindow();

public slots:
    void browseOutputPath();
    bool close();

signals:
    void closeWindow();

private:
    Ui::OpctionsWindow *ui;
    void updateShortcut();

    MainWindow *window = nullptr;

    const QString SettingFilePath = "./setting.ini";
};

#endif // OPCTIONSWINDOW_H
