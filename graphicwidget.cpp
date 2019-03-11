#include "graphicwidget.h"
#include "ui_graphicwidget.h"

GraphicWidget::GraphicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphicWidget)
{
    ui->setupUi(this);
    empty = new Functor<GraphicScene>(nullptr);
    scene = new GraphicScene(this, empty);

    ui->graphicsView->setGeometry(rect());
    scene->setSceneRect(rect());
    ui->graphicsView->setScene(scene);

    this->parent = parent;
    this->setMouseTracking(true);
    emit itemNumberChanged(0);
}

void GraphicWidget::setGeometry(const QRect &rect)
{
    ui->graphicsView->setGeometry(10, 10, rect.width() + 2, rect.height() + 2);
    scene->setSceneRect(10, 10, rect.width(), rect.height());
    imgWidth = rect.width();
    imgHeight = rect.height();
    QWidget::setGeometry(rect);
}

void GraphicWidget::setImage(QString path)
{
    scene->setImage(path, imgWidth, imgHeight);
}

void GraphicWidget::setImage(cv::Mat img)
{
    scene->setImage(img,  imgWidth, imgHeight);
}

void GraphicWidget::addRectItem(float x1, float y1, float x2, float y2, QColor color, QList<QString> &classList)
{
    if(x1 >= x2 || y1 >= y2 || x1 < 0 || y1 < 0)
        return;
    SocketGraphicsItem *rectItem = new SocketGraphicsItem();
    color.setAlpha(50);
    rectItem->setBrush(color);
    color.setAlpha(255);
    rectItem->setPen(QPen(color, 1));

    rectItem->setRect(
                x1 * scene->sceneRect().width(),
                y1 * scene->sceneRect().height(),
                (x2 - x1) * scene->sceneRect().width(),
                (y2 - y1) * scene->sceneRect().height());

//    connect(rectItem, SIGNAL(mouseDoubleClickSignal(SocketGraphicsItem*)),
//            this, SLOT(mouseDoubleClickSignal(SocketGraphicsItem*)));

    scene->addItem(rectItem, classList);
    emit itemNumberChanged(scene->getGraphicItemsNum());
}

void GraphicWidget::changeRectItemColor(SocketGraphicsItem *rectItem, QColor color)
{
    color.setAlpha(50);
    rectItem->setBrush(color);
    color.setAlpha(255);
    rectItem->setPen(QPen(color, 1));
}

void GraphicWidget::setSelectMode(Functor<GraphicScene> *functor)
{
    if(functor == nullptr)
        functor = empty;
    scene->setSelectMode(functor);
    emit itemNumberChanged(scene->getGraphicItemsNum());
}

void GraphicWidget::setSelectAble(bool state)
{
    if(state)
        ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    else
        ui->graphicsView->setDragMode(QGraphicsView::NoDrag);

    scene->setSelectAble(state);
}

void GraphicWidget::deleteAllItems()
{
    scene->deleteAllItems();

    emit itemNumberChanged(scene->getGraphicItemsNum());
}

void GraphicWidget::update()
{
    ui->graphicsView->update();
    emit itemNumberChanged(scene->getGraphicItemsNum());
    parent->update();
}

GraphicScene *GraphicWidget::getMainScene()
{
    return scene;
}

void GraphicWidget::setRectInfo(QColor color, QList<QString> &classList)
{
    scene->setRectInfo(color, classList);
}

void GraphicWidget::setMultiClassesList(QList<QString> classesList)
{
    scene->setMultiClassesList(classesList);
}

QList<QGraphicsItem*> GraphicWidget::items()
{
    return scene->items();
}

QList<QGraphicsItem *> GraphicWidget::selectItems()
{
    return scene->selectedItems();
}

QList<QString> GraphicWidget::getMultiClassesList()
{
    return scene->getMultiClassesList();
}

void GraphicWidget::setDeleteHotKey(Qt::Key key)
{
    scene->setDeleteHotKey(key);
}

cv::Mat &GraphicWidget::getOriginImage()
{
    return scene->getOriginImage();
}

void GraphicWidget::mouseReleaseEvent(QMouseEvent *event)
{
    emit itemNumberChanged(scene->getGraphicItemsNum());
    QWidget::mouseReleaseEvent(event);
}

GraphicWidget::~GraphicWidget()
{
    delete ui;
    delete scene;
    delete empty;
}
