/****************************************************************************
** Meta object code from reading C++ file 'server.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "server.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'server.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSServerENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSServerENDCLASS = QtMocHelpers::stringData(
    "Server",
    "onNewImageClientConnection",
    "",
    "onNewImageAdminConnection",
    "onNewInfoClientConnection",
    "onNewInfoAdminConnection",
    "onClientImageReceived",
    "onClientInfoReceived",
    "onClientDisconnected",
    "onAdminDisconnected"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSServerENDCLASS_t {
    uint offsetsAndSizes[20];
    char stringdata0[7];
    char stringdata1[27];
    char stringdata2[1];
    char stringdata3[26];
    char stringdata4[26];
    char stringdata5[25];
    char stringdata6[22];
    char stringdata7[21];
    char stringdata8[21];
    char stringdata9[20];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSServerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSServerENDCLASS_t qt_meta_stringdata_CLASSServerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 6),  // "Server"
        QT_MOC_LITERAL(7, 26),  // "onNewImageClientConnection"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 25),  // "onNewImageAdminConnection"
        QT_MOC_LITERAL(61, 25),  // "onNewInfoClientConnection"
        QT_MOC_LITERAL(87, 24),  // "onNewInfoAdminConnection"
        QT_MOC_LITERAL(112, 21),  // "onClientImageReceived"
        QT_MOC_LITERAL(134, 20),  // "onClientInfoReceived"
        QT_MOC_LITERAL(155, 20),  // "onClientDisconnected"
        QT_MOC_LITERAL(176, 19)   // "onAdminDisconnected"
    },
    "Server",
    "onNewImageClientConnection",
    "",
    "onNewImageAdminConnection",
    "onNewInfoClientConnection",
    "onNewInfoAdminConnection",
    "onClientImageReceived",
    "onClientInfoReceived",
    "onClientDisconnected",
    "onAdminDisconnected"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSServerENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x08,    1 /* Private */,
       3,    0,   63,    2, 0x08,    2 /* Private */,
       4,    0,   64,    2, 0x08,    3 /* Private */,
       5,    0,   65,    2, 0x08,    4 /* Private */,
       6,    0,   66,    2, 0x08,    5 /* Private */,
       7,    0,   67,    2, 0x08,    6 /* Private */,
       8,    0,   68,    2, 0x08,    7 /* Private */,
       9,    0,   69,    2, 0x08,    8 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject Server::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSServerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSServerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSServerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Server, std::true_type>,
        // method 'onNewImageClientConnection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNewImageAdminConnection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNewInfoClientConnection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNewInfoAdminConnection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClientImageReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClientInfoReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClientDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onAdminDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Server::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Server *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onNewImageClientConnection(); break;
        case 1: _t->onNewImageAdminConnection(); break;
        case 2: _t->onNewInfoClientConnection(); break;
        case 3: _t->onNewInfoAdminConnection(); break;
        case 4: _t->onClientImageReceived(); break;
        case 5: _t->onClientInfoReceived(); break;
        case 6: _t->onClientDisconnected(); break;
        case 7: _t->onAdminDisconnected(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *Server::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Server::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSServerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Server::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
