/****************************************************************************
** Meta object code from reading C++ file 'mqtt_comm.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../mqtt_comm.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mqtt_comm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MqttComm_t {
    QByteArrayData data[23];
    char stringdata0[292];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MqttComm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MqttComm_t qt_meta_stringdata_MqttComm = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MqttComm"
QT_MOC_LITERAL(1, 9, 22), // "connectionStateChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 9), // "connected"
QT_MOC_LITERAL(4, 43, 15), // "messageReceived"
QT_MOC_LITERAL(5, 59, 5), // "topic"
QT_MOC_LITERAL(6, 65, 7), // "payload"
QT_MOC_LITERAL(7, 73, 13), // "errorOccurred"
QT_MOC_LITERAL(8, 87, 8), // "errorStr"
QT_MOC_LITERAL(9, 96, 5), // "start"
QT_MOC_LITERAL(10, 102, 20), // "disconnectFromBroker"
QT_MOC_LITERAL(11, 123, 14), // "publishMessage"
QT_MOC_LITERAL(12, 138, 3), // "qos"
QT_MOC_LITERAL(13, 142, 14), // "subscribeTopic"
QT_MOC_LITERAL(14, 157, 11), // "onConnected"
QT_MOC_LITERAL(15, 169, 14), // "onDisconnected"
QT_MOC_LITERAL(16, 184, 17), // "onMessageReceived"
QT_MOC_LITERAL(17, 202, 7), // "message"
QT_MOC_LITERAL(18, 210, 14), // "QMqttTopicName"
QT_MOC_LITERAL(19, 225, 15), // "onErrorOccurred"
QT_MOC_LITERAL(20, 241, 24), // "QMqttClient::ClientError"
QT_MOC_LITERAL(21, 266, 5), // "error"
QT_MOC_LITERAL(22, 272, 19) // "attemptReconnection"

    },
    "MqttComm\0connectionStateChanged\0\0"
    "connected\0messageReceived\0topic\0payload\0"
    "errorOccurred\0errorStr\0start\0"
    "disconnectFromBroker\0publishMessage\0"
    "qos\0subscribeTopic\0onConnected\0"
    "onDisconnected\0onMessageReceived\0"
    "message\0QMqttTopicName\0onErrorOccurred\0"
    "QMqttClient::ClientError\0error\0"
    "attemptReconnection"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MqttComm[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    2,   87,    2, 0x06 /* Public */,
       7,    1,   92,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   95,    2, 0x0a /* Public */,
      10,    0,   96,    2, 0x0a /* Public */,
      11,    3,   97,    2, 0x0a /* Public */,
      11,    2,  104,    2, 0x2a /* Public | MethodCloned */,
      13,    2,  109,    2, 0x0a /* Public */,
      13,    1,  114,    2, 0x2a /* Public | MethodCloned */,
      14,    0,  117,    2, 0x08 /* Private */,
      15,    0,  118,    2, 0x08 /* Private */,
      16,    2,  119,    2, 0x08 /* Private */,
      19,    1,  124,    2, 0x08 /* Private */,
      22,    0,  127,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QByteArray,    5,    6,
    QMetaType::Void, QMetaType::QString,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString, QMetaType::QByteArray, QMetaType::UChar,    5,    6,   12,
    QMetaType::Bool, QMetaType::QString, QMetaType::QByteArray,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::UChar,    5,   12,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 18,   17,    5,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void,

       0        // eod
};

void MqttComm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MqttComm *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectionStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->messageReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 2: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->start(); break;
        case 4: _t->disconnectFromBroker(); break;
        case 5: { bool _r = _t->publishMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->publishMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->subscribeTopic((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2]))); break;
        case 8: _t->subscribeTopic((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->onConnected(); break;
        case 10: _t->onDisconnected(); break;
        case 11: _t->onMessageReceived((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QMqttTopicName(*)>(_a[2]))); break;
        case 12: _t->onErrorOccurred((*reinterpret_cast< QMqttClient::ClientError(*)>(_a[1]))); break;
        case 13: _t->attemptReconnection(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMqttTopicName >(); break;
            }
            break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMqttClient::ClientError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MqttComm::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MqttComm::connectionStateChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MqttComm::*)(const QString & , const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MqttComm::messageReceived)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MqttComm::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MqttComm::errorOccurred)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MqttComm::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MqttComm.data,
    qt_meta_data_MqttComm,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MqttComm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MqttComm::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MqttComm.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MqttComm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void MqttComm::connectionStateChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MqttComm::messageReceived(const QString & _t1, const QByteArray & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MqttComm::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
