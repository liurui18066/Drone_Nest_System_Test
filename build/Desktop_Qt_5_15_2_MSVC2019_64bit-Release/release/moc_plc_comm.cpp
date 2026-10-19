/****************************************************************************
** Meta object code from reading C++ file 'plc_comm.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../plc_comm.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plc_comm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PlcComm_t {
    QByteArrayData data[28];
    char stringdata0[365];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PlcComm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PlcComm_t qt_meta_stringdata_PlcComm = {
    {
QT_MOC_LITERAL(0, 0, 7), // "PlcComm"
QT_MOC_LITERAL(1, 8, 22), // "connectionStateChanged"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 9), // "connected"
QT_MOC_LITERAL(4, 42, 12), // "dataReceived"
QT_MOC_LITERAL(5, 55, 3), // "cmd"
QT_MOC_LITERAL(6, 59, 6), // "status"
QT_MOC_LITERAL(7, 66, 7), // "payload"
QT_MOC_LITERAL(8, 74, 13), // "errorOccurred"
QT_MOC_LITERAL(9, 88, 8), // "errorStr"
QT_MOC_LITERAL(10, 97, 10), // "logMessage"
QT_MOC_LITERAL(11, 108, 3), // "msg"
QT_MOC_LITERAL(12, 112, 15), // "sendDoorControl"
QT_MOC_LITERAL(13, 128, 13), // "operationCode"
QT_MOC_LITERAL(14, 142, 17), // "sendGripFRControl"
QT_MOC_LITERAL(15, 160, 17), // "sendGripCHControl"
QT_MOC_LITERAL(16, 178, 14), // "sendPlcControl"
QT_MOC_LITERAL(17, 193, 11), // "controlItem"
QT_MOC_LITERAL(18, 205, 17), // "sendNetworkStatus"
QT_MOC_LITERAL(19, 223, 17), // "disconnectFromPlc"
QT_MOC_LITERAL(20, 241, 20), // "onModbusStateChanged"
QT_MOC_LITERAL(21, 262, 20), // "QModbusDevice::State"
QT_MOC_LITERAL(22, 283, 5), // "state"
QT_MOC_LITERAL(23, 289, 15), // "onErrorOccurred"
QT_MOC_LITERAL(24, 305, 20), // "QModbusDevice::Error"
QT_MOC_LITERAL(25, 326, 5), // "error"
QT_MOC_LITERAL(26, 332, 12), // "periodicRead"
QT_MOC_LITERAL(27, 345, 19) // "attemptReconnection"

    },
    "PlcComm\0connectionStateChanged\0\0"
    "connected\0dataReceived\0cmd\0status\0"
    "payload\0errorOccurred\0errorStr\0"
    "logMessage\0msg\0sendDoorControl\0"
    "operationCode\0sendGripFRControl\0"
    "sendGripCHControl\0sendPlcControl\0"
    "controlItem\0sendNetworkStatus\0"
    "disconnectFromPlc\0onModbusStateChanged\0"
    "QModbusDevice::State\0state\0onErrorOccurred\0"
    "QModbusDevice::Error\0error\0periodicRead\0"
    "attemptReconnection"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlcComm[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    3,   87,    2, 0x06 /* Public */,
       8,    1,   94,    2, 0x06 /* Public */,
      10,    1,   97,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    1,  100,    2, 0x0a /* Public */,
      14,    1,  103,    2, 0x0a /* Public */,
      15,    1,  106,    2, 0x0a /* Public */,
      16,    2,  109,    2, 0x0a /* Public */,
      18,    2,  114,    2, 0x0a /* Public */,
      19,    0,  119,    2, 0x0a /* Public */,
      20,    1,  120,    2, 0x08 /* Private */,
      23,    1,  123,    2, 0x08 /* Private */,
      26,    0,  126,    2, 0x08 /* Private */,
      27,    0,  127,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar, QMetaType::QByteArray,    5,    6,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,   11,

 // slots: parameters
    QMetaType::Void, QMetaType::UChar,   13,
    QMetaType::Void, QMetaType::UChar,   13,
    QMetaType::Void, QMetaType::UChar,   13,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar,   17,   13,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar,   17,   13,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, 0x80000000 | 24,   25,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PlcComm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PlcComm *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectionStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->dataReceived((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 2: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->logMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->sendDoorControl((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 5: _t->sendGripFRControl((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 6: _t->sendGripCHControl((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 7: _t->sendPlcControl((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2]))); break;
        case 8: _t->sendNetworkStatus((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2]))); break;
        case 9: _t->disconnectFromPlc(); break;
        case 10: _t->onModbusStateChanged((*reinterpret_cast< QModbusDevice::State(*)>(_a[1]))); break;
        case 11: _t->onErrorOccurred((*reinterpret_cast< QModbusDevice::Error(*)>(_a[1]))); break;
        case 12: _t->periodicRead(); break;
        case 13: _t->attemptReconnection(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PlcComm::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PlcComm::connectionStateChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PlcComm::*)(quint8 , quint8 , const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PlcComm::dataReceived)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PlcComm::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PlcComm::errorOccurred)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (PlcComm::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PlcComm::logMessage)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PlcComm::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_PlcComm.data,
    qt_meta_data_PlcComm,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PlcComm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlcComm::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PlcComm.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PlcComm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void PlcComm::connectionStateChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PlcComm::dataReceived(quint8 _t1, quint8 _t2, const QByteArray & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PlcComm::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void PlcComm::logMessage(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
