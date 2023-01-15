#ifndef MACROS_H
#define MACROS_H


#define ASSET_META_PROPERTY(type, name)\
    Q_PROPERTY(type name READ get##name WRITE set##name NOTIFY name##Changed USER("saveToMeta=true"))\
    private:\
    type name;\
    public:\
    type get##name() const {return name;}\
    void set##name(type name) { if(this->name != name) {this->name=name; emit name##Changed();}}\
    signals: void name##Changed();

#endif // MACROS_H
