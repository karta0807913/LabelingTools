#include <QTcpSocket>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#define YOLO_V2
#ifndef YOLO_V2
#include "yolo_output.h"
#else
#include "yolo_v2_output.h"
#endif

//typedef yolo_v2::BASE_YOLO_2_FUNCTOR YOLO_BOX_FUNCTION;
// typedef yolo_v2::SOFT_NMS YOLO_FUNCTOR;

#include <QRect>
#include <QTime>
#include "socketgraphicsitem.h"
#include <QDomDocument>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadSetting();

    ui->widget->setGeometry(QRect(10,10,MAX_IMAGE_SIZE,MAX_IMAGE_SIZE));

    ui->widget->setDeleteHotKey(Qt::Key_Space);

    opctionsWindow.setRecallFunction(this);

    CreateMode = new DearMode<DrawRect, GraphicScene>   (ui->widget->getMainScene());
    SelectMode = new DearMode<SelectRect, GraphicScene> (ui->widget->getMainScene());
    ResizeMode = new DearMode<RectResize, GraphicScene> (ui->widget->getMainScene());

    connect(ui->Export      , SIGNAL(clicked()), this, SLOT(saveFile()));
    connect(ui->addItem     , SIGNAL(clicked()), this, SLOT(addItem()));
    connect(ui->deleteItem  , SIGNAL(clicked()), this, SLOT(deleteItem()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboBoxChanged(QString)));

    connect(ui->actionOpenFile  , SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionOpenFloder, SIGNAL(triggered()), this, SLOT(openFloder()));
    connect(ui->actionModify    , SIGNAL(triggered()), this, SLOT(modifyData()));
    connect(ui->actionFrame     , SIGNAL(triggered()), this, SLOT(setCreateMode()));
    connect(ui->actionArror     , SIGNAL(triggered()), this, SLOT(setSelectMode()));
    connect(ui->actionScaling   , SIGNAL(triggered()), this, SLOT(setResizeMode()));
    connect(ui->actionUp        , SIGNAL(triggered()), this, SLOT(videoPrvFrame()));
    connect(ui->actionUp        , SIGNAL(triggered()), this, SLOT(prvImage()));
    connect(ui->actionDown      , SIGNAL(triggered()), this, SLOT(videoNexFrame()));
    connect(ui->actionDown      , SIGNAL(triggered()), this, SLOT(nextImage()));
    connect(ui->actionOpctions  , SIGNAL(triggered()), this, SLOT(setOpctions()));

    connect(ui->VideoSlider     , SIGNAL(valueChanged(int)), this, SLOT(videoSliderChanged(int)));
    connect(ui->FrameStep       , SIGNAL(valueChanged(int)), this, SLOT(videoStepChange(int)));

    connect(ui->widget          , SIGNAL(itemNumberChanged(int)), this, SLOT(widghtItemNumberChange(int)));

    ui->ImageNum->setVisible(false);

    updateShortcut();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_1: ui->comboBox->setCurrentIndex(0); comboBoxChanged(ui->comboBox->currentText()); break;
    case Qt::Key_2: ui->comboBox->setCurrentIndex(1); comboBoxChanged(ui->comboBox->currentText()); break;
    case Qt::Key_3: ui->comboBox->setCurrentIndex(2); comboBoxChanged(ui->comboBox->currentText()); break;
    case Qt::Key_4: ui->comboBox->setCurrentIndex(3); comboBoxChanged(ui->comboBox->currentText()); break;
    case Qt::Key_5: ui->comboBox->setCurrentIndex(4); comboBoxChanged(ui->comboBox->currentText()); break;
    case Qt::Key_6: ui->comboBox->setCurrentIndex(5); comboBoxChanged(ui->comboBox->currentText()); break;
    case Qt::Key_7: ui->comboBox->setCurrentIndex(6); comboBoxChanged(ui->comboBox->currentText()); break;
    case Qt::Key_8: ui->comboBox->setCurrentIndex(7); comboBoxChanged(ui->comboBox->currentText()); break;
    case Qt::Key_9: ui->comboBox->setCurrentIndex(8); comboBoxChanged(ui->comboBox->currentText()); break;

    case Qt::Key_Enter: // input frame(image) number change
        int numIndex = ui->ImageNum->text().split('/')[0].toInt();
        if(numIndex == now_Index)
            break;
        if(numIndex > total_Image){
            updateUI();
            break;
        }

        if(!videoCapture.isOpened() && filePathList.empty())
            return;

        if(videoCapture.isOpened() && numIndex == 0)
            numIndex += 1;

        now_Index = numIndex;

        cv::Mat img;
        if(videoCapture.isOpened()){
            img = getVideoImage(now_Index);
            setImage(img);
        }
        else if(!filePathList.empty())
        {
            img = cv::imread(filePathList[now_Index].absoluteFilePath().toLocal8Bit().data());
            setImage(img, txtDir + filePathList[now_Index].baseName() + ".prototxt");
            fileName = filePathList[now_Index].baseName();
        }
        else
        {
            ui->VideoSlider->setValue(0);
            return;
        }

        updateUI();

        break;
    }
}

void MainWindow::updateUI()
{
    ui->ImageNum->setVisible(videoCapture.isOpened() || !filePathList.empty());
    if(ui->ImageNum->isVisible())
    {
        ui->ImageNum->setText(QString::number(now_Index));

        QString total(QString::number(total_Image));
        QString inputMask;
        for(int index = 0; index < total.length(); ++index)
            inputMask += "9";
        inputMask += "/";
        foreach (const QChar c, total) {
            if(c == '9' || c == '0')
                inputMask += '\\';
            inputMask += c;
        }
        ui->ImageNum->setInputMask(inputMask);
    }
    ui->fileNameLabel->setText(fileName);
    ui->VideoSlider->setValue(now_Index);
}

void MainWindow::updateShortcut()
{
    ui->actionFrame     ->setShortcut(opctionsWindow.frameShortcut);
    ui->actionArror     ->setShortcut(opctionsWindow.arrorShortcut);
    ui->actionScaling   ->setShortcut(opctionsWindow.scalingShortcut);
    ui->actionUp        ->setShortcut(opctionsWindow.upShortcut);
    ui->actionDown      ->setShortcut(opctionsWindow.downShortcut);
    ui->Export          ->setShortcut(opctionsWindow.exportShortcut);
}


void MainWindow::setCreateMode()
{
    ui->actionArror->setChecked(false);
    ui->actionScaling->setChecked(false);
    ui->widget->setSelectAble(false);
    ui->widget->setSelectMode(CreateMode);
}

void MainWindow::setSelectMode()
{
    ui->actionFrame->setChecked(false);
    ui->actionScaling->setChecked(false);
    ui->widget->setSelectAble(true);
    ui->widget->setSelectMode(SelectMode);
}

void MainWindow::setResizeMode()
{
    ui->actionFrame->setChecked(false);
    ui->actionArror->setChecked(false);
    ui->widget->setSelectAble(true);
    ui->widget->setSelectMode(ResizeMode);
}

void MainWindow::openFile()
{
    //QString selfilter = tr("IMG (*.jpg *.jpeg *.png)");
    QString imagePath = QFileDialog::getOpenFileName(this, "SelectImage", filePath,
                                                     "All files (*.jpg *.jpeg *.png *.bmp *.mp4 *.avi *.wmv *.mov);;\
                                                      JPEG (*.jpg *.jpeg);;PNG (*.png);;\
                                                      BMP (*.bmp);;\
                                                      Movie Files(*.mp4 *.avi *.wmv *.mov)");

    if(imagePath == "")
        return;

    QFileInfo info(imagePath);
    fileName = info.baseName();
    filePath = info.absolutePath();
    QString bundleName = imagePath.split(fileName).back();
    bundleName = bundleName.toLower();

    cv::Mat img;

    if(bundleName == ".mp4" ||
       bundleName == ".avi" ||
       bundleName == ".wmv" ||
       bundleName == ".mov")
    {
        img = openVideo(imagePath);
    }
    else
    {
        img = openImage(imagePath);
    }

    if(img.empty())
        return;

    setImage(img);
    updateShortcut();
}

void MainWindow::openFloder()
{
    QString path = QFileDialog::getExistingDirectory(
                NULL,
                "Open Floder",
                filePath,
                QFileDialog::ShowDirsOnly);

    if(path == "")
        return;
    filePath = path;

    QStringList nameFilters;
    nameFilters << QString("*.jpeg") << QString("*.jpg")
                << QString("*.png") << QString("*.bmp");
    QDir dir(path + "/");

    if(dir.exists()){
        dir.setNameFilters(nameFilters);

        /**/
        filePathList.clear();
        txtDir = "";

        foreach(QFileInfo fileInfo , dir.entryInfoList(nameFilters))
            filePathList.append(fileInfo);

        if(filePathList.empty())
            return;

        now_Index = 0;
        total_Image = filePathList.length() - 1;

        cv::Mat img = cv::imread(filePathList[now_Index].absoluteFilePath().toLocal8Bit().data());
        fileName = filePathList[now_Index].baseName();

        setImage(img);

		ui->VideoSlider->setMaximum(filePathList.size() - 1);

        updateUI();
        /**/

        videoCapture.release();
        updateShortcut();
    }
}

cv::Mat MainWindow::openImage(QString path)
{
	now_Index = 0;
	ui->VideoSlider->setMaximum(0);
	ui->VideoSlider->setValue(0);
    videoCapture.release();
    filePathList.clear();
    updateUI();
	return cv::imread(path.toLocal8Bit().constData(), cv::IMREAD_COLOR);
}

cv::Mat MainWindow::openVideo(QString path)
{
    if(videoCapture.open(path.toLocal8Bit().data()))
    {
        now_Index = 1;
        total_Image = videoCapture.get(cv::CAP_PROP_FRAME_COUNT) - 1;
		ui->VideoSlider->setMaximum(total_Image);
		ui->VideoSlider->setValue(1);
        ui->fileNameLabel->setText(fileName);
        updateUI();

        filePathList.clear();
        return getVideoImage(now_Index);
    }
    return cv::Mat();
}

void MainWindow::modifyData()
{
    QString path = QFileDialog::getExistingDirectory(
                NULL,
                "Select Image Floder",
                filePath,
                QFileDialog::ShowDirsOnly);

    if(path == "")
        return;

    txtDir = QFileDialog::getExistingDirectory(
                    NULL,
                    "Select Text Floder",
                    filePath,
                    QFileDialog::ShowDirsOnly);

    if(txtDir == "")
        return;

    filePath = path;
    txtDir += "/";

    QStringList nameFilters;
    nameFilters << QString("*.jpeg") << QString("*.jpg")
                << QString("*.png") << QString("*.bmp");
    QDir dir(path + "/");

    if(dir.exists()){
        dir.setNameFilters(nameFilters);

        /**/
        filePathList.clear();

        foreach(QFileInfo fileInfo , dir.entryInfoList(nameFilters))
            filePathList.append(fileInfo);

        if(filePathList.empty())
            return;

		ui->VideoSlider->setMaximum(filePathList.size() - 1);
        now_Index = 0;
        total_Image = filePathList.length() - 1;

        cv::Mat img = cv::imread(filePathList[now_Index].absoluteFilePath().toLocal8Bit().data());
        fileName = filePathList[now_Index].baseName();

        setImage(img, txtDir + filePathList[now_Index].baseName() + ".prototxt");

        updateUI();

        videoCapture.release();
        updateShortcut();
    }

}

cv::Mat MainWindow::getVideoImage(int index)
{
    videoCapture.set(cv::CAP_PROP_POS_FRAMES, index);
    cv::Mat img;
    videoCapture.read(img);
    return img;
}

void MainWindow::videoPrvFrame()
{
    if(!videoCapture.isOpened() || now_Index - video_Step < 1)
        return;

    cv::Mat img = getVideoImage(now_Index -= video_Step);

	predictionBoxAndDraw(img);
    setImage(img);
    updateUI();
}

void  MainWindow::prvImage()
{
    if(filePathList.empty())
        return;
    if(now_Index == 0)
        return;

    cv::Mat img = cv::imread(filePathList[--now_Index].absoluteFilePath().toLocal8Bit().data());

    if(img.empty())
    {
        return;
    }
    fileName = filePathList[now_Index].baseName();
    setImage(img, txtDir + filePathList[now_Index].baseName() + ".prototxt");

    updateUI();

}

void MainWindow::videoNexFrame()
{

    if(!videoCapture.isOpened() || now_Index + video_Step > total_Image){
        if(filePathList.empty())
            openFile();
        return;
    }

    cv::Mat img = getVideoImage(now_Index += video_Step);

    setImage(img);
	predictionBoxAndDraw(img);
    updateUI();

}

void MainWindow::nextImage()
{
    if(filePathList.empty())
        return;
    if(now_Index == total_Image)
        return;

    cv::Mat img = cv::imread(filePathList[++now_Index].absoluteFilePath().toLocal8Bit().data());

    if(img.empty())
    {
        return;
    }
    fileName = filePathList[now_Index].baseName();
    setImage(img, txtDir + filePathList[now_Index].baseName() + ".prototxt");

    updateUI();

}

void MainWindow::predictionBoxAndDraw(const cv::Mat &src_img)
{
	if (!ui->PredictImage->isChecked()) return;
	QTcpSocket socket(this);
	socket.connectToHost("127.0.0.1", 14333);
	unsigned int width = 1;
	unsigned int height = 1;

	cv::Mat img;
	cv::resize(src_img, img, cv::Size(416, 416));
	if (socket.waitForConnected()) {
		socket.write((const char *)&width, sizeof(unsigned int) / sizeof(const char));
		socket.write((const char *)&height, sizeof(unsigned int) / sizeof(const char));
		socket.write((const char *)img.data, img.cols * img.rows * img.elemSize());
		socket.flush();
	}
	if (socket.waitForReadyRead()) {
		QByteArray data =  socket.readAll();
		auto float_data = (float *)data.data();
		QString className("airplane");
        std::map<QString, QColor>::iterator color = classMap.find(className);
		if (color == classMap.end()) {
			int R = qrand() % 255;
            int G = qrand() % 255;
            int B = qrand() % 255;
            classMap[className] = QColor(R, G, B);
			color = classMap.find(className);
            ui->comboBox->addItem(className);
            updateComboBoxColor();
		}
		QList<QString>classlist;
		classlist.append(className);
		for (int i = 0; i < data.length() / sizeof(float) / 4; ++i) {
		    float w = float_data[i * 4 + 2];
			float h = float_data[i * 4 + 3];
			float x = float_data[i * 4 + 0];
			float y = float_data[i * 4 + 1];
			x = std::max(0.0f, x);
			y = std::max(0.0f, y);
			std::cout << x << " " << y << " " << w << " " << h << std::endl;
            ui->widget->addRectItem(x, y,  std::min(x+w, 1.0f), std::min(y+h, 1.0f), color->second, classlist);
		}
	}
}

void MainWindow::saveFile()
{
    if(fileName == "")
        return;

    QStringList list = fileName.split(' ');
    QString saveFileName;
    foreach (QString pice, list) {
        saveFileName += pice;
    }
    ui->widget->setSelectAble(false);
    ui->widget->setSelectMode(CreateMode);

    QList<QGraphicsItem*> graphicItem = ui->widget->items();
    if(graphicItem.count() < 4)
        return;

    QDir saveDir;
    if(videoCapture.isOpened())
    {
        saveDir.setPath(opctionsWindow.getOutPutPath() + "/" + saveFileName);
        if(!saveDir.exists())
            QDir().mkdir(saveDir.path());
    } else
    {
        saveDir.setPath(opctionsWindow.getOutPutPath() + "/Images");
        if(!saveDir.exists())
            QDir().mkdir(saveDir.path());
    }


    QDir saveDir_txt(saveDir.path() + "/proto");
    if(!saveDir_txt.exists())
        QDir().mkdir(saveDir_txt.path());

    QDir saveDir_img(saveDir.path() + "/image");
    if(!saveDir_img.exists())
        QDir().mkdir(saveDir_img.path());

	int originWidth = ui->widget->getOriginImage().cols,
		originHeight = ui->widget->getOriginImage().rows;

	RectData::PictureData pictureData;
	pictureData.set_width(originWidth);
	pictureData.set_height(originHeight);

	bool have_item = false;
	foreach(QGraphicsItem *item, graphicItem) {
		if (item->type() == SocketGraphicsItem::Type) {
			SocketGraphicsItem *rectItem = static_cast<SocketGraphicsItem*>(item);
			QList<QString> classList = rectItem->getClasses();
			if (!classList.empty())
			{
				/* save to txt */
				QRectF rect = rectItem->mapRectToScene(rectItem->boundingRect());

				int   x = (float)rect.x() * ((float)originWidth / ui->widget->rect().width()),
					y = (float)rect.y() * ((float)originHeight / ui->widget->rect().height()),
					w = (float)rect.width() * ((float)originWidth / ui->widget->rect().width()),
					h = (float)rect.height() * ((float)originHeight / ui->widget->rect().height());
				
				if (x + w <= 0)
					continue;
				if (x > originWidth)
					continue;
				if (y > originHeight)
					continue;
				if (y + h <= 0)
					continue;
				if (x < 0)
					x = 0;
				if (x + w > originWidth)
					w = (originWidth - x);
				if (y < 0)
					y = 0;
				if (y + h > originHeight)
					h = (originHeight - y);

				have_item = true;
				RectData::ObjectParam* object_param = pictureData.add_object_parameter();
				object_param->set_xmin(x);
				object_param->set_ymin(y);
				object_param->set_xmax(x + w);
				object_param->set_ymax(y + h);

				for (auto iterator = classList.begin(); iterator != classList.end(); ++iterator) {
					object_param->add_tag(iterator->toStdString());
				}
			}
		}
	}
	QFile txtFile;
	if (videoCapture.isOpened())
	{
		txtFile.setFileName(saveDir_txt.path() + "/" + saveFileName + "_" + QString::number(now_Index) + ".prototxt");
	}
	else
	{
		txtFile.setFileName(saveDir_txt.path() + "/" + saveFileName + ".prototxt");
	}

	ui->widget->deleteAllItems();
	if (have_item) {
		if (txtFile.open(QIODevice::WriteOnly)) {
			QTextStream out(&txtFile);

			std::string tmp;
			google::protobuf::TextFormat::PrintToString(pictureData, &tmp); //message to prototxt --ascii
			out << QString(tmp.c_str());

			txtFile.close();
		}

		if (videoCapture.isOpened()) {
			cv::imwrite((saveDir_img.path() + "/" +
				saveFileName + "_" + QString::number(now_Index) + ".png").toLocal8Bit().data(),
				ui->widget->getOriginImage());
		}
		else
		{
			cv::imwrite((saveDir_img.path() + "/" +
				saveFileName + ".png").toLocal8Bit().data(),
				ui->widget->getOriginImage());
		}


		if (videoCapture.isOpened())
		{
			videoNexFrame();
		}
		else if (!filePathList.empty())
		{
			nextImage();
		}
		else
		{
			fileName = "";
			openFile();
		}
	}
}

void MainWindow::addItem()
{
    if(ui->ItemName->text() == "")
        return;
    QColor color = QColorDialog::getColor();

    if(classMap.find(ui->ItemName->text()) != classMap.end()) // modify text color
    {
        classMap[ui->ItemName->text()] = color;
        comboBoxChanged(ui->ItemName->text());
        return;
    }

    classMap[ui->ItemName->text()] = color;

    ui->comboBox->addItem(ui->ItemName->text());
    ui->comboBox->setCurrentIndex(
                ui->comboBox->count() - 1);
    ui->ItemName->setText("");
    updateComboBoxColor();
}

void MainWindow::deleteItem()
{
    if(ui->comboBox->count() == 0)
        return;
    QString name = ui->comboBox->currentText();
    ui->comboBox->removeItem(ui->comboBox->currentIndex());
    classMap.erase(classMap.find(name));
}

void MainWindow::comboBoxChanged(QString name)
{
    QColor color = classMap[name];

    QList<QString> classList;
    classList.push_back(name);

    ui->widget->setRectInfo(color, classList);
    foreach (QGraphicsItem *item, ui->widget->selectItems()) {
        if(item->type() == SocketGraphicsItem::Type) {
            SocketGraphicsItem *socketItem = static_cast<SocketGraphicsItem*>(item);
            ui->widget->changeRectItemColor(socketItem, color);
            socketItem->setList(classList);
        }
    }

    updateComboBoxColor();
}

void MainWindow::updateComboBoxColor()
{
    for(int index = 0; index < ui->comboBox->count(); ++index)
    {
        ui->comboBox->setItemData(index, classMap[ui->comboBox->itemText(index)], Qt::TextColorRole);
    }
}

void MainWindow::loadSetting()
{
    QFileInfo file(SettingFilePath);
    if(!file.exists())
        return;
    QSettings settings(SettingFilePath, QSettings::IniFormat);

    /* Load Item */
    int counter = settings.beginReadArray("Items");
    for(int index = 0; index < counter; ++index)
    {
        settings.setArrayIndex(index);
        QString name = settings.value("Name").toString();
        QColor color(settings.value("Red").toInt(),
                     settings.value("Green").toInt(),
                     settings.value("Blue").toInt());

        classMap[name] = color;
        ui->comboBox->addItem(name);
    }
    comboBoxChanged(ui->comboBox->currentText());
    settings.endArray();
    /* End */

    /* MultiClasses */
    QList<QString> classesList;
    counter = settings.beginReadArray("MultiClasses");
    for(int index = 0; index < counter; ++index)
    {
        settings.setArrayIndex(index);
        QString a = settings.value("Class").toString();
        classesList.push_back(a);
    }
    ui->widget->setMultiClassesList(classesList);
    settings.endArray();
    /* End */

    /* General */
    filePath = settings.value("Def File Path").toString();
    ui->FrameStep->setValue(settings.value("Video Frame Step").toInt());
    video_Step = ui->FrameStep->value();
    /* End */
}

void MainWindow::saveSetting()
{
    QSettings settings(SettingFilePath, QSettings::IniFormat);

    /* Save Item */
    int counter = ui->comboBox->count();
    settings.beginWriteArray("Items");

    for(int index = 0; index < counter; ++index)
    {

        QString name = ui->comboBox->itemText(index);//ui->comboBox->currentText();

        QColor color = classMap[name];
        settings.setArrayIndex(index);
        settings.setValue("Name", name);
        settings.setValue("Red", color.red());
        settings.setValue("Green", color.green());
        settings.setValue("Blue", color.blue());
    }

    settings.endArray();
    /* End */

    /* MultiClasses List */
    QList<QString> classList(ui->widget->getMultiClassesList());
    counter = classList.count();
    settings.beginWriteArray("MultiClasses");

    for(int index = 0; index < counter; ++index)
    {
        settings.setArrayIndex(index);
        settings.setValue("Class", classList[index]);
    }
    settings.endArray();
    /* End */

    /* General */
    settings.setValue("Def File Path", filePath);
    settings.setValue("Video Frame Step", ui->FrameStep->value());
    /* End */
}

void MainWindow::videoSliderChanged(int index)
{
	if (now_Index == index) return;
    cv::Mat img;
    if(videoCapture.isOpened()){
        img = getVideoImage(++now_Index);
    }
    else if(!filePathList.empty())
    {
		now_Index = index;
        img = cv::imread(filePathList[now_Index].absoluteFilePath().toLocal8Bit().data());
        fileName = filePathList[now_Index].baseName();
    }
    else
    {
        ui->VideoSlider->setValue(0);
        return;
    }

    updateUI();

    setImage(img);

}

void MainWindow::setOpctions()
{
    opctionsWindow.show();
    updateShortcut();
}

void MainWindow::videoStepChange(int video_Step)
{
    this->video_Step = video_Step;
}

void MainWindow::setImage(cv::Mat &img)
{
    float scaleX = (img.cols < img.rows) ? (static_cast<float>(img.cols) / img.rows) : 1;
    float scaleY = (img.cols > img.rows) ? (static_cast<float>(img.rows) / img.cols) : 1;

    ui->widget->setGeometry(QRect(10, 10,
                                  scaleX * MAX_IMAGE_SIZE,
                                  scaleY * MAX_IMAGE_SIZE));

    ui->widget->setImage(img);

    cv::waitKey(DELAY_TIME); // make it delay
}

void MainWindow::setImage(cv::Mat &img, QString txtPath)
{
    ui->widget->deleteAllItems();
    QFile txt(txtPath);
    if(txt.open(QIODevice::ReadOnly))
    {
        QTextStream in(&txt);
        QString tmp = in.readAll();
        std::string file = tmp.toLocal8Bit().data();
        RectData::PictureData pictureData;
        bool success = google::protobuf::TextFormat::ParseFromString(file, &pictureData);
        if(success)
        {
            float scaleX = (img.cols < img.rows) ? (static_cast<float>(img.cols) / img.rows) : 1;
            float scaleY = (img.cols > img.rows) ? (static_cast<float>(img.rows) / img.cols) : 1;

            ui->widget->setGeometry(QRect(10, 10,
                                          scaleX * MAX_IMAGE_SIZE,
                                          scaleY * MAX_IMAGE_SIZE));

            ui->widget->setImage(img);

            for(google::protobuf::RepeatedPtrField<::RectData::ObjectParam>::const_iterator iterator = pictureData.object_parameter().begin();
                iterator != pictureData.object_parameter().end(); ++iterator)
            {
                QList<QString> classList;
                for(google::protobuf::RepeatedPtrField<std::string>::const_iterator tagIterator = iterator->tag().begin();
                    tagIterator != iterator->tag().end(); ++tagIterator){
                    classList.push_back(QString(tagIterator->c_str()));
                }
                if(iterator->tag_size() == 1){
                    std::map<QString, QColor>::iterator color = classMap.find(classList[0]);
                    if(color != classMap.end())
                        ui->widget->addRectItem(iterator->xmin() / static_cast<float>(pictureData.width()),
                                                iterator->ymin() / static_cast<float>(pictureData.height()),
                                                iterator->xmax() / static_cast<float>(pictureData.width()),
                                                iterator->ymax() / static_cast<float>(pictureData.height()), color->second, classList);
                    else{
                        int R = qrand() % 255;
                        int G = qrand() % 255;
                        int B = qrand() % 255;
                        classMap[classList[0]] = QColor(R, G, B);
                        ui->widget->addRectItem(iterator->xmin() / static_cast<float>(pictureData.width()),
                                                iterator->ymin() / static_cast<float>(pictureData.height()),
                                                iterator->xmax() / static_cast<float>(pictureData.width()),
                                                iterator->ymax() / static_cast<float>(pictureData.height()),
                                                classMap[classList[0]], classList);
                        ui->comboBox->addItem(classList[0]);
                        updateComboBoxColor();
                    }
                }
                else
                {
                    ui->widget->addRectItem(iterator->xmin() / static_cast<float>(pictureData.width()),
                                            iterator->ymin() / static_cast<float>(pictureData.height()),
                                            iterator->xmax() / static_cast<float>(pictureData.width()),
                                            iterator->ymax() / static_cast<float>(pictureData.height()),
                                            QColor(0,0,0), classList);
                }
            }
        }

        cv::waitKey(DELAY_TIME); // make it slow down
    }
    else
    {
        setImage(img);
    }

}

void MainWindow::widghtItemNumberChange(int num)
{
    this->statusBar()->showMessage("Item Quantity: " +
                               QString::number(num));
}

MainWindow::~MainWindow()
{
    saveSetting();
    delete ui;
#ifdef USE_CAFFE_
    delete net;
#endif
    delete CreateMode;
    delete SelectMode;
    delete ResizeMode;
}
