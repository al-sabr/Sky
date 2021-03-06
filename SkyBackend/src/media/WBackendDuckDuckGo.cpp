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

#include "WBackendDuckDuckGo.h"

#ifndef SK_NO_BACKENDDUCKDUCKGO

// Qt includes
#ifdef QT_LATEST
#include <QUrlQuery>
#endif

// Sk includes
#include <WControllerApplication>
#include <WControllerNetwork>
#include <SkyBackendExports.h>

//-------------------------------------------------------------------------------------------------
// Private
//-------------------------------------------------------------------------------------------------

#include <private/media/WBackendNet_p>

class SKY_BACKEND_EXPORT WBackendDuckDuckGoPrivate : public WBackendNetPrivate
{
public:
    WBackendDuckDuckGoPrivate(WBackendDuckDuckGo * p);

    void init();

public: // Functions
    QString getUrl(const QString & q) const;

protected:
    W_DECLARE_PUBLIC(WBackendDuckDuckGo)
};

//-------------------------------------------------------------------------------------------------

WBackendDuckDuckGoPrivate::WBackendDuckDuckGoPrivate(WBackendDuckDuckGo * p)
    : WBackendNetPrivate(p) {}

void WBackendDuckDuckGoPrivate::init() {}

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------

QString WBackendDuckDuckGoPrivate::getUrl(const QString & q) const
{
    QUrl url("https://duckduckgo.com/");

    QString search = q.simplified();

#ifdef QT_4
    url.addQueryItem("q", search);

    url.addQueryItem("kl", "us-en");

    url.addQueryItem("kp", "-2");
#else
    QUrlQuery query(url);

    query.addQueryItem("q", search);

    query.addQueryItem("kl", "us-en");

    query.addQueryItem("kp", "-2");

    url.setQuery(query);
#endif

    return url.toString();
}

//-------------------------------------------------------------------------------------------------
// Ctor / dtor
//-------------------------------------------------------------------------------------------------

WBackendDuckDuckGo::WBackendDuckDuckGo() : WBackendNet(new WBackendDuckDuckGoPrivate(this))
{
    Q_D(WBackendDuckDuckGo); d->init();
}

//-------------------------------------------------------------------------------------------------
// WBackendNet implementation
//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QString WBackendDuckDuckGo::getId() const
{
    return "duckduckgo";
}

/* Q_INVOKABLE virtual */ QString WBackendDuckDuckGo::getTitle() const
{
    return "DuckDuckGo";
}

//-------------------------------------------------------------------------------------------------
// WBackendNet reimplementation
//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ bool WBackendDuckDuckGo::isSearchEngine() const
{
    return true;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QString WBackendDuckDuckGo::validate() const
{
    return "^duckduckgo.com";
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */
WBackendNetQuery WBackendDuckDuckGo::createQuery(const QString & method,
                                                 const QString & label, const QString & q) const
{
    WBackendNetQuery query;

    if (method == "search")
    {
        if (label == "urls")
        {
            Q_D(const WBackendDuckDuckGo);

            query.url  = d->getUrl(q);
            query.data = q;
        }
        else if (label == "site")
        {
            Q_D(const WBackendDuckDuckGo);

            query.url  = d->getUrl(q);
            query.id   = 2;
            query.data = q;
        }
    }

    return query;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */
WBackendNetFolder WBackendDuckDuckGo::extractFolder(const QByteArray       & data,
                                                    const WBackendNetQuery & query) const
{
    WBackendNetFolder reply;

    int id = query.id;

    if (id == 1) // Search urls
    {
        QStringList urls;

        QString string = Sk::sliceIn(data, "('d',[", "]);");

        QStringList list = WControllerNetwork::splitJson(string);

        foreach (const QString & string, list)
        {
            QString source = WControllerNetwork::extractJsonUtf8(string, "c");

            if (source.isEmpty()) continue;

            source = WControllerNetwork::urlName(source);

            if (urls.contains(source)) continue;

            urls.append(source);

            WLibraryFolderItem folder(WLibraryItem::FolderSearch, WLocalObject::Default);

            folder.title = source;

            reply.items.append(folder);
        }
    }
    else if (id == 3) // Search site
    {
        QString string = Sk::sliceIn(data, "('d',[", "]);");

        QStringList list = WControllerNetwork::splitJson(string);

        foreach (const QString & string, list)
        {
            QString source = WControllerNetwork::extractJsonUtf8(string, "c");

            if (source.isEmpty()) continue;

            QString title = WControllerNetwork::extractJsonUtf8(string, "t");

            if (title == "EOF") break;

            WLibraryFolderItem folder(WLibraryItem::FolderSearch, WLocalObject::Default);

            folder.source = source;
            folder.title  = title;

            reply.items.append(folder);
        }
    }
    else
    {
        QString content = Sk::readUtf8(data);

        QString source = Sk::sliceIn(content, ";nrj('", "'");

        WBackendNetQuery * nextQuery = &(reply.nextQuery);

        nextQuery->url = "https://duckduckgo.com" + source;

        nextQuery->id = id + 1;
    }

    reply.scanItems = true;

    return reply;
}

#endif // SK_NO_BACKENDDUCKDUCKGO
