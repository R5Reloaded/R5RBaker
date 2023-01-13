#include "cdependencypane.h"
#include "../logic/models/cassetgraphmodel.h"
#include "../views/cdependencytreeview.h"

#include <QBoxLayout>
#include <QTreeView>

CDependencyPane::CDependencyPane(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* vbox = new QVBoxLayout(this);
    CAssetGraphModel* rpakModel = new CAssetGraphModel(this);
    CDependencyTreeView* treeView = new CDependencyTreeView(this);

    vbox->addWidget(treeView);
    treeView->setModel(rpakModel);
    treeView->setHeaderHidden(true);
}

