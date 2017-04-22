#ifndef SOCKETGRAPHICSITEM_H
#define SOCKETGRAPHICSITEM_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
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
    ~SocketGraphicsItem();

private:
    QList<QString> classList;

signals:
    void mouseDoubleClickSignal(SocketGraphicsItem *item);
};

#endif // SOCKETGRAPHICSITEM_H
