#include "clogpane.h"


CLogPane* LogPane;

CLogPane::CLogPane(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* vbox = new QVBoxLayout(this);

    ListView = new QListView(this);
    ListModel = new QStringListModel(this);
    ToolBar = new QToolBar(this);

    ToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ToolBar->setIconSize(QSize(16, 16));


    QAction *clearAction = new QAction(
        style()->standardIcon(QStyle::SP_MessageBoxCritical),
        tr("&Clear Log"),
        this
    );
    ToolBar->addAction(clearAction);


    connect(clearAction, &QAction::triggered, this, [this]() {
        List.clear();
        ListModel->setStringList(List);
    });


    ListModel->setStringList(List);
    ListView->setModel(ListModel);
    ListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    vbox->addWidget(ToolBar);
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

