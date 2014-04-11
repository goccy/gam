/****************************************************************************
** Meta object code from reading C++ file 'physics.hpp'
**
** Created: Fri Nov 18 11:42:39 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "physics.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'physics.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PhysicsWorld[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   14,   13,   13, 0x0a,
      61,   14,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PhysicsWorld[] = {
    "PhysicsWorld\0\0o1,o2\0"
    "beginContactEvent(GamObject*,GamObject*)\0"
    "endContactEvent(GamObject*,GamObject*)\0"
};

const QMetaObject PhysicsWorld::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PhysicsWorld,
      qt_meta_data_PhysicsWorld, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PhysicsWorld::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PhysicsWorld::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PhysicsWorld::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PhysicsWorld))
        return static_cast<void*>(const_cast< PhysicsWorld*>(this));
    return QObject::qt_metacast(_clname);
}

int PhysicsWorld::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: beginContactEvent((*reinterpret_cast< GamObject*(*)>(_a[1])),(*reinterpret_cast< GamObject*(*)>(_a[2]))); break;
        case 1: endContactEvent((*reinterpret_cast< GamObject*(*)>(_a[1])),(*reinterpret_cast< GamObject*(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
