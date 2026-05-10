/****************************************************************************
** Meta object code from reading C++ file 'GenealogyManager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/GenealogyManager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GenealogyManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.3. It"
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
struct qt_meta_tag_ZN16GenealogyManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto GenealogyManager::qt_create_metaobjectdata<qt_meta_tag_ZN16GenealogyManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GenealogyManager",
        "genealogySelected",
        "",
        "genealogyId",
        "genealogyCreated",
        "genealogyDeleted",
        "onCreateGenealogy",
        "onEditGenealogy",
        "onDeleteGenealogy",
        "onInviteUser",
        "onGenealogyTableClicked",
        "QModelIndex",
        "index",
        "onGenealogyTableDoubleClicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'genealogySelected'
        QtMocHelpers::SignalData<void(int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'genealogyCreated'
        QtMocHelpers::SignalData<void(int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'genealogyDeleted'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'onCreateGenealogy'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onEditGenealogy'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDeleteGenealogy'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onInviteUser'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onGenealogyTableClicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Slot 'onGenealogyTableDoubleClicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(13, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GenealogyManager, qt_meta_tag_ZN16GenealogyManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GenealogyManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16GenealogyManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16GenealogyManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16GenealogyManagerE_t>.metaTypes,
    nullptr
} };

void GenealogyManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GenealogyManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->genealogySelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->genealogyCreated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->genealogyDeleted((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->onCreateGenealogy(); break;
        case 4: _t->onEditGenealogy(); break;
        case 5: _t->onDeleteGenealogy(); break;
        case 6: _t->onInviteUser(); break;
        case 7: _t->onGenealogyTableClicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 8: _t->onGenealogyTableDoubleClicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (GenealogyManager::*)(int )>(_a, &GenealogyManager::genealogySelected, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (GenealogyManager::*)(int )>(_a, &GenealogyManager::genealogyCreated, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (GenealogyManager::*)(int )>(_a, &GenealogyManager::genealogyDeleted, 2))
            return;
    }
}

const QMetaObject *GenealogyManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GenealogyManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16GenealogyManagerE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int GenealogyManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void GenealogyManager::genealogySelected(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void GenealogyManager::genealogyCreated(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void GenealogyManager::genealogyDeleted(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP
