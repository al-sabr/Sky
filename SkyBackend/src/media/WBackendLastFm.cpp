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

#include "WBackendLastFm.h"

#ifndef SK_NO_BACKENDLASTFM

// Qt includes
#ifdef QT_LATEST
#include <QUrlQuery>
#endif

// Sk includes
#include <WControllerApplication>
#include <WControllerNetwork>
#include <WControllerPlaylist>
#include <SkyBackendExports.h>

//-------------------------------------------------------------------------------------------------
// Static variables

static const QString BACKENDLASTFM_MATCH = "[,.:\\-_(){}\\[\\]]";

//-------------------------------------------------------------------------------------------------
// Private
//-------------------------------------------------------------------------------------------------

#include <private/media/WBackendNet_p>

class SKY_BACKEND_EXPORT WBackendLastFmPrivate : public WBackendNetPrivate
{
public:
    WBackendLastFmPrivate(WBackendLastFm * p);

    void init();

public: // Functions
    void applyQuery(WBackendNetQuery * query, const QString & label, const QString & q) const;

    bool match(const QStringList & listA, const QStringList & listB) const;

    QString     getString(const QString & data) const;
    QStringList getList  (const QString & data) const;

protected:
    W_DECLARE_PUBLIC(WBackendLastFm)
};

//-------------------------------------------------------------------------------------------------

WBackendLastFmPrivate::WBackendLastFmPrivate(WBackendLastFm * p) : WBackendNetPrivate(p) {}

void WBackendLastFmPrivate::init() {}

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------

void WBackendLastFmPrivate::applyQuery(WBackendNetQuery * query, const QString & label,
                                                                 const QString & q) const
{
    QString title = getString(q);

    int index  = 0;
    int length = title.length();

    while (index < length && title.at(index).isDigit())
    {
        index++;
    }

    if (index == length) return;

    while (index < length && title.at(index).isSpace())
    {
        index++;
    }

    if (index == length) return;

    title = title.mid(index, title.lastIndexOf(' ') - index);

    QVariantList variants;

    variants.append(title);
    variants.append(getString(label));

    QUrl url("https://www.last.fm/search/tracks");

#ifdef QT_4
    url.addQueryItem("q", title);
#else
    QUrlQuery urlQuery(url);

    urlQuery.addQueryItem("q", title);

    url.setQuery(urlQuery);
#endif

    query->url = url.toString();

    query->data = variants;
}

//-------------------------------------------------------------------------------------------------

bool WBackendLastFmPrivate::match(const QStringList & listA, const QStringList & listB) const
{
    int from = 0;

    foreach (const QString & string, listA)
    {
        from = listB.indexOf(string, from);

        if (from == -1)
        {
            from = 0;

            foreach (const QString & string, listB)
            {
                from = listA.indexOf(string, from);

                if (from == -1) return false;
            }

            return true;
        }
    }

    return true;
}

//-------------------------------------------------------------------------------------------------

QString WBackendLastFmPrivate::getString(const QString & data) const
{
    QString result = data;

    result = result.replace(QRegExp(BACKENDLASTFM_MATCH), " ");

    return result.simplified().toLower();
}

QStringList WBackendLastFmPrivate::getList(const QString & data) const
{
    return getString(data).split(' ');
}

//-------------------------------------------------------------------------------------------------
// Ctor / dtor
//-------------------------------------------------------------------------------------------------

WBackendLastFm::WBackendLastFm() : WBackendNet(new WBackendLastFmPrivate(this))
{
    Q_D(WBackendLastFm); d->init();
}

//-------------------------------------------------------------------------------------------------
// WBackendNet implementation
//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QString WBackendLastFm::getId() const
{
    return "lastfm";
}

/* Q_INVOKABLE virtual */ QString WBackendLastFm::getTitle() const
{
    return "Last.fm";
}

//-------------------------------------------------------------------------------------------------
// WBackendNet reimplementation
//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ bool WBackendLastFm::isSearchCover() const
{
    return true;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QString WBackendLastFm::validate() const
{
    return "^last.fm";
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ bool WBackendLastFm::checkCover(const QString & label,
                                                          const QString & q) const
{
    if (label.isEmpty()) return false;

    QString extension = WControllerNetwork::extractUrlExtension(q);

    return WControllerPlaylist::extensionIsAudio(extension);
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */
WBackendNetQuery WBackendLastFm::createQuery(const QString & method,
                                             const QString & label, const QString & q) const
{
    WBackendNetQuery query;

    if (method == "cover")
    {
        Q_D(const WBackendLastFm);

        d->applyQuery(&query, label, q);
    }

    return query;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */
WBackendNetTrack WBackendLastFm::extractTrack(const QByteArray       & data,
                                              const WBackendNetQuery & query) const
{
    Q_D(const WBackendLastFm);

    WBackendNetTrack reply;

    QString content = Sk::readUtf8(data);

    if (query.id == 1)
    {
        int index = content.indexOf("property=\"og:image\"");

        if (index == -1) return reply;

        QString cover = WControllerNetwork::extractAttribute(content, "content", index);

        reply.track.setCover(cover);
    }
    else
    {
        QStringList list = Sk::slices(content, "chartlist-row", "</tr>");

        QVariantList variants = query.data.toList();

        QStringList listTitle  = variants.first().toString().split(' ');
        QStringList listAuthor = variants.last ().toString().split(' ');

        foreach (const QString & string, list)
        {
            int index = string.indexOf("class=\"chartlist-artist\"");

            QString author = WControllerNetwork::extractAttributeUtf8(string, "title", index);

            QStringList list = d->getList(author);

            if (d->match(list, listAuthor))
            {
                index = string.indexOf("class=\"chartlist-name\"");

                QString title = WControllerNetwork::extractAttributeUtf8(string, "title", index);

                list = d->getList(title);

                if (d->match(list, listTitle))
                {
                    QString source = WControllerNetwork::extractAttributeUtf8(string, "href", index);

                    WBackendNetQuery * nextQuery = &(reply.nextQuery);

                    nextQuery->url = "https://www.last.fm" + source;
                    nextQuery->id  = 1;

                    return reply;
                }
            }
        }
    }

    return reply;
}

#endif // SK_NO_BACKENDLASTFM
