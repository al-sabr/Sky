//=================================================================================================
/*
    Copyright (C) 2015-2020 Sky kit authors. <http://omega.gg/Sky>

    Author: Benjamin Arnaud. <http://bunjee.me> <bunjee@omega.gg>

    This file is part of SkBackend.

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

#ifndef WBACKENDYOUTUBE_H
#define WBACKENDYOUTUBE_H

// Sk includes
#include <WBackendNet>
#include <SkyBackendExports.h>

#ifndef SK_NO_BACKENDYOUTUBE

// Forward declarations
class WBackendYoutubePrivate;

class SKY_BACKEND_EXPORT WBackendYoutube : public WBackendNet
{
    Q_OBJECT

public:
    WBackendYoutube();

public: // WBackendNet implementation
    /* Q_INVOKABLE virtual */ QString getId   () const;
    /* Q_INVOKABLE virtual */ QString getTitle() const;

public: // WBackendNet reimplementation
    /* Q_INVOKABLE virtual */ bool hasSearch() const;

    /* Q_INVOKABLE virtual */ QString validate() const;

    /* Q_INVOKABLE virtual */ QString getHost() const;

    /* Q_INVOKABLE virtual */ QList<WLibraryFolderItem> getLibraryItems() const;

    /* Q_INVOKABLE virtual */ QString getTrackId(const QString & url) const;

    /* Q_INVOKABLE virtual */ WBackendNetPlaylistInfo getPlaylistInfo(const QString & url) const;

    /* Q_INVOKABLE virtual */ QString getUrlTrack(const QString & id) const;

    /* Q_INVOKABLE virtual */ QString getUrlPlaylist(const WBackendNetPlaylistInfo & info) const;

    //---------------------------------------------------------------------------------------------

    /* Q_INVOKABLE virtual */ WBackendNetQuery getQuerySource  (const QString & url) const;
    /* Q_INVOKABLE virtual */ WBackendNetQuery getQueryTrack   (const QString & url) const;
    /* Q_INVOKABLE virtual */ WBackendNetQuery getQueryPlaylist(const QString & url) const;

    //---------------------------------------------------------------------------------------------

    /* Q_INVOKABLE virtual */
    WBackendNetQuery createQuery(const QString & method,
                                 const QString & label, const QString & q) const;

    //---------------------------------------------------------------------------------------------

    /* Q_INVOKABLE virtual */
    WBackendNetSource extractSource(const QByteArray       & data,
                                    const WBackendNetQuery & query) const;

    /* Q_INVOKABLE virtual */
    WBackendNetTrack extractTrack(const QByteArray       & data,
                                  const WBackendNetQuery & query) const;

    /* Q_INVOKABLE virtual */
    WBackendNetPlaylist extractPlaylist(const QByteArray       & data,
                                        const WBackendNetQuery & query) const;

    /* Q_INVOKABLE virtual */
    WBackendNetFolder extractFolder(const QByteArray       & data,
                                    const WBackendNetQuery & query) const;

    //---------------------------------------------------------------------------------------------

    /* Q_INVOKABLE virtual */ void applySource(const WBackendNetQuery  & query,
                                               const WBackendNetSource & source);

private:
    W_DECLARE_PRIVATE(WBackendYoutube)
};

#endif // SK_NO_BACKENDYOUTUBE
#endif // WBACKENDYOUTUBE_H
