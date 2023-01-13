#ifndef MACROS_H
#define MACROS_H


#define C_SIMPLE_PROPERTY(type, name)\
    Q_PROPERTY(type name READ get##name WRITE set##name NOTIFY name##Changed)\
    public:\
    type name;\
    type get##name() const {return name;}\
    void set##name(type name) { if(this->name != name) {this->name=name; emit name##Changed();}}\
    signals: void name##Changed();

#endif // MACROS_H
