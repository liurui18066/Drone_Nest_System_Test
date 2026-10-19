/****************************************************************************
** Meta object code from reading C++ file 'page_debug_control.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../page_debug_control.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'page_debug_control.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PageDebugControl_t {
    QByteArrayData data[14];
    char stringdata0[206];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PageDebugControl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PageDebugControl_t qt_meta_stringdata_PageDebugControl = {
    {
QT_MOC_LITERAL(0, 0, 16), // "PageDebugControl"
QT_MOC_LITERAL(1, 17, 14), // "onResetClicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 17), // "onOpenDoorClicked"
QT_MOC_LITERAL(4, 51, 18), // "onCloseDoorClicked"
QT_MOC_LITERAL(5, 70, 19), // "onOpenGripFRClicked"
QT_MOC_LITERAL(6, 90, 20), // "onCloseGripFRClicked"
QT_MOC_LITERAL(7, 111, 19), // "onOpenGripCHClicked"
QT_MOC_LITERAL(8, 131, 20), // "onCloseGripCHClicked"
QT_MOC_LITERAL(9, 152, 16), // "onCommandTimeout"
QT_MOC_LITERAL(10, 169, 15), // "onCommandResult"
QT_MOC_LITERAL(11, 185, 3), // "cmd"
QT_MOC_LITERAL(12, 189, 6), // "status"
QT_MOC_LITERAL(13, 196, 9) // "errorCode"

    },
    "PageDebugControl\0onResetClicked\0\0"
    "onOpenDoorClicked\0onCloseDoorClicked\0"
    "onOpenGripFRClicked\0onCloseGripFRClicked\0"
    "onOpenGripCHClicked\0onCloseGripCHClicked\0"
    "onCommandTimeout\0onCommandResult\0cmd\0"
    "status\0errorCode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PageDebugControl[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    0,   66,    2, 0x08 /* Private */,
      10,    3,   67,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar, QMetaType::UShort,   11,   12,   13,

       0        // eod
};

void PageDebugControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PageDebugControl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onResetClicked(); break;
        case 1: _t->onOpenDoorClicked(); break;
        case 2: _t->onCloseDoorClicked(); break;
        case 3: _t->onOpenGripFRClicked(); break;
        case 4: _t->onCloseGripFRClicked(); break;
        case 5: _t->onOpenGripCHClicked(); break;
        case 6: _t->onCloseGripCHClicked(); break;
        case 7: _t->onCommandTimeout(); break;
        case 8: _t->onCommandResult((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PageDebugControl::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_PageDebugControl.data,
    qt_meta_data_PageDebugControl,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PageDebugControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PageDebugControl::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PageDebugControl.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PageDebugControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
