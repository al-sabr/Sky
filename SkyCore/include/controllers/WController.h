//=================================================================================================
/*
    Copyright (C) 2015-2020 Sky kit authors. <http://omega.gg/Sky>

    Author: Benjamin Arnaud. <http://bunjee.me> <bunjee@omega.gg>

    This file is part of SkCore.

    - GNU Lesser General Public License Usage:
    This file may be used under the terms of the GNU Lesser General Public License version 3 as
    published by the Free Software Foundation and appearing in the LICENSE.md file included in the
    packaging of this file. Please review the following information to ensure the GNU Lesser
    General Public License requirements will be met: https://www.gnu.org/licenses/lgpl.html.

    - Private License Usage:
    Sky kit licensees holding valid private licenses may use this file in accordance with the
    private license agreement provided with the Software or, alternatively, in accordance with the
    terms contained in written agreement between you and Sky kit authors. For further information
    contact us at contact@omega.gg.
*/
//=================================================================================================

#ifndef WCONTROLLER_H
#define WCONTROLLER_H

// Qt includes
#include <QObject>

// Sk includes
#include <Sk>
#include <SkyCoreExports.h>

// Forward declarations
class WControllerPrivate;

//-------------------------------------------------------------------------------------------------
// Defines

#define W_DECLARE_CONTROLLER(Class)   \
                                      \
public:                               \
static Class * instance()             \
{                                     \
    if (Class::_instance == NULL)     \
    {                                 \
        Class::_instance = new Class; \
    }                                 \
                                      \
    return Class::_instance;          \
}                                     \
                                      \
static Class * instancePointer()      \
{                                     \
    return Class::_instance;          \
}                                     \
                                      \
static void clearInstance()           \
{                                     \
    Class::_instance = NULL;          \
}                                     \
                                      \
private:                              \
static Class * _instance;             \

#define W_INIT_CONTROLLER(Class)     \
                                     \
    Class * Class::_instance = NULL; \

//-------------------------------------------------------------------------------------------------

#define W_CREATE_CONTROLLER(Class)       \
                                         \
    Class::instance()->initController(); \

#define W_CREATE_CONTROLLER_1(Class, Param)   \
                                              \
    Class::instance()->initController(Param); \

#define W_CREATE_CONTROLLER_2(Class, Param1, Param2)   \
                                                       \
    Class::instance()->initController(Param1, Param2); \

#define W_CREATE_CONTROLLER_3(Class, Param1, Param2, Param3)   \
                                                               \
    Class::instance()->initController(Param1, Param2, Param3); \

//-------------------------------------------------------------------------------------------------

#define W_CLEAR_CONTROLLER(Class) \
                                  \
    Class::clearInstance();       \

//-------------------------------------------------------------------------------------------------

#define W_GET_CONTROLLER(Class, Name)        \
                                             \
    Class * Name = Class::instancePointer(); \

#define W_CONTROLLER(Class, Name)     \
                                      \
    Class * Name = Class::instance(); \
                                      \
    Q_ASSERT(Name);                   \

//-------------------------------------------------------------------------------------------------

class SKY_CORE_EXPORT WController : public QObject, public WPrivatable
{
    Q_OBJECT

protected:
    WController();
    WController(WControllerPrivate * p);

public: // Interface
    virtual void initController();

    bool isCreated();

protected: // Functions
    virtual void init(); // {}

private:
    W_DECLARE_PRIVATE(WController)
};

#endif // WCONTROLLER_H
