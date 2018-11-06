/****************************************************************************
** Meta object code from reading C++ file 'ctitlebar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ctitlebar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ctitlebar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CTitleBar_t {
    QByteArrayData data[5];
    char stringdata0[73];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CTitleBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CTitleBar_t qt_meta_stringdata_CTitleBar = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CTitleBar"
QT_MOC_LITERAL(1, 10, 21), // "sigOnMinimizeBtnClick"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 21), // "sigOnMaximizeBtnClick"
QT_MOC_LITERAL(4, 55, 17) // "sigOnExitBtnClick"

    },
    "CTitleBar\0sigOnMinimizeBtnClick\0\0"
    "sigOnMaximizeBtnClick\0sigOnExitBtnClick"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CTitleBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    0,   30,    2, 0x06 /* Public */,
       4,    0,   31,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CTitleBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CTitleBar *_t = static_cast<CTitleBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigOnMinimizeBtnClick(); break;
        case 1: _t->sigOnMaximizeBtnClick(); break;
        case 2: _t->sigOnExitBtnClick(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (CTitleBar::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CTitleBar::sigOnMinimizeBtnClick)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CTitleBar::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CTitleBar::sigOnMaximizeBtnClick)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CTitleBar::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CTitleBar::sigOnExitBtnClick)) {
                *result = 2;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CTitleBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CTitleBar.data,
      qt_meta_data_CTitleBar,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CTitleBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CTitleBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CTitleBar.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CTitleBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CTitleBar::sigOnMinimizeBtnClick()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CTitleBar::sigOnMaximizeBtnClick()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CTitleBar::sigOnExitBtnClick()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
