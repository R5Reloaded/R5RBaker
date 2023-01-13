#ifndef CASSETGRAPHMODEL_H
#define CASSETGRAPHMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QApplication>
#include <QTimer>


class CAssetGraphModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CAssetGraphModel(QObject *parent = nullptr);
signals:


    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // CASSETGRAPHMODEL_H
