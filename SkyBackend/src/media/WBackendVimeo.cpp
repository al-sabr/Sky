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

#include "WBackendVimeo.h"

#ifndef SK_NO_BACKENDVIMEO

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

class SKY_BACKEND_EXPORT WBackendVimeoPrivate : public WBackendNetPrivate
{
public:
    WBackendVimeoPrivate(WBackendVimeo * p);

    void init();

public: // Functions
    QString extractId(const QString & source) const;

    WAbstractBackend::Quality extractQuality(const QString & data) const;

    QString extractCover(const QString & cover) const;

    QString getNextUrl(const WBackendNetQuery & query, const QString & data, int id) const;

protected:
    W_DECLARE_PUBLIC(WBackendVimeo)
};

//-------------------------------------------------------------------------------------------------

WBackendVimeoPrivate::WBackendVimeoPrivate(WBackendVimeo * p) : WBackendNetPrivate(p) {}

void WBackendVimeoPrivate::init() {}

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------

QString WBackendVimeoPrivate::extractId(const QString & source) const
{
    int index = source.lastIndexOf('/');

    if (index == -1) return QString();

    QString id = WControllerNetwork::extractUrlElement(source, index + 1);

    if (id.toInt() == 0 || source.contains("/channels/" + id) || source.contains("/groups/" + id))
    {
         return QString();
    }
    else return id;
}

//-------------------------------------------------------------------------------------------------

WAbstractBackend::Quality WBackendVimeoPrivate::extractQuality(const QString & data) const
{
    QString quality = WControllerNetwork::extractJson(data, "quality");

    if (quality == "270p")
    {
        return WAbstractBackend::Quality240;
    }
    else if (quality == "360p")
    {
        return WAbstractBackend::Quality360;
    }
    else if (quality == "480p")
    {
        return WAbstractBackend::Quality480;
    }
    else if (quality == "720p")
    {
        return WAbstractBackend::Quality720;
    }
    else if (quality == "1080p")
    {
        return WAbstractBackend::Quality1080;
    }
    else return WAbstractBackend::QualityInvalid;
}

//-------------------------------------------------------------------------------------------------

QString WBackendVimeoPrivate::extractCover(const QString & cover) const
{
    if (cover.isEmpty())
    {
         return QString();
    }
    else if (cover.startsWith("https://i.vimeocdn.com"))
    {
        int index = cover.indexOf("?src");

        if (index == -1)
        {
            index = cover.lastIndexOf('_');

            if (index == -1)
            {
                 return cover;
            }
            else return cover.mid(0, index + 1) + "1280.jpg";
        }
        else
        {
            index += 6;

            int indexB = cover.indexOf('_', index);

            if (indexB != -1)
            {
                QString source = cover.mid(index, indexB - index + 1);

                return WControllerNetwork::decodeUrl(source) + "1280.jpg";
            }
            else return QString();
        }
    }
    else if (cover.startsWith("https://vimeo.com"))
    {
        int index = cover.lastIndexOf('/');

        if (index != -1)
        {
            QString id = cover.mid(index + 1);

            return "https://i.vimeocdn.com/video/" + id +  + "_1280.jpg";
        }
        else return QString();
    }
    else return WControllerNetwork::decodeUrl(cover);
}

//-------------------------------------------------------------------------------------------------

QString WBackendVimeoPrivate::getNextUrl(const WBackendNetQuery & query,
                                         const QString          & data, int id) const
{
    QString paging = WControllerNetwork::extractJson(data, "paging");

    if (paging.isEmpty())
    {
        if (data.indexOf("<li class=\"pagination_next") == -1)
        {
            return QString();
        }
    }
    else if (WControllerNetwork::extractJson(paging, "next") == "null")
    {
        return QString();
    }

    QString url = query.url;

    url.replace("/page:" + QString::number(id),
                "/page:" + QString::number(id + 1));

    return url;
}

//-------------------------------------------------------------------------------------------------
// Ctor / dtor
//-------------------------------------------------------------------------------------------------

WBackendVimeo::WBackendVimeo() : WBackendNet(new WBackendVimeoPrivate(this))
{
    Q_D(WBackendVimeo); d->init();
}

//-------------------------------------------------------------------------------------------------
// WBackendNet implementation
//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QString WBackendVimeo::getId() const
{
    return "vimeo";
}

/* Q_INVOKABLE virtual */ QString WBackendVimeo::getTitle() const
{
    return "Vimeo";
}

//-------------------------------------------------------------------------------------------------
// WBackendNet reimplementation
//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ bool WBackendVimeo::hasSearch() const
{
    return true;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QString WBackendVimeo::validate() const
{
    return "^vimeo.com|^player.vimeo.com";
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QString WBackendVimeo::getHost() const
{
    return "vimeo.com";
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QList<WLibraryFolderItem> WBackendVimeo::getLibraryItems() const
{
    WLibraryFolderItem tracks(WLibraryItem::PlaylistSearch);

    tracks.title = tr("Tracks");
    tracks.label = "tracks";

    WLibraryFolderItem people(WLibraryItem::FolderSearch);

    people.title = tr("People");
    people.label = "people";

    WLibraryFolderItem channels(WLibraryItem::FolderSearch);

    channels.title = tr("Channels");
    channels.label = "channels";

    WLibraryFolderItem groups(WLibraryItem::FolderSearch);

    groups.title = tr("Groups");
    groups.label = "groups";

    QList<WLibraryFolderItem> items;

    items.append(tracks);
    items.append(people);
    items.append(channels);
    items.append(groups);

    return items;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QString WBackendVimeo::getTrackId(const QString & url) const
{
    Q_D(const WBackendVimeo);

    return d->extractId(url);
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */
WBackendNetPlaylistInfo WBackendVimeo::getPlaylistInfo(const QString & url) const
{
    QString source = WControllerNetwork::removeUrlPrefix(url);

    if (source.startsWith("vimeo.com") == false)
    {
        return WBackendNetPlaylistInfo();
    }

    if (source.startsWith("vimeo.com/channels/") || source.startsWith("vimeo.com/groups/"))
    {
        return WBackendNetPlaylistInfo(WLibraryItem::PlaylistFeed,
                                       WControllerNetwork::extractUrlElement(source, 2, 10));
    }
    else if (source.startsWith("vimeo.com/"))
    {
        QString id = WControllerNetwork::extractUrlElement(source, 10);

        if (id.toInt())
        {
             return WBackendNetPlaylistInfo();
        }
        else return WBackendNetPlaylistInfo(WLibraryItem::PlaylistFeed, id);
    }
    else return WBackendNetPlaylistInfo();
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */
QString WBackendVimeo::getUrlTrack(const QString & id) const
{
    return "https://vimeo.com/" + id;
}

/* Q_INVOKABLE virtual */
QString WBackendVimeo::getUrlPlaylist(const WBackendNetPlaylistInfo & info) const
{
    return "https://vimeo.com/" + info.id;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */
WBackendNetQuery WBackendVimeo::getQuerySource(const QString & url) const
{
    QString id = getTrackId(url);

    if (id.isEmpty())
    {
         return WBackendNetQuery();
    }
    else return WBackendNetQuery("https://player.vimeo.com/video/" + id);
}

/* Q_INVOKABLE virtual */
WBackendNetQuery WBackendVimeo::getQueryTrack(const QString & url) const
{
    QString id = getTrackId(url);

    if (id.isEmpty())
    {
         return WBackendNetQuery();
    }
    else return WBackendNetQuery("https://vimeo.com/" + id);
}

/* Q_INVOKABLE virtual */
WBackendNetQuery WBackendVimeo::getQueryPlaylist(const QString & url) const
{
    QString id = getPlaylistInfo(url).id;

    if (id.isEmpty()) return WBackendNetQuery();

    if (id.startsWith("tag:"))
    {
         return WBackendNetQuery("https://vimeo.com/" + id + "/page:1/sort:date/format:thumbnail");
    }
    else return WBackendNetQuery("https://vimeo.com/" + id
                                 +
                                 "/videos/page:1/sort:date/format:thumbnail");
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */
WBackendNetQuery WBackendVimeo::createQuery(const QString & method,
                                            const QString & label, const QString & q) const
{
    WBackendNetQuery query;

    if (method == "search")
    {
        if (label == "tracks")
        {
            QUrl url("https://vimeo.com/search/page:1/sort:relevant/format:thumbnail");

#ifdef QT_4
            url.addQueryItem("q", q);

            url.addQueryItem("type", "video");
#else
            QUrlQuery urlQuery(url);

            urlQuery.addQueryItem("q", q);

            urlQuery.addQueryItem("type", "video");

            url.setQuery(urlQuery);
#endif

            query.url = url.toString();
            query.id  = 1;

            query.header = true;
        }
        else if (label == "people")
        {
            QUrl url("https://vimeo.com/search/people/page:1/sort:relevant/format:thumbnail");

#ifdef QT_4
            url.addQueryItem("q", q);
#else
            QUrlQuery urlQuery(url);

            urlQuery.addQueryItem("q", q);

            url.setQuery(urlQuery);
#endif

            query.url = url.toString();
            query.id  = 1;

            query.header = true;
        }
        else if (label == "channels")
        {
            QUrl url("https://vimeo.com/search/channels/page:1/sort:relevant/format:thumbnail");

#ifdef QT_4
            url.addQueryItem("q", q);
#else
            QUrlQuery urlQuery(url);

            urlQuery.addQueryItem("q", q);

            url.setQuery(urlQuery);
#endif

            query.url = url.toString();

            query.header = true;
        }
        else if (label == "groups")
        {
            QUrl url("https://vimeo.com/search/groups/page:1/sort:relevant/format:thumbnail");

#ifdef QT_4
            url.addQueryItem("q", q);
#else
            QUrlQuery urlQuery(url);

            urlQuery.addQueryItem("q", q);

            url.setQuery(urlQuery);
#endif

            query.url = url.toString();

            query.header = true;
        }
    }
    else if (method == "related" && label == "tracks")
    {
        query.url = "https://vimeo.com/" + q
                    +
                    "/collections/channels/sort:relevant/format:thumbnail";

        query.id = 2;
    }

    return query;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */
WBackendNetSource WBackendVimeo::extractSource(const QByteArray       & data,
                                               const WBackendNetQuery &) const
{
    Q_D(const WBackendVimeo);

    WBackendNetSource reply;

    QString content = Sk::readUtf8(data);

    QHash<WAbstractBackend::Quality, QString> * medias = &(reply.medias);

    QString json = WControllerNetwork::extractJsonHtml(content, "progressive");

    QStringList list = WControllerNetwork::splitJson(json);

    foreach (const QString & string, list)
    {
        WAbstractBackend::Quality quality = d->extractQuality(string);

        if (quality != WAbstractBackend::QualityInvalid)
        {
            QString url = WControllerNetwork::extractJson(string, "url");

            medias->insert(quality, url);
        }
    }

    return reply;
}

/* Q_INVOKABLE virtual */
WBackendNetTrack WBackendVimeo::extractTrack(const QByteArray       & data,
                                             const WBackendNetQuery &) const
{
    Q_D(const WBackendVimeo);

    WBackendNetTrack reply;

    QString content = Sk::readUtf8(data);

    content = Sk::slice(content, "<script type=\"application", "</script>");

    QString title = WControllerNetwork::extractJsonUtf8(content, "name");
    QString cover = WControllerNetwork::extractJson    (content, "thumbnailUrl");

    cover = d->extractCover(cover);

    QString author = WControllerNetwork::extractJsonUtf8(content, "author");

    QString feed = WControllerNetwork::extractJson(author, "url");

    feed = WControllerNetwork::extractUrlPath(feed);

    author = WControllerNetwork::extractJson(author, "name");

    QString duration = WControllerNetwork::extractJson(content, "duration");

    duration = Sk::sliceIn(duration, "T", "S");

    QString date = WControllerNetwork::extractJson(content, "uploadDate");

    if (date.isEmpty())
    {
        date = WControllerNetwork::extractJson(content, "datePublished");
    }

    date = date.mid(0, date.lastIndexOf('-'));

    QString quality = WControllerNetwork::extractJson(content, "videoQuality");

    WTrack * track = &(reply.track);

    track->setTitle(title);
    track->setCover(cover);

    track->setAuthor(author);
    track->setFeed  (feed);

    QTime time = QTime::fromString(duration, "hh'H'mm'M'ss");

    track->setDuration(Sk::getMsecs(time));

    track->setDate(QDateTime::fromString(date, "yyyy-MM-ddThh:mm:ss"));

    if (quality == "HD")
    {
         track->setQuality(WAbstractBackend::Quality720);
    }
    else track->setQuality(WAbstractBackend::Quality480);

    return reply;
}

/* Q_INVOKABLE virtual */
WBackendNetPlaylist WBackendVimeo::extractPlaylist(const QByteArray       & data,
                                                   const WBackendNetQuery & query) const
{
    Q_D(const WBackendVimeo);

    WBackendNetPlaylist reply;

    QString content = Sk::readUtf8(data);

    int id = query.id;

    if (id == 2)
    {
        QStringList urls = query.data.toStringList();

        int index = content.indexOf("<ol class=\"js-browse_list");

        QString string;

        if (index == -1)
        {
             string = Sk::slice(content, "<ul class=\"small-block-grid", "</ul>");
        }
        else string = Sk::slice(content, "<ol class=\"js-browse_list", "</ol>");

        QStringList list = Sk::slices(string, "<li ", "</li>");

        for (int i = 0; i < list.count() && i < 3; i++)
        {
            const QString & string = list.at(i);

            QString id = WControllerNetwork::extractAttribute(string, "href");

            urls.append("https://vimeo.com" + id + "/videos/page:1/sort:relevant/format:thumbnail");
        }

        if (urls.isEmpty() == false)
        {
            WBackendNetQuery * nextQuery = &(reply.nextQuery);

            nextQuery->url = urls.takeFirst();

            if (urls.isEmpty() == false)
            {
                nextQuery->id   = 3;
                nextQuery->data = urls;
            }
            else nextQuery->id = 4;

            nextQuery->header = true;
        }

        return reply;
    }
    else if (id == 3)
    {
        QStringList urls = query.data.toStringList();

        WBackendNetQuery * nextQuery = &(reply.nextQuery);

        nextQuery->url = urls.takeFirst();

        if (urls.isEmpty() == false)
        {
            nextQuery->id   = 3;
            nextQuery->data = urls;
        }
        else nextQuery->id = 4;

        nextQuery->header = true;

        reply.clearDuplicate = true;
    }
    else if (id != 4)
    {
        int queryId = query.data.toInt() + 1;

        if (queryId < 3)
        {
            if (id == 0 && queryId == 1)
            {
                QString json = WControllerNetwork::extractJsonHtml(content, "itemListElement");

                int index = json.lastIndexOf("\"name\"");

                reply.title = WControllerNetwork::extractJsonUtf8(json, "name", index);
            }

            QString url = d->getNextUrl(query, content, queryId);

            if (url.isEmpty() == false)
            {
                WBackendNetQuery * nextQuery = &(reply.nextQuery);

                nextQuery->url = url;
                nextQuery->id  = id;

                nextQuery->data = queryId;

                nextQuery->header = true;
            }
        }

        if (id == 1)
        {
            QString json = WControllerNetwork::extractJsonHtml(content, "data");

            QStringList list = WControllerNetwork::splitJson(json);

            foreach (const QString & string, list)
            {
                QString privacy = WControllerNetwork::extractJson(string, "privacy");

                if (privacy == "\"view\":\"ptv\"") continue;

                QString id = WControllerNetwork::extractJson(string, "uri");

                id = id.mid(8);

                QString title = WControllerNetwork::extractJsonUtf8(string, "name");

                QString cover = WControllerNetwork::extractJson(string, "pictures");

                cover = WControllerNetwork::extractJson(cover, "link");

                cover = d->extractCover(cover);

                WTrack track("https://vimeo.com/" + id, WTrack::Default);

                track.setTitle(title);
                track.setCover(cover);

                reply.tracks.append(track);
            }

            return reply;
        }
    }
    else reply.clearDuplicate = true;

    QString string = Sk::slice(content, "<ol class=\"js-browse_list", "</ol>");

    QStringList list = Sk::slices(string, "<li ", "</li>");

    foreach (const QString & string, list)
    {
        QString id = WControllerNetwork::extractAttribute(string, "href");

        id = d->extractId(id);

        QString title = WControllerNetwork::extractAttributeUtf8(string, "title");
        QString cover = WControllerNetwork::extractAttribute    (string, "srcset");

        cover = d->extractCover(cover);

        WTrack track("https://vimeo.com/" + id, WTrack::Default);

        track.setTitle(title);
        track.setCover(cover);

        reply.tracks.append(track);
    }

    return reply;
}

/* Q_INVOKABLE virtual */
WBackendNetFolder WBackendVimeo::extractFolder(const QByteArray       & data,
                                               const WBackendNetQuery & query) const
{
    Q_D(const WBackendVimeo);

    WBackendNetFolder reply;

    QString content = Sk::readUtf8(data);

    QString json = WControllerNetwork::extractJsonHtml(content, "data");

    QStringList list = WControllerNetwork::splitJson(json);

    foreach (const QString & string, list)
    {
        QString source = WControllerNetwork::extractJson(string, "link");

        QString title = WControllerNetwork::extractJsonUtf8(string, "name");

        QString cover = WControllerNetwork::extractJson(string, "pictures");

        cover = WControllerNetwork::extractJson(cover, "link");

        cover = d->extractCover(cover);

        WLibraryFolderItem playlist(WLibraryItem::PlaylistFeed, WLibraryItem::Default);

        playlist.source = source;

        playlist.title = title;
        playlist.cover = cover;

        reply.items.append(playlist);
    }

    int id = query.data.toInt() + 1;

    if (id < 3)
    {
        QString url = d->getNextUrl(query, content, id);

        if (url.isEmpty() == false)
        {
            WBackendNetQuery * nextQuery = &(reply.nextQuery);

            nextQuery->url = url;
            nextQuery->id  = query.id;

            nextQuery->data = id;

            nextQuery->header = true;
        }
    }

    return reply;
}

#endif // SK_NO_BACKENDVIMEO
