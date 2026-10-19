/****************************************************************************
** Meta object code from reading C++ file 'page_device_status.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../page_device_status.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'page_device_status.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PageDeviceStatus_t {
    QByteArrayData data[15];
    char stringdata0[161];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PageDeviceStatus_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PageDeviceStatus_t qt_meta_stringdata_PageDeviceStatus = {
    {
QT_MOC_LITERAL(0, 0, 16), // "PageDeviceStatus"
QT_MOC_LITERAL(1, 17, 19), // "connectPlcRequested"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 2), // "ip"
QT_MOC_LITERAL(4, 41, 4), // "port"
QT_MOC_LITERAL(5, 46, 17), // "closePlcRequested"
QT_MOC_LITERAL(6, 64, 15), // "setPlcConnected"
QT_MOC_LITERAL(7, 80, 9), // "connected"
QT_MOC_LITERAL(8, 90, 13), // "updateFromPlc"
QT_MOC_LITERAL(9, 104, 3), // "cmd"
QT_MOC_LITERAL(10, 108, 6), // "status"
QT_MOC_LITERAL(11, 115, 7), // "payload"
QT_MOC_LITERAL(12, 123, 16), // "onBtnOpenclicked"
QT_MOC_LITERAL(13, 140, 9), // "onTimeout"
QT_MOC_LITERAL(14, 150, 10) // "onReadData"

    },
    "PageDeviceStatus\0connectPlcRequested\0"
    "\0ip\0port\0closePlcRequested\0setPlcConnected\0"
    "connected\0updateFromPlc\0cmd\0status\0"
    "payload\0onBtnOpenclicked\0onTimeout\0"
    "onReadData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PageDeviceStatus[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,
       5,    0,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   55,    2, 0x0a /* Public */,
       8,    3,   58,    2, 0x0a /* Public */,
      12,    0,   65,    2, 0x0a /* Public */,
      13,    0,   66,    2, 0x0a /* Public */,
      14,    0,   67,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::UShort,    3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar, QMetaType::QByteArray,    9,   10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PageDeviceStatus::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PageDeviceStatus *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectPlcRequested((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 1: _t->closePlcRequested(); break;
        case 2: _t->setPlcConnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->updateFromPlc((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 4: _t->onBtnOpenclicked(); break;
        case 5: _t->onTimeout(); break;
        case 6: _t->onReadData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PageDeviceStatus::*)(const QString & , quint16 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PageDeviceStatus::connectPlcRequested)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PageDeviceStatus::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PageDeviceStatus::closePlcRequested)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PageDeviceStatus::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_PageDeviceStatus.data,
    qt_meta_data_PageDeviceStatus,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PageDeviceStatus::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PageDeviceStatus::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PageDeviceStatus.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PageDeviceStatus::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void PageDeviceStatus::connectPlcRequested(const QString & _t1, quint16 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PageDeviceStatus::closePlcRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
