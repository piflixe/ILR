/****************************************************************************
** Meta object code from reading C++ file 'ilr.h'
**
** Created: Fri May 15 17:47:06 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../FP_MA/ilr.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ilr.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ILR[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       5,    4,    4,    4, 0x0a,
      12,    4,    4,    4, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ILR[] = {
    "ILR\0\0loop()\0changei()\0"
};

void ILR::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ILR *_t = static_cast<ILR *>(_o);
        switch (_id) {
        case 0: _t->loop(); break;
        case 1: _t->changei(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ILR::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ILR::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ILR,
      qt_meta_data_ILR, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ILR::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ILR::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ILR::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ILR))
        return static_cast<void*>(const_cast< ILR*>(this));
    return QObject::qt_metacast(_clname);
}

int ILR::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
