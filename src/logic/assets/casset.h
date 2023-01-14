#ifndef CASSET_H
#define CASSET_H

#include <QObject>

class CAsset : public QObject, public std::enable_shared_from_this<CAsset>, public IHasProperties, public IHasActions
{
    Q_OBJECT
public:
    QString Name;
    QString SavePath;

    QString getName() const;
    void setName(const QString &newName);


    explicit CAsset(QString Name, QObject *parent = nullptr);

    virtual void buildBeforeChildren(){}
    virtual void buildAfterChildren(){}

    virtual void buildMenu(class QMenu* Menu) override;
    virtual class QObject* getPropertiesObject(QPoint pos) override;


private:
    virtual void _buildMenu(QMenu* Menu){}

    Q_PROPERTY(QString Name READ getName WRITE setName NOTIFY NameChanged)

signals:
    void NameChanged();
};

#endif // CASSET_H
