#ifndef CLASSESDIALOG_H
#define CLASSESDIALOG_H

#include <QString>
#include <QList>
#include <QDialog>
#include <QListWidgetItem>
#include <QModelIndex>

namespace Ui {
class ClassesDialog;
}

class ClassesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClassesDialog(QWidget *parent = 0);
    void addListItem(QString name);
    void setCheckedItem(QList<QString> list);
    QList<QString> getClassList();
    QList<QString> getAllClasses();
    ~ClassesDialog();

private:
    Ui::ClassesDialog *ui;

private slots:
    void addListItem();
    void deleteListItem();
};

#endif // CLASSESDIALOG_H
