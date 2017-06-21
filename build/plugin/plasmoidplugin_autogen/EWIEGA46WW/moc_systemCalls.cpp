/****************************************************************************
** Meta object code from reading C++ file 'systemCalls.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../plugin/systemCalls.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'systemCalls.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_systemCalls_t {
    QByteArrayData data[7];
    char stringdata0[83];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_systemCalls_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_systemCalls_t qt_meta_stringdata_systemCalls = {
    {
QT_MOC_LITERAL(0, 0, 11), // "systemCalls"
QT_MOC_LITERAL(1, 12, 20), // "showProgressInqDebug"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 12), // "checkUpdates"
QT_MOC_LITERAL(4, 47, 9), // "arguments"
QT_MOC_LITERAL(5, 57, 17), // "upgradeConcurrent"
QT_MOC_LITERAL(6, 75, 7) // "getSize"

    },
    "systemCalls\0showProgressInqDebug\0\0"
    "checkUpdates\0arguments\0upgradeConcurrent\0"
    "getSize"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_systemCalls[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
       3,    1,   35,    2, 0x02 /* Public */,
       5,    0,   38,    2, 0x02 /* Public */,
       6,    0,   39,    2, 0x02 /* Public */,

 // slots: parameters
    QMetaType::Void,

 // methods: parameters
    QMetaType::QStringList, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Int,

       0        // eod
};

void systemCalls::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        systemCalls *_t = static_cast<systemCalls *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showProgressInqDebug(); break;
        case 1: { QStringList _r = _t->checkUpdates((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->upgradeConcurrent(); break;
        case 3: { int _r = _t->getSize();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject systemCalls::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_systemCalls.data,
      qt_meta_data_systemCalls,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *systemCalls::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *systemCalls::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_systemCalls.stringdata0))
        return static_cast<void*>(const_cast< systemCalls*>(this));
    return QObject::qt_metacast(_clname);
}

int systemCalls::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
