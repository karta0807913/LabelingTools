#ifndef SOCKETGRAPHICSITEM_H
#define SOCKETGRAPHICSITEM_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QString>
#include <QList>
#include <QObject>

class SocketGraphicsItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    explicit SocketGraphicsItem(QObject *parent = 0);
    void setList(QList<QString> &classList);
    QList<QString> getClasses();
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *);
    void setRect(const QRectF &rect);
    void setRect(qreal x, qreal y, qreal w, qreal h);
//    void setPos(qreal x, qreal y);
//    void setPos(const QPointF &pos);
     ~SocketGraphicsItem();

private:
    QList<QString> classList;
    QGraphicsTextItem textItem;

signals:
    void mouseDoubleClickSignal(SocketGraphicsItem *item);
};

#endif // SOCKETGRAPHICSITEM_H
