//=================================================================================================
/*
    Copyright (C) 2015-2020 Sky kit authors. <http://omega.gg/Sky>

    Author: Benjamin Arnaud. <http://bunjee.me> <bunjee@omega.gg>

    This file is part of SkGui.

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

#ifndef WBACKENDLOADER_H
#define WBACKENDLOADER_H

// Qt includes
#include <QObject>
#include <QStringList>

// Sk includes
#include <WLibraryItem>
#include <SkyGuiExports.h>

#ifndef SK_NO_BACKENDLOADER

// Forward declarations
class WBackendLoaderPrivate;
class WBackendNet;
class WLibraryFolder;

class SKY_GUI_EXPORT WBackendLoader : public QObject, public WPrivatable
{
    Q_OBJECT

public:
    explicit WBackendLoader(QObject * parent = NULL);
protected:
    WBackendLoader(WBackendLoaderPrivate * p, QObject * parent = NULL);

public: // Interface
    Q_INVOKABLE WBackendNet * create(const QString & id);

    Q_INVOKABLE WBackendNet * match(const QString & url);

    Q_INVOKABLE WBackendNet * matchCover(const QString & label, const QString & q);

    Q_INVOKABLE QString idFromUrl(const QString & url) const;

    Q_INVOKABLE QString coverFromUrl(const QString & url) const;

public: // Static functions
    Q_INVOKABLE static void reloadBackends();

    Q_INVOKABLE static void clearCache();

    Q_INVOKABLE static int  getMaxCache();
    Q_INVOKABLE static void setMaxCache(int max);

public: // Virtual interface
    Q_INVOKABLE virtual bool checkId(const QString & id) const; // {}

    Q_INVOKABLE virtual void createFolderItems(WLibraryFolder * folder,
                                               WLibraryItem::Type type) const; // {}

    Q_INVOKABLE virtual QString coverFromId(const QString & id) const; // {}

protected: // Static functions
    Q_INVOKABLE static WBackendNet * getBackend(const QString & id);

protected: // Virtual functions
    Q_INVOKABLE virtual WBackendNet * createBackend(const QString & id) const; // {}

    Q_INVOKABLE virtual QString getId(const QString & url) const; // {}

    Q_INVOKABLE virtual QStringList getCoverIds() const; // {}

private: // Functions
    WBackendNet * createNow(const QString & id);

private:
    W_DECLARE_PRIVATE(WBackendLoader)

    Q_PRIVATE_SLOT(d_func(), void onDestroyed())

    friend class WControllerPlaylist;
};

#include <private/io/WBackendLoader_p>

#endif // SK_NO_BACKENDLOADER
#endif // WBACKENDLOADER_H
