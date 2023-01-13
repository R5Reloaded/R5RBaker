#ifndef CASSET_H
#define CASSET_H

#include <QObject>
#include <QMenu>

class CAsset : public QObject, public std::enable_shared_from_this<CAsset>
{
    Q_OBJECT
public:
    QString Name;
    QString SavePath;

    explicit CAsset(QString Name, QObject *parent = nullptr);

    virtual void buildBeforeChildren(){}
    virtual void buildAfterChildren(){}
    void buildContextMenu(QMenu* Menu, class CGraphItem* associatedItem);

private:
    virtual void _buildContextMenu(QMenu* Menu){}

signals:

};

#endif // CASSET_H
