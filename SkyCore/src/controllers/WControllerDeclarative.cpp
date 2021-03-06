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

#include "WControllerDeclarative.h"

#ifndef SK_NO_CONTROLLERDECLARATIVE

// Qt includes
#include <QCoreApplication>
#ifdef QT_4
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#else
#include <QQmlEngine>
#include <QQmlContext>
#endif
#include <QDir>

W_INIT_CONTROLLER(WControllerDeclarative)

//-------------------------------------------------------------------------------------------------
// Private
//-------------------------------------------------------------------------------------------------

#include <private/controllers/WController_p>

class SKY_CORE_EXPORT WControllerDeclarativePrivate : public WControllerPrivate
{
public:
    WControllerDeclarativePrivate(WControllerDeclarative * p);

    /* virtual */ ~WControllerDeclarativePrivate();

    void init();

public: // Variables
#ifdef QT_4
    QDeclarativeEngine * engine;
#else
    QQmlEngine * engine;
#endif

protected:
    W_DECLARE_PUBLIC(WControllerDeclarative)
};

//-------------------------------------------------------------------------------------------------

WControllerDeclarativePrivate::WControllerDeclarativePrivate(WControllerDeclarative * p)
    : WControllerPrivate(p) {}

/* virtual */ WControllerDeclarativePrivate::~WControllerDeclarativePrivate()
{
    W_CLEAR_CONTROLLER(WControllerDeclarative);
}

//-------------------------------------------------------------------------------------------------

void WControllerDeclarativePrivate::init()
{
#ifdef QT_4
    engine = new QDeclarativeEngine(qApp);
#else
    engine = new QQmlEngine(qApp);
#endif

    engine->addImportPath(QDir::currentPath());
    engine->addPluginPath(QDir::currentPath());
}

//-------------------------------------------------------------------------------------------------
// Private ctor / dtor
//-------------------------------------------------------------------------------------------------

WControllerDeclarative::WControllerDeclarative()
    : WController(new WControllerDeclarativePrivate(this)) {}

//-------------------------------------------------------------------------------------------------
// Initialize
//-------------------------------------------------------------------------------------------------

/* virtual */ void WControllerDeclarative::init()
{
    Q_D(WControllerDeclarative); d->init();
}

//-------------------------------------------------------------------------------------------------
// Interface
//-------------------------------------------------------------------------------------------------

void WControllerDeclarative::clearComponentCache()
{
    Q_D(WControllerDeclarative);

    d->engine->clearComponentCache();
}

void WControllerDeclarative::setContextProperty(const QString & name, const QVariant & value)
{
    Q_D(WControllerDeclarative);

    d->engine->rootContext()->setContextProperty(name, value);
}

void WControllerDeclarative::setContextProperty(const QString & name, QObject * value)
{
    Q_D(WControllerDeclarative);

    d->engine->rootContext()->setContextProperty(name, value);
}

//-------------------------------------------------------------------------------------------------
// Properties
//-------------------------------------------------------------------------------------------------

#ifdef QT_4
QDeclarativeEngine * WControllerDeclarative::engine() const
#else
QQmlEngine * WControllerDeclarative::engine() const
#endif
{
    Q_D(const WControllerDeclarative); return d->engine;
}

#endif // SK_NO_CONTROLLERDECLARATIVE
