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

#ifndef WPLUGIN_H
#define WPLUGIN_H

// Qt includes
#include <QObject>

// Sk includes
#include <WPluginInterface.h>
#include <SkyCoreExports.h>

#ifndef SK_NO_PLUGIN

// Forward declarations
class QDomDocument;
class WPluginPrivate;

class SKY_CORE_EXPORT WPlugin : public QObject, public WPrivatable
{
    Q_OBJECT

    Q_PROPERTY(QString fileName READ fileName)

    Q_PROPERTY(bool loaded READ isLoaded)

    Q_PROPERTY(QObject * instance READ instance)

public:
    WPlugin(const QString & fileName, QObject * parent = NULL);

    explicit WPlugin(QObject * parent = NULL);

public: // Interface
    bool load  (const QString & fileName);
    bool unload();

    bool isLoaded() const;

    QString errorString () const;

    template <class T> T interface() const;

public: // Static functions
    static QString specFileName(const QString & pluginFileName);

    static QString nameFromPlugin  (const QString & fileName);
    static QString nameFromSpecFile(const QString & fileName);

protected: // Functions
    virtual bool loadSpecs(QDomDocument * specifications);

protected: // Events
    virtual void loadEvent  ();
    virtual void unloadEvent();

public: // Properties
    QString fileName() const;

    QObject * instance() const;

    QString name() const;

    QString version      () const;
    QString compatVersion() const;

    QString description() const;
    QString url        () const;

    QString license() const;

private:
    W_DECLARE_PRIVATE(WPlugin)
};

template <class T> T WPlugin::interface() const
{
    return qobject_cast<T> (instance());
}

#endif // SK_NO_PLUGIN
#endif // WPLUGIN_H
