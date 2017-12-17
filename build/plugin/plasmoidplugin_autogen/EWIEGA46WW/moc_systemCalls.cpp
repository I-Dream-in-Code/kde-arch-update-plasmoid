/****************************************************************************
** Meta object code from reading C++ file 'systemCalls.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../plugin/systemCalls.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'systemCalls.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_systemCalls_t {
    QByteArrayData data[10];
    char stringdata0[122];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_systemCalls_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_systemCalls_t qt_meta_stringdata_systemCalls = {
    {
QT_MOC_LITERAL(0, 0, 11), // "systemCalls"
QT_MOC_LITERAL(1, 12, 18), // "checkUpdatesSignal"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 9), // "namesOnly"
QT_MOC_LITERAL(4, 42, 3), // "aur"
QT_MOC_LITERAL(5, 46, 19), // "upgradeSystemSignal"
QT_MOC_LITERAL(6, 66, 11), // "konsoleFlag"
QT_MOC_LITERAL(7, 78, 12), // "checkUpdates"
QT_MOC_LITERAL(8, 91, 13), // "upgradeSystem"
QT_MOC_LITERAL(9, 105, 16) // "readCheckUpdates"

    },
    "systemCalls\0checkUpdatesSignal\0\0"
    "namesOnly\0aur\0upgradeSystemSignal\0"
    "konsoleFlag\0checkUpdates\0upgradeSystem\0"
    "readCheckUpdates"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_systemCalls[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,
       5,    2,   44,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       7,    2,   49,    2, 0x02 /* Public */,
       8,    2,   54,    2, 0x02 /* Public */,
       9,    0,   59,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,    3,    4,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,    6,    4,

 // methods: parameters
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,    3,    4,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,    6,    4,
    QMetaType::QStringList,

       0        // eod
};

void systemCalls::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        systemCalls *_t = static_cast<systemCalls *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->checkUpdatesSignal((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->upgradeSystemSignal((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->checkUpdates((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->upgradeSystem((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: { QStringList _r = _t->readCheckUpdates();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (systemCalls::*_t)(bool , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&systemCalls::checkUpdatesSignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (systemCalls::*_t)(bool , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&systemCalls::upgradeSystemSignal)) {
                *result = 1;
                return;
            }
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
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int systemCalls::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void systemCalls::checkUpdatesSignal(bool _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void systemCalls::upgradeSystemSignal(bool _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
