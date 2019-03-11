#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define DONT_SCALE_IMAGE

#include <map>
#include <QKeyEvent>
#include <QSettings>
#include <QFileInfo>
#include <QMainWindow>
#include <QColorDialog>
#include <QTime>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QStack>
#include <list>
#include <fstream>
#include <iostream>
#include <mutex>

#include "proto2/data.pb.h"
#include "netforward.h"
#include "graphicscene.h"
#include "functor.h"
#include "convertimage.h"
#include "opctionswindow.h"
#include <google/protobuf/text_format.h>

namespace Ui {
class MainWindow;
}

template<bool I>
class predictBoxes{};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *event);
    ~MainWindow();

public slots:
    void openFile();
    void openFloder();
    void modifyData();

    void setCreateMode();
    void setSelectMode();
    void setResizeMode();

    void videoPrvFrame();
    void prvImage();
    void videoNexFrame();
    void nextImage();

    void saveFile();
    void addItem();
    void deleteItem();
    void comboBoxChanged(QString name);
    void loadSetting();
    void saveSetting();
    void setOpctions();


    void updateUI();
    void updateShortcut();

    void videoSliderChanged(int index);

    void videoStepChange(int video_Step);

    void widghtItemNumberChange(int num);

private:

    void predictionBoxAndDraw(const cv::Mat &img);

    void setImage(cv::Mat &img);
    void setImage(cv::Mat &img, QString txtPath);

    void updateComboBoxColor();

    cv::Mat getVideoImage(int index);
    cv::Mat openImage(QString path);
    cv::Mat openVideo(QString path);

    Ui::MainWindow *ui;


    //every time you open new file you should call the release function
    cv::VideoCapture videoCapture;
    bool isVideoOpen = false;

    QString fileName;
    QString filePath;
    int total_Image;
    int now_Index;
    int video_Step;
    QColorDialog itemColorDialog;
    std::map<QString, QColor> classMap;
    DearMode<DrawRect, GraphicScene>    *CreateMode;
    DearMode<SelectRect, GraphicScene>  *SelectMode;
    DearMode<RectResize, GraphicScene>  *ResizeMode;

    static const predictBoxes<true>  PREDICTBOXES;
    static const predictBoxes<false> DONOTPRDICT;

    OpctionsWindow opctionsWindow;

    QList<QFileInfo> filePathList;
    QString txtDir;

//    std::mutex threadLocker;

    const QString SettingFilePath = "./setting.ini";
    const QString MultiClassName = "MultiClass";

    const int DELAY_TIME = 20;
    const int MAX_IMAGE_SIZE   = 800;
};

#endif // MAINWINDOW_H
