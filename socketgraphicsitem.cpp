#include "socketgraphicsitem.h"

SocketGraphicsItem::SocketGraphicsItem(QObject *parent) :
    QObject(parent), QGraphicsRectItem(nullptr)
{
    textItem.setParentItem(this);
}

void SocketGraphicsItem::setList(QList<QString> &classList)
{
    if(classList.count() == 1)
        textItem.setPlainText(classList[0]);
    else
        textItem.setPlainText("");
    this->classList = classList;
}

void SocketGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *item)
{
    emit mouseDoubleClickSignal(this);
    QGraphicsRectItem::mouseDoubleClickEvent(item);
}

QList<QString> SocketGraphicsItem::getClasses()
{
    return classList;
}

void SocketGraphicsItem::setRect(const QRectF &rect)
{
    textItem.setPos(rect.x(), rect.y());
    QGraphicsRectItem::setRect(rect);
}

void SocketGraphicsItem::setRect(qreal x, qreal y, qreal w, qreal h)
{
    textItem.setPos(x, y);
    QGraphicsRectItem::setRect(x, y, w, h);
}

//void SocketGraphicsItem::setPos(const QPointF &pos)
//{
//    textItem.setPos(pos.x(), pos.y());
//    QGraphicsRectItem::setPos(pos);
//}

//void SocketGraphicsItem::setPos(qreal x, qreal y)
//{
//    textItem.setPos(x, y);
//    QGraphicsRectItem::setPos(x, y);
//}

SocketGraphicsItem::~SocketGraphicsItem()
{
    this->disconnect();
}
