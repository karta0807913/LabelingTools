#include "graphicscene.h"
#include "convertimage.h"

#include <QDebug>

GraphicScene::GraphicScene(QWidget *parent, Functor<GraphicScene> *functor)
    : QGraphicsScene(parent), MultiClassColor(0,0,0)
{

    this->addItem(&pixmapItem);
    this->addItem(&auxiliiaryLine_X);
    this->addItem(&auxiliiaryLine_Y);

    auxiliiaryLine_X.setPen(QPen(Qt::green, 1, Qt::DashLine));
    auxiliiaryLine_Y.setPen(QPen(Qt::green, 1, Qt::DashLine));

    mouseEvent = functor;

    drawItem = nullptr;
    graphicItemNum = 0;
}

void GraphicScene::setImage(QString path, int height, int width)
{
    cv::Mat mat = cv::imread(path.toLocal8Bit().constData(), cv::IMREAD_COLOR);
    setImage(mat,
             height,
             width);
}

void GraphicScene::setImage(cv::Mat &img, int height, int width)
{
    cv::Mat resizeImg;
    cv::Mat reFilledImage;

#ifdef DONT_SCALE_IMAGE
    originImg = reFilledImage = fillUpImageToSqure(img);
#else
    originImg = reFilledImage = img;
#endif
    cv::resize(reFilledImage, resizeImg, cv::Size(
                   height,
                   width));

    displayImg = QPixmap::fromImage(Mat2QImage(resizeImg));
    pixmapItem.setPixmap(displayImg);
}

void GraphicScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    drawItem = (*mouseEvent)(rectColor, event, Int2Type<MouseMode::Press>());
}

void GraphicScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    auxiliiaryLine_X.setLine(0, event->scenePos().y(), 2048, event->scenePos().y());
    auxiliiaryLine_Y.setLine(event->scenePos().x(), 0, event->scenePos().x(), 2048);

    (*mouseEvent)(drawItem, event, Int2Type<MouseMode::Move>());

    QGraphicsScene::mouseMoveEvent(event);
}

void GraphicScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    (*mouseEvent)(drawItem, event, Int2Type<MouseMode::Relese>());
    drawItem = nullptr;

    QGraphicsScene::mouseReleaseEvent(event);
}

void GraphicScene::deleteAllItems()
{
    foreach (QGraphicsItem *item, items()) {
        if(SocketGraphicsItem::Type == item->type()){
            delete item;
        }
    }
    drawItem = nullptr;
    graphicItemNum = 0;
    itemCounter = 0;
}

void GraphicScene::setSelectAble(bool state)
{
    foreach (QGraphicsItem *item, items()) {
        if(item->type() == SocketGraphicsItem::Type){
            item->setFlag(QGraphicsItem::ItemIsSelectable, state);
            item->setFlag(QGraphicsItem::ItemIsMovable, state);
        }
    }
}

void GraphicScene::setDeleteHotKey(Qt::Key key)
{
    deleteHotKey = key;
}

void GraphicScene::addItem(QGraphicsItem *item)
{
    ++graphicItemNum;
    QGraphicsScene::addItem(item);
}

void GraphicScene::addItem(SocketGraphicsItem *item, QList<QString> &classList)
{
    ++graphicItemNum;
    connect(item, SIGNAL(mouseDoubleClickSignal(SocketGraphicsItem*)),
            this, SLOT(mouseDoubleClickSignal(SocketGraphicsItem*)));

    item->setList(classList);
    QGraphicsScene::addItem(item);
}

void GraphicScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete || event->key() == deleteHotKey){
        foreach (QGraphicsItem *item, selectedItems()) {
            delete item;
        }
    }
    graphicItemNum = items().count() - 3;
    QGraphicsScene::keyPressEvent(event);
}

bool GraphicScene::deleteItem(QGraphicsItem *item)
{
    if(this->items().indexOf(item) == -1)
        return false;
    delete item;
    --graphicItemNum;
    return true;
}

void GraphicScene::setSelectMode(Functor<GraphicScene> *functor)
{
    (*mouseEvent)(drawItem, Int2Type<MouseMode::ChangeMode>());
    mouseEvent = functor;
}

void GraphicScene::setMultiClassesList(QList<QString> classesList)
{
    for(auto iterator = classesList.begin(); iterator != classesList.end(); ++iterator){
        classesDialog.addListItem(*iterator);
    }
}

QList<QString> GraphicScene::getPlotClassesList()
{
    return rectClassList;
}

Size<float, float> GraphicScene::getScalingRate()
{
    return scalingRate;
}

void GraphicScene::setRectInfo(QColor color, QList<QString> &classList)
{
    rectColor = color;
    rectClassList = classList;
}

int GraphicScene::getGraphicItemsNum()
{
    return graphicItemNum;
}

cv::Mat &GraphicScene::getOriginImage()
{
    return originImg;
}

QList<QString> GraphicScene::getMultiClassesList()
{
    return classesDialog.getAllClasses();
}

void GraphicScene::mouseDoubleClickSignal(SocketGraphicsItem *item)
{
    QList<QString> tmp = item->getClasses();
    classesDialog.setCheckedItem(tmp);
    classesDialog.exec();
    QList<QString> classesList(classesDialog.getClassList());
    if(classesList.isEmpty())
        return;
//    foreach (QGraphicsItem selectItems, this->selectedItems()) {
//        if(selectItems.type() == SocketGraphicsItem::Type){
//            static_cast<SocketGraphicsItem>(selectItems).setList(classesList);
//        }
//    }
    item->setList(classesList);
    MultiClassColor.setAlpha(50);
    item->setBrush(MultiClassColor);
    MultiClassColor.setAlpha(255);
    item->setPen(QPen(MultiClassColor, 1));
}

GraphicScene::~GraphicScene()
{
}
