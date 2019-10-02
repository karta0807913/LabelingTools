#ifndef FUNCTOR_H
#define FUNCTOR_H

#include <QGraphicsSceneMouseEvent>
#include "socketgraphicsitem.h"
#include <QGraphicsRectItem>
#include <QList>
#include <QRectF>
#include <QColor>

template <int I>
struct Int2Type{};

enum Mode{
    DrawRect,
    SelectRect,
    RectResize
};


enum MouseMode { Press ,
                 Move  ,
                 Relese,
                 ChangeMode
               };


template<typename SCENE>
class Functor
{
public:
    Functor(SCENE*){}
    virtual SocketGraphicsItem* operator()(QColor , QGraphicsSceneMouseEvent *, Int2Type<MouseMode::Press>){ return nullptr; }
    virtual void operator()(SocketGraphicsItem *&, QGraphicsSceneMouseEvent *, Int2Type<MouseMode::Move>){}
    virtual void operator()(SocketGraphicsItem *&, QGraphicsSceneMouseEvent *, Int2Type<MouseMode::Relese>){}
    virtual void operator()(SocketGraphicsItem *&, Int2Type<MouseMode::ChangeMode>){}
    virtual ~Functor(){}

protected:
    SCENE *scene;
};

template<int I, typename SCENE>
class DearMode : public Functor<SCENE>{};

    /* DrawMode */
template<typename SCENE>
class DearMode<DrawRect, SCENE>
        : public Functor<SCENE>
{
public:
    DearMode(SCENE *scene) : Functor<SCENE>(scene)
    {
        this->scene = scene;
    }

    SocketGraphicsItem* operator()(QColor color, QGraphicsSceneMouseEvent * event, Int2Type<MouseMode::Press>){
        SocketGraphicsItem *drawItem = new SocketGraphicsItem();
        QList<QString> classList = this->scene->getPlotClassesList();
        this->scene->addItem(drawItem, classList);

        color.setAlpha(25);
        drawItem->setBrush(color);
        color.setAlpha(255);
        drawItem->setPen(QPen(color, 1));

        startX = event->scenePos().x();
        startY = event->scenePos().y();

        drawItem->setRect(event->scenePos().x(), event->scenePos().y(), 0, 0);
        return drawItem;
    }

    void operator()(SocketGraphicsItem *& item, QGraphicsSceneMouseEvent * event, Int2Type<MouseMode::Move>){
        if(item == nullptr)
            return;

        int x, y, w, h;

        x = (event->scenePos().x() > startX)? startX : event->scenePos().x();
        y = (event->scenePos().y() > startY)? startY : event->scenePos().y();

        w = ((int)event->scenePos().x() ^ startX ^ x) - x;
        h = ((int)event->scenePos().y() ^ startY ^ y) - y;

        item->setRect(x, y, w, h);
    }

    void operator()(SocketGraphicsItem *&item, QGraphicsSceneMouseEvent *, Int2Type<MouseMode::Relese>){

        if(item == nullptr)
            return;
        if(item->rect().width() < 10 && item->rect().height() < 10)
            this->scene->deleteItem(item);
        item = nullptr;
    }

    void operator()(SocketGraphicsItem *&item, Int2Type<MouseMode::ChangeMode>)
    {
        if(item == nullptr)
            return;
        delete item;
        item = nullptr;
    }

private:
    int startX;
    int startY;
};

    /* Move Mode */
template<typename SCENE>
class DearMode<SelectRect, SCENE>
        : public Functor<SCENE>
{
public:
    DearMode(SCENE *scene) : Functor<SCENE>(scene)
    {
        this->scene = scene;
    }

};
    /* Resize Mode */
template<typename SCENE>
class DearMode<RectResize, SCENE>
        :public Functor<SCENE>
{
public:
    DearMode(SCENE *scene) : Functor<SCENE>(scene)
    {
        this->scene = scene;
    }

    SocketGraphicsItem* operator()(QColor , QGraphicsSceneMouseEvent *, Int2Type<MouseMode::Press>)
    {
        QList<QGraphicsItem*> itemList = this->scene->selectedItems();
        int childWidth  = 20;
        int childHeight = 20;
        if(itemList.count() == 0)
        {
            this->scene->setSelectAble(true);
            if(controlItems[0] != nullptr)
                for(int index = 0; index < 4; ++index)
                {
                    delete controlItems[index];
                    controlItems[index] = nullptr;
                }
        }
        else
        {
            if(controlItems[0] == nullptr)
            {
                QGraphicsRectItem *rectItem = static_cast<QGraphicsRectItem *>(itemList[0]);
                QRectF parentRect = rectItem->rect();

                this->scene->setSelectAble(false);

                for(int index = 0; index < 4; ++index)
                {
                    controlItems[index] = new QGraphicsRectItem(rectItem);
                    controlItems[index]->setBrush(Qt::black);
                    controlItems[index]->setFlag(QGraphicsItem::ItemIsSelectable, true);
                    controlItems[index]->setFlag(QGraphicsItem::ItemIsMovable, true);
                }

                controlItems[0]->setRect(parentRect.x() - childWidth / 2, parentRect.y() - childHeight / 2, childWidth, childHeight);
                controlItems[1]->setRect(parentRect.x() - childWidth / 2, parentRect.y() - childHeight / 2 + parentRect.height(), childWidth, childHeight);
                controlItems[2]->setRect(parentRect.x() - childWidth / 2 + parentRect.width(), parentRect.y() - childHeight / 2, childWidth, childHeight);
                controlItems[3]->setRect(parentRect.x() - childWidth / 2 + parentRect.width(), parentRect.y() - childHeight / 2 + parentRect.height(), childWidth, childHeight);

                originPosX          = parentRect.x();
                originPosY          = parentRect.y();
                originRectHeight    = parentRect.height();
                originRectWidth     = parentRect.width();
            }
        }
        return nullptr;
    }

    void operator()(SocketGraphicsItem *&, QGraphicsSceneMouseEvent *, Int2Type<MouseMode::Move>)
    {
        if(controlItems[0] == nullptr)
            return;

       /*0**********2
         *          *
         *          *
         *          *
         *          *
         *          *
         *          *
         1**********3*/

        if(controlItems[0]->isSelected())
        {
            controlItems[1]->setPos(controlItems[0]->pos().x(), controlItems[1]->pos().y());
            controlItems[2]->setPos(controlItems[2]->pos().x(), controlItems[0]->pos().y());
        }
        else if(controlItems[1]->isSelected())
        {
            controlItems[0]->setPos(controlItems[1]->pos().x(), controlItems[0]->pos().y());
            controlItems[3]->setPos(controlItems[3]->pos().x(), controlItems[1]->pos().y());
        }
        else if(controlItems[2]->isSelected())
        {
            controlItems[0]->setPos(controlItems[0]->pos().x(), controlItems[2]->pos().y());
            controlItems[3]->setPos(controlItems[2]->pos().x(), controlItems[3]->pos().y());
        }
        else if(controlItems[3]->isSelected())
        {
            controlItems[1]->setPos(controlItems[1]->pos().x(), controlItems[3]->pos().y());
            controlItems[2]->setPos(controlItems[3]->pos().x(), controlItems[2]->pos().y());
        }

        QGraphicsRectItem *parentItem = static_cast<QGraphicsRectItem*>(controlItems[0]->parentItem());
        parentItem->setRect(originPosX + controlItems[0]->pos().x(), originPosY + controlItems[0]->pos().y(),
                originRectWidth - controlItems[0]->pos().x() + controlItems[3]->pos().x(),
                originRectHeight - controlItems[0]->pos().y() + controlItems[3]->pos().y());
    }

    void operator()(SocketGraphicsItem *&, QGraphicsSceneMouseEvent *, Int2Type<MouseMode::Relese>)
    {
    }

    void operator()(SocketGraphicsItem *&, Int2Type<MouseMode::ChangeMode>)
    {
        if(controlItems[0] != nullptr)
            for(int index = 0; index < 4; ++index)
            {
                delete controlItems[index];
                controlItems[index] = nullptr;
            }
    }

private:
    QGraphicsRectItem *controlItems[4] = { nullptr };
    int originPosX;
    int originPosY;
    int originRectHeight;
    int originRectWidth;
};

#endif // FUNCTOR_H
