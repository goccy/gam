/****************************************************************************
** Meta object code from reading C++ file 'gam.hpp'
**
** Created: Thu Sep 29 18:44:55 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/gam.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gam.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GamContact[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   12,   11,   11, 0x05,
      64,   12,   11,   11, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_GamContact[] = {
    "GamContact\0\0o1,o2\0"
    "emitBeginContactSignal(GamObject*,GamObject*)\0"
    "emitEndContactSignal(GamObject*,GamObject*)\0"
};

const QMetaObject GamContact::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GamContact,
      qt_meta_data_GamContact, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GamContact::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GamContact::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GamContact::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GamContact))
        return static_cast<void*>(const_cast< GamContact*>(this));
    if (!strcmp(_clname, "b2ContactListener"))
        return static_cast< b2ContactListener*>(const_cast< GamContact*>(this));
    return QObject::qt_metacast(_clname);
}

int GamContact::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: emitBeginContactSignal((*reinterpret_cast< GamObject*(*)>(_a[1])),(*reinterpret_cast< GamObject*(*)>(_a[2]))); break;
        case 1: emitEndContactSignal((*reinterpret_cast< GamObject*(*)>(_a[1])),(*reinterpret_cast< GamObject*(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void GamContact::emitBeginContactSignal(GamObject * _t1, GamObject * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GamContact::emitEndContactSignal(GamObject * _t1, GamObject * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_GamWorld[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   10,    9,    9, 0x05,
      52,   10,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      86,   10,    9,    9, 0x0a,
     126,   10,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GamWorld[] = {
    "GamWorld\0\0o1,o2\0beginContact(GamObject*,GamObject*)\0"
    "endContact(GamObject*,GamObject*)\0"
    "beginContactSlot(GamObject*,GamObject*)\0"
    "endContactSlot(GamObject*,GamObject*)\0"
};

const QMetaObject GamWorld::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GamWorld,
      qt_meta_data_GamWorld, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GamWorld::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GamWorld::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GamWorld::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GamWorld))
        return static_cast<void*>(const_cast< GamWorld*>(this));
    if (!strcmp(_clname, "GamObject"))
        return static_cast< GamObject*>(const_cast< GamWorld*>(this));
    return QObject::qt_metacast(_clname);
}

int GamWorld::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: beginContact((*reinterpret_cast< GamObject*(*)>(_a[1])),(*reinterpret_cast< GamObject*(*)>(_a[2]))); break;
        case 1: endContact((*reinterpret_cast< GamObject*(*)>(_a[1])),(*reinterpret_cast< GamObject*(*)>(_a[2]))); break;
        case 2: beginContactSlot((*reinterpret_cast< GamObject*(*)>(_a[1])),(*reinterpret_cast< GamObject*(*)>(_a[2]))); break;
        case 3: endContactSlot((*reinterpret_cast< GamObject*(*)>(_a[1])),(*reinterpret_cast< GamObject*(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void GamWorld::beginContact(GamObject * _t1, GamObject * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GamWorld::endContact(GamObject * _t1, GamObject * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_GamRect[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_GamRect[] = {
    "GamRect\0"
};

const QMetaObject GamRect::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GamRect,
      qt_meta_data_GamRect, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GamRect::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GamRect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GamRect::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GamRect))
        return static_cast<void*>(const_cast< GamRect*>(this));
    if (!strcmp(_clname, "GamObject"))
        return static_cast< GamObject*>(const_cast< GamRect*>(this));
    if (!strcmp(_clname, "QGraphicsRectItem"))
        return static_cast< QGraphicsRectItem*>(const_cast< GamRect*>(this));
    if (!strcmp(_clname, "GamRigidBody"))
        return static_cast< GamRigidBody*>(const_cast< GamRect*>(this));
    return QObject::qt_metacast(_clname);
}

int GamRect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_GamEllipse[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_GamEllipse[] = {
    "GamEllipse\0"
};

const QMetaObject GamEllipse::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GamEllipse,
      qt_meta_data_GamEllipse, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GamEllipse::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GamEllipse::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GamEllipse::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GamEllipse))
        return static_cast<void*>(const_cast< GamEllipse*>(this));
    if (!strcmp(_clname, "GamObject"))
        return static_cast< GamObject*>(const_cast< GamEllipse*>(this));
    if (!strcmp(_clname, "QGraphicsEllipseItem"))
        return static_cast< QGraphicsEllipseItem*>(const_cast< GamEllipse*>(this));
    if (!strcmp(_clname, "GamRigidBody"))
        return static_cast< GamRigidBody*>(const_cast< GamEllipse*>(this));
    return QObject::qt_metacast(_clname);
}

int GamEllipse::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_GamTexture[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_GamTexture[] = {
    "GamTexture\0"
};

const QMetaObject GamTexture::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GamTexture,
      qt_meta_data_GamTexture, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GamTexture::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GamTexture::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GamTexture::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GamTexture))
        return static_cast<void*>(const_cast< GamTexture*>(this));
    if (!strcmp(_clname, "GamObject"))
        return static_cast< GamObject*>(const_cast< GamTexture*>(this));
    if (!strcmp(_clname, "QGraphicsPixmapItem"))
        return static_cast< QGraphicsPixmapItem*>(const_cast< GamTexture*>(this));
    if (!strcmp(_clname, "GamRigidBody"))
        return static_cast< GamRigidBody*>(const_cast< GamTexture*>(this));
    return QObject::qt_metacast(_clname);
}

int GamTexture::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_GamLine[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_GamLine[] = {
    "GamLine\0"
};

const QMetaObject GamLine::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GamLine,
      qt_meta_data_GamLine, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GamLine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GamLine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GamLine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GamLine))
        return static_cast<void*>(const_cast< GamLine*>(this));
    if (!strcmp(_clname, "GamObject"))
        return static_cast< GamObject*>(const_cast< GamLine*>(this));
    if (!strcmp(_clname, "QGraphicsLineItem"))
        return static_cast< QGraphicsLineItem*>(const_cast< GamLine*>(this));
    if (!strcmp(_clname, "GamRigidBody"))
        return static_cast< GamRigidBody*>(const_cast< GamLine*>(this));
    return QObject::qt_metacast(_clname);
}

int GamLine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_GamText[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_GamText[] = {
    "GamText\0"
};

const QMetaObject GamText::staticMetaObject = {
    { &QGraphicsTextItem::staticMetaObject, qt_meta_stringdata_GamText,
      qt_meta_data_GamText, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GamText::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GamText::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GamText::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GamText))
        return static_cast<void*>(const_cast< GamText*>(this));
    if (!strcmp(_clname, "GamObject"))
        return static_cast< GamObject*>(const_cast< GamText*>(this));
    if (!strcmp(_clname, "GamRigidBody"))
        return static_cast< GamRigidBody*>(const_cast< GamText*>(this));
    return QGraphicsTextItem::qt_metacast(_clname);
}

int GamText::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsTextItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_GamComplexItem[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_GamComplexItem[] = {
    "GamComplexItem\0"
};

const QMetaObject GamComplexItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GamComplexItem,
      qt_meta_data_GamComplexItem, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GamComplexItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GamComplexItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GamComplexItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GamComplexItem))
        return static_cast<void*>(const_cast< GamComplexItem*>(this));
    if (!strcmp(_clname, "GamObject"))
        return static_cast< GamObject*>(const_cast< GamComplexItem*>(this));
    if (!strcmp(_clname, "QGraphicsItemGroup"))
        return static_cast< QGraphicsItemGroup*>(const_cast< GamComplexItem*>(this));
    if (!strcmp(_clname, "GamRigidBody"))
        return static_cast< GamRigidBody*>(const_cast< GamComplexItem*>(this));
    return QObject::qt_metacast(_clname);
}

int GamComplexItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
