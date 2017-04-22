#include "socketgraphicsitem.h"

SocketGraphicsItem::SocketGraphicsItem(QObject *parent) :
    QObject(parent), QGraphicsRectItem(nullptr)
{
}

void SocketGraphicsItem::setList(QList<QString> &classList)
{
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

SocketGraphicsItem::~SocketGraphicsItem()
{
    this->disconnect();
}
