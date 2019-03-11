#include "classesdialog.h"
#include "ui_classesdialog.h"

ClassesDialog::ClassesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClassesDialog)
{
    ui->setupUi(this);
    connect(ui->deleteItemButton, SIGNAL(clicked()), this, SLOT(deleteListItem()));
    connect(ui->addItemButton   , SIGNAL(clicked()), this, SLOT(addListItem()));
}

void ClassesDialog::addListItem()
{
    addListItem(ui->nameBox->text());
}

void ClassesDialog::addListItem(QString name)
{
    if(name == "")
        return;
    if(ui->listWidget->findItems(name, Qt::MatchExactly).count() != 0)
        return;
    QListWidgetItem *item = new QListWidgetItem;
    item->setData(Qt::DisplayRole, name);
    item->setData(Qt::CheckStateRole, Qt::Checked);
    ui->listWidget->addItem(item);
}

void ClassesDialog::setCheckedItem(QList<QString> list)
{
    int counter = ui->listWidget->count();
    for(int index = 0; index < counter; ++index) {
        if(list.indexOf(ui->listWidget->item(index)->text()) == -1) {
            ui->listWidget->item(index)->setCheckState(Qt::Unchecked);
        }
        else{
            ui->listWidget->item(index)->setCheckState(Qt::Checked);
        }
    }
}

QList<QString> ClassesDialog::getAllClasses()
{
    QList<QString> list;
    for(int index = 0; index < ui->listWidget->count(); ++index) {
        list.push_back(ui->listWidget->item(index)->text());
    }
    return list;
}

void ClassesDialog::deleteListItem()
{
    qDeleteAll(ui->listWidget->selectedItems());
}

QList<QString> ClassesDialog::getClassList()
{
    QList<QString> selectList;
    for(int index = 0; index < ui->listWidget->count(); ++index) {
        QListWidgetItem* item = ui->listWidget->item(index);
        if(item->checkState() == Qt::Checked) {
            selectList.push_back(item->text());
        }
    }
    return selectList;
}

ClassesDialog::~ClassesDialog()
{
    delete ui;
}
