#ifndef IHASPROPERTIES_H
#define IHASPROPERTIES_H

#include <QObject>
#include <QPoint>


class IHasProperties {
public:
    virtual class QObject* getPropertiesObject(QPoint pos) = 0;
    virtual void propertiesUpdated() = 0;
};



#endif // IHASPROPERTIES_H
