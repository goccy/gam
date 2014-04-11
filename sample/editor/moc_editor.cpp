/****************************************************************************
** Meta object code from reading C++ file 'editor.hpp'
**
** Created: Thu Oct 13 18:26:51 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "editor.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'editor.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ColorDialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_ColorDialog[] = {
    "ColorDialog\0\0closedDialogSignal()\0"
};

const QMetaObject ColorDialog::staticMetaObject = {
    { &QColorDialog::staticMetaObject, qt_meta_stringdata_ColorDialog,
      qt_meta_data_ColorDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ColorDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ColorDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ColorDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ColorDialog))
        return static_cast<void*>(const_cast< ColorDialog*>(this));
    return QColorDialog::qt_metacast(_clname);
}

int ColorDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QColorDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: closedDialogSignal(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ColorDialog::closedDialogSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_Menu[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,    6,    5,    5, 0x05,
      40,    6,    5,    5, 0x05,

 // slots: signature, parameters, type, tag, flags
      72,   67,    5,    5, 0x0a,
     106,   67,    5,    5, 0x0a,
     146,   67,    5,    5, 0x0a,
     180,    6,    5,    5, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Menu[] = {
    "Menu\0\0color\0colorSelectedSignal(QColor)\0"
    "colorChangedSignal(QColor)\0item\0"
    "itemEnteredSlot(QListWidgetItem*)\0"
    "itemDoubleClickedSlot(QListWidgetItem*)\0"
    "itemClickedSlot(QListWidgetItem*)\0"
    "colorSelectedSlot(QColor)\0"
};

const QMetaObject Menu::staticMetaObject = {
    { &QListWidget::staticMetaObject, qt_meta_stringdata_Menu,
      qt_meta_data_Menu, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Menu::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Menu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Menu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Menu))
        return static_cast<void*>(const_cast< Menu*>(this));
    return QListWidget::qt_metacast(_clname);
}

int Menu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QListWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: colorSelectedSignal((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 1: colorChangedSignal((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 2: itemEnteredSlot((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 3: itemDoubleClickedSlot((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 4: itemClickedSlot((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 5: colorSelectedSlot((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Menu::colorSelectedSignal(QColor _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Menu::colorChangedSignal(QColor _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_RectObject[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_RectObject[] = {
    "RectObject\0\0type\0selectedSignal(int)\0"
};

const QMetaObject RectObject::staticMetaObject = {
    { &GamRect::staticMetaObject, qt_meta_stringdata_RectObject,
      qt_meta_data_RectObject, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RectObject::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RectObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RectObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RectObject))
        return static_cast<void*>(const_cast< RectObject*>(this));
    return GamRect::qt_metacast(_clname);
}

int RectObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GamRect::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectedSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void RectObject::selectedSignal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_ResizeEllipseObject[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      32,   21,   20,   20, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_ResizeEllipseObject[] = {
    "ResizeEllipseObject\0\0type,event\0"
    "resizeSignal(CornerType,QGraphicsSceneMouseEvent*)\0"
};

const QMetaObject ResizeEllipseObject::staticMetaObject = {
    { &GamEllipse::staticMetaObject, qt_meta_stringdata_ResizeEllipseObject,
      qt_meta_data_ResizeEllipseObject, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ResizeEllipseObject::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ResizeEllipseObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ResizeEllipseObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ResizeEllipseObject))
        return static_cast<void*>(const_cast< ResizeEllipseObject*>(this));
    return GamEllipse::qt_metacast(_clname);
}

int ResizeEllipseObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GamEllipse::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: resizeSignal((*reinterpret_cast< CornerType(*)>(_a[1])),(*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ResizeEllipseObject::resizeSignal(CornerType _t1, QGraphicsSceneMouseEvent * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_EditableRectObject[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   20,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      76,   65,   19,   19, 0x0a,
     137,  131,   19,   19, 0x0a,
     169,  131,   19,   19, 0x0a,
     201,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EditableRectObject[] = {
    "EditableRectObject\0\0,type\0"
    "selectedEditableSignal(GamObject*,int)\0"
    "type,event\0"
    "resizeSignalSlot(CornerType,QGraphicsSceneMouseEvent*)\0"
    "color\0colorSelectedSignalSlot(QColor)\0"
    "currentColorChangedSlot(QColor)\0"
    "closedDialogSignalSlot()\0"
};

const QMetaObject EditableRectObject::staticMetaObject = {
    { &GamRect::staticMetaObject, qt_meta_stringdata_EditableRectObject,
      qt_meta_data_EditableRectObject, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EditableRectObject::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EditableRectObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EditableRectObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EditableRectObject))
        return static_cast<void*>(const_cast< EditableRectObject*>(this));
    return GamRect::qt_metacast(_clname);
}

int EditableRectObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GamRect::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectedEditableSignal((*reinterpret_cast< GamObject*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: resizeSignalSlot((*reinterpret_cast< CornerType(*)>(_a[1])),(*reinterpret_cast< QGraphicsSceneMouseEvent*(*)>(_a[2]))); break;
        case 2: colorSelectedSignalSlot((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 3: currentColorChangedSlot((*reinterpret_cast< QColor(*)>(_a[1]))); break;
        case 4: closedDialogSignalSlot(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void EditableRectObject::selectedEditableSignal(GamObject * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_EditScene[] = {

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
      16,   11,   10,   10, 0x0a,
      46,   40,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_EditScene[] = {
    "EditScene\0\0type\0selectedSignalSlot(int)\0"
    ",type\0selectedEditableSignalSlot(GamObject*,int)\0"
};

const QMetaObject EditScene::staticMetaObject = {
    { &GamScene::staticMetaObject, qt_meta_stringdata_EditScene,
      qt_meta_data_EditScene, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EditScene::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EditScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EditScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EditScene))
        return static_cast<void*>(const_cast< EditScene*>(this));
    return GamScene::qt_metacast(_clname);
}

int EditScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GamScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectedSignalSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: selectedEditableSignalSlot((*reinterpret_cast< GamObject*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
