#include "cinspectorpane.h"
#include <QTableView>
#include <QStandardItemModel>
#include <QMetaProperty>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>

CInspectorPane::CInspectorPane(QWidget *parent)
    : QWidget{parent}
{
    vbox = new QVBoxLayout(this);
    Model = new QStandardItemModel(this);
    Table = new QTableView(this);

    TitleLabel = new QLabel(this);
    SubtitleLabel = new QLabel(this);
    MenuButton = new QPushButton(this);

    auto f = TitleLabel->font();
    f.setWeight(QFont::Bold);
    f.setPointSize(f.pointSize() + 2);
    TitleLabel->setFont(f);

    MenuButton->setText("Actions");

    vbox->addWidget(TitleLabel);
    vbox->addWidget(SubtitleLabel);
    vbox->addWidget(MenuButton);

    vbox->addWidget(Table);
    Table->setModel(Model);
    Table->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    Table->verticalHeader()->setHidden(true);
    Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(Model, &QStandardItemModel::dataChanged, this, &CInspectorPane::updateObjectProperty);

    connect(MenuButton, &QPushButton::clicked, this, [this]() {
        auto hasActions = dynamic_cast<IHasActions*>(CurrentObject);
        if(hasActions) {
            QMenu* menu = new QMenu();
            hasActions->buildMenu(menu);
            menu->exec(this->mapToGlobal(MenuButton->pos()));
            delete menu;
        }
    });
}

void CInspectorPane::setInspectorEnabled(bool enabled)
{
    vbox->setEnabled(enabled);
    for (int i = 0; i < vbox->count(); ++i) {
        QWidget* widget = vbox->itemAt(i)->widget();
        if (widget) {
            widget->setEnabled(enabled);
        }
    }
}

void CInspectorPane::setInspectedObject(QObject *object)
{
    if(!object) {
        setInspectorEnabled(false);
        return;
    } else {
        CurrentObject = object;
        setInspectorEnabled(true);
    }


    TitleLabel->setText(object->objectName());
    SubtitleLabel->setText(object->metaObject()->className());

    Model->clear();
    Model->setHorizontalHeaderItem(0, new QStandardItem("Name"));
    Model->setHorizontalHeaderItem(1, new QStandardItem("Value"));


    auto objectMeta = CurrentObject->metaObject();

    for(int i = 0; i < objectMeta->propertyCount(); i++) {
        auto prop = objectMeta->property(i);

        if(!QString::compare(prop.name(), "objectName")) continue;

        auto itemName = new QStandardItem();
        auto itemValue = new QStandardItem();

        QList<QStandardItem*> items;
        items.push_back(itemName);
        items.push_back(itemValue);

        itemName->setData(prop.name(), Qt::DisplayRole);
        itemValue->setData(CurrentObject->property(prop.name()), Qt::EditRole);
        Model->appendRow(items);
    }
    Table->resizeColumnsToContents();
}

void CInspectorPane::updateObjectProperty(const QModelIndex &index, const QModelIndex &)
{
    QStandardItem* itemValue = Model->itemFromIndex(index);
    if(itemValue->column() != 1)
        return;

    QStandardItem* itemName = Model->item(itemValue->row(), 0);
    CurrentObject->setProperty(itemName->text().toLatin1(), itemValue->data(Qt::EditRole));
}
