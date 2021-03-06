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

#include "WBackendBing.h"

#ifndef SK_NO_BACKENDBING

// Qt includes
#ifdef QT_LATEST
#include <QUrlQuery>
#endif

// Sk includes
#include <WControllerApplication>
#include <WControllerNetwork>
#include <SkyBackendExports.h>

//-------------------------------------------------------------------------------------------------
// Static variables

static const QString BACKENDBING_FILTERS
    = "filterui:imagesize-custom_512_512+filterui:aspect-wide";

//-------------------------------------------------------------------------------------------------
// Private
//-------------------------------------------------------------------------------------------------

#include <private/media/WBackendNet_p>

class SKY_BACKEND_EXPORT WBackendBingPrivate : public WBackendNetPrivate
{
public:
    WBackendBingPrivate(WBackendBing * p);

    void init();

public: // Functions
    QString getUrl(const QString & q) const;

protected:
    W_DECLARE_PUBLIC(WBackendBing)
};

//-------------------------------------------------------------------------------------------------

WBackendBingPrivate::WBackendBingPrivate(WBackendBing * p) : WBackendNetPrivate(p) {}

void WBackendBingPrivate::init() {}

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------

QString WBackendBingPrivate::getUrl(const QString & q) const
{
    QUrl url("http://www.bing.com/images/search");

    QString search = q;

    search.replace(QRegExp("[,.\\-_(){}\\[\\]]"), " ");

    search = search.simplified();

    if (search.length() < 10 || search.count('+') < 3)
    {
        return QString();
    }

#ifdef QT_4
    url.addQueryItem("q", search);

    url.addQueryItem("qft", BACKENDBING_FILTERS);
#else
    QUrlQuery query(url);

    query.addQueryItem("q", search);

    query.addQueryItem("qft", BACKENDBING_FILTERS);

    url.setQuery(query);
#endif

    return url.toString();
}

//-------------------------------------------------------------------------------------------------
// Ctor / dtor
//-------------------------------------------------------------------------------------------------

WBackendBing::WBackendBing() : WBackendNet(new WBackendBingPrivate(this))
{
    Q_D(WBackendBing); d->init();
}

//-------------------------------------------------------------------------------------------------
// WBackendNet implementation
//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QString WBackendBing::getId() const
{
    return "bing";
}

/* Q_INVOKABLE virtual */ QString WBackendBing::getTitle() const
{
    return "Bing";
}

//-------------------------------------------------------------------------------------------------
// WBackendNet reimplementation
//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QString WBackendBing::validate() const
{
    return "^bing.com";
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */
WBackendNetQuery WBackendBing::createQuery(const QString & method,
                                           const QString & label, const QString & q) const
{
    WBackendNetQuery query;

    if (method == "cover" && label == "track")
    {
        Q_D(const WBackendBing);

        query.url = d->getUrl(q);

        query.maxHost = 1;
    }

    return query;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */
WBackendNetTrack WBackendBing::extractTrack(const QByteArray       & data,
                                            const WBackendNetQuery &) const
{
    WBackendNetTrack reply;

    QString content = Sk::readUtf8(data);

    QString cover = Sk::sliceIn(content, "<div class=\"item\"><a href=\"", "\"");

    if (cover.isEmpty()) return reply;

    cover = WControllerNetwork::decodeUrl(cover);

    reply.track.setCover(cover);

    return reply;
}

#endif // SK_NO_BACKENDBING
