#ifndef MACROS_H
#define MACROS_H

#include <variant>
#include <QDateTime>
#include <QDate>
#include <QTime>

#define ASSET_META_PROPERTY(type, name)\
    private:\
    Q_PROPERTY(type name READ get##name WRITE set##name NOTIFY name##Changed USER("saveToMeta=true"))\
    type name;\
    public:\
    type get##name() const {return name;}\
    void set##name(type name) { if(this->name != name) {this->name=name; emit name##Changed();}}\
    signals: void name##Changed();


#define ASSET_META_PROPERTY_CUSTOM_SETTER(type, name)\
    private:\
    Q_PROPERTY(type name READ get##name WRITE set##name NOTIFY name##Changed USER("saveToMeta=true"))\
    type name;\
    public:\
    type get##name() const {return name;}\
    void set##name(type name);\
    signals: void name##Changed(){}
#endif // MACROS_H
