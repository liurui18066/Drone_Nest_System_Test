/****************************************************************************
** Meta object code from reading C++ file 'communication_manager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../communication_manager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'communication_manager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CommunicationManager_t {
    QByteArrayData data[25];
    char stringdata0[307];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CommunicationManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CommunicationManager_t qt_meta_stringdata_CommunicationManager = {
    {
QT_MOC_LITERAL(0, 0, 20), // "CommunicationManager"
QT_MOC_LITERAL(1, 21, 20), // "plcConnectionChanged"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 9), // "connected"
QT_MOC_LITERAL(4, 53, 15), // "plcDataReceived"
QT_MOC_LITERAL(5, 69, 3), // "cmd"
QT_MOC_LITERAL(6, 73, 6), // "status"
QT_MOC_LITERAL(7, 80, 7), // "payload"
QT_MOC_LITERAL(8, 88, 16), // "plcCommandResult"
QT_MOC_LITERAL(9, 105, 9), // "errorCode"
QT_MOC_LITERAL(10, 115, 10), // "logMessage"
QT_MOC_LITERAL(11, 126, 3), // "msg"
QT_MOC_LITERAL(12, 130, 15), // "sendDoorControl"
QT_MOC_LITERAL(13, 146, 13), // "operationCode"
QT_MOC_LITERAL(14, 160, 17), // "sendGripFRControl"
QT_MOC_LITERAL(15, 178, 17), // "sendGripCHControl"
QT_MOC_LITERAL(16, 196, 14), // "sendPlcControl"
QT_MOC_LITERAL(17, 211, 11), // "controlItem"
QT_MOC_LITERAL(18, 223, 17), // "sendNetworkStatus"
QT_MOC_LITERAL(19, 241, 10), // "mqttStatus"
QT_MOC_LITERAL(20, 252, 12), // "reconnectPlc"
QT_MOC_LITERAL(21, 265, 4), // "host"
QT_MOC_LITERAL(22, 270, 4), // "port"
QT_MOC_LITERAL(23, 275, 13), // "disconnectPlc"
QT_MOC_LITERAL(24, 289, 17) // "onPlcDataReceived"

    },
    "CommunicationManager\0plcConnectionChanged\0"
    "\0connected\0plcDataReceived\0cmd\0status\0"
    "payload\0plcCommandResult\0errorCode\0"
    "logMessage\0msg\0sendDoorControl\0"
    "operationCode\0sendGripFRControl\0"
    "sendGripCHControl\0sendPlcControl\0"
    "controlItem\0sendNetworkStatus\0mqttStatus\0"
    "reconnectPlc\0host\0port\0disconnectPlc\0"
    "onPlcDataReceived"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CommunicationManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    3,   77,    2, 0x06 /* Public */,
       8,    3,   84,    2, 0x06 /* Public */,
      10,    1,   91,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    1,   94,    2, 0x0a /* Public */,
      14,    1,   97,    2, 0x0a /* Public */,
      15,    1,  100,    2, 0x0a /* Public */,
      16,    2,  103,    2, 0x0a /* Public */,
      18,    1,  108,    2, 0x0a /* Public */,
      20,    2,  111,    2, 0x0a /* Public */,
      23,    0,  116,    2, 0x0a /* Public */,
      24,    3,  117,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar, QMetaType::QByteArray,    5,    6,    7,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar, QMetaType::UShort,    5,    6,    9,
    QMetaType::Void, QMetaType::QString,   11,

 // slots: parameters
    QMetaType::Void, QMetaType::UChar,   13,
    QMetaType::Void, QMetaType::UChar,   13,
    QMetaType::Void, QMetaType::UChar,   13,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar,   17,   13,
    QMetaType::Void, QMetaType::UChar,   19,
    QMetaType::Void, QMetaType::QString, QMetaType::UShort,   21,   22,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar, QMetaType::QByteArray,    5,    6,    7,

       0        // eod
};

void CommunicationManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CommunicationManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->plcConnectionChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->plcDataReceived((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 2: _t->plcCommandResult((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 3: _t->logMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->sendDoorControl((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 5: _t->sendGripFRControl((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 6: _t->sendGripCHControl((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 7: _t->sendPlcControl((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2]))); break;
        case 8: _t->sendNetworkStatus((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 9: _t->reconnectPlc((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 10: _t->disconnectPlc(); break;
        case 11: _t->onPlcDataReceived((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CommunicationManager::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommunicationManager::plcConnectionChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CommunicationManager::*)(quint8 , quint8 , const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommunicationManager::plcDataReceived)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CommunicationManager::*)(quint8 , quint8 , quint16 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommunicationManager::plcCommandResult)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CommunicationManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommunicationManager::logMessage)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CommunicationManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CommunicationManager.data,
    qt_meta_data_CommunicationManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CommunicationManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommunicationManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CommunicationManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CommunicationManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void CommunicationManager::plcConnectionChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CommunicationManager::plcDataReceived(quint8 _t1, quint8 _t2, const QByteArray & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CommunicationManager::plcCommandResult(quint8 _t1, quint8 _t2, quint16 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CommunicationManager::logMessage(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
