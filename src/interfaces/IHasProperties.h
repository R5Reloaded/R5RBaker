#ifndef IHASPROPERTIES_H
#define IHASPROPERTIES_H

#include <QPoint>


class IHasProperties {
public:
    virtual class QObject* getPropertiesObject(QPoint pos) = 0;
};



#endif // IHASPROPERTIES_H
