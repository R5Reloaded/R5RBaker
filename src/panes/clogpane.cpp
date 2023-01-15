#include "clogpane.h"


CLogPane* LogPane;

CLogPane::CLogPane(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* vbox = new QVBoxLayout(this);

    ListView = new QListView(this);
    ListModel = new QStringListModel(this);

    ListModel->setStringList(List);
    ListView->setModel(ListModel);
    ListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    vbox->addWidget(ListView);
}

CLogPane& CLogPane::operator<<(QVariant entry)
{
    if(ListModel->insertRow(ListModel->rowCount())) {
        QModelIndex index = ListModel->index(ListModel->rowCount() - 1, 0);
        ListModel->setData(index, entry);
        ListView->scrollToBottom();
    }
    return *this;
}

void CLogPane::clearLog()
{
    List.clear();
    ListModel->setStringList(List);
}

