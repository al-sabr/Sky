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

#include "WBackendYoutube.h"

#ifndef SK_NO_BACKENDYOUTUBE

// Qt includes
#ifdef QT_4
#include <QScriptEngine>
#else
#include <QJSEngine>
#include <QUrlQuery>
#endif

// Sk includes
#include <WControllerApplication>
#include <WControllerNetwork>
#include <WControllerScript>
#include <SkyBackendExports.h>
//-------------------------------------------------------------------------------------------------
// Private
//-------------------------------------------------------------------------------------------------

#include <private/media/WBackendNet_p>

class SKY_BACKEND_EXPORT WBackendYoutubePrivate : public WBackendNetPrivate
{
public:
    WBackendYoutubePrivate(WBackendYoutube * p);

    void init();

public: // Functions
    void loadTracks(QList<WTrack> * tracks, const QStringList & list, const QString & start) const;

    bool loadTrack(WTrack * track, const QString & data, int from) const;

    void loadMedia(QHash<WAbstractBackend::Quality, QString> * sources,
                   const QString                             & data) const;

    void loadAdaptative(QHash<WAbstractBackend::Quality, QString> * medias,
                        QHash<WAbstractBackend::Quality, QString> * audios,
                        const QString                             & data,
                        const QString                             & audio) const;

    QString extractUrl(const QString & data) const;

    QString extractType(const QString & data) const;
    QString extractMime(const QString & data) const;

    WAbstractBackend::Quality extractQuality     (const QString & data) const;
    WAbstractBackend::Quality extractQualityLabel(const QString & data) const;

    QString extractPlaylistCover(const QString & data, int from) const;

    void applySignatures(WBackendNetSource * source, const QVariantList & variants,
                                                     const QString      & script) const;

#ifdef QT_4
    void applySignature(QString * source, QScriptValue * value) const;
#else
    void applySignature(QString * source, QJSValue * value) const;
#endif

    QString getNextUrl(const QString & data) const;

private:
    QStringList script;

protected:
    W_DECLARE_PUBLIC(WBackendYoutube)
};

//-------------------------------------------------------------------------------------------------

WBackendYoutubePrivate::WBackendYoutubePrivate(WBackendYoutube * p) : WBackendNetPrivate(p) {}

void WBackendYoutubePrivate::init()
{
    script.append(QString());
}

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------

void WBackendYoutubePrivate::loadTracks(QList<WTrack> * tracks, const QStringList & list,
                                                                const QString     & start) const
{
    foreach (const QString & string, list)
    {
        WTrack track;

        int index = string.indexOf(start);

        if (loadTrack(&track, string, index))
        {
            tracks->append(track);
        }
    }
}

bool WBackendYoutubePrivate::loadTrack(WTrack * track, const QString & data, int from) const
{
    QString source = WControllerNetwork::extractAttribute(data, "href", from);

    if (source.isEmpty() || source.startsWith('/') == false)
    {
        return false;
    }

    QString id = WControllerNetwork::extractUrlValue(source, "v");

    QString title = WControllerNetwork::extractAttributeUtf8(data, "title", from);

    track->setState(WTrack::Default);

    track->setSource("https://www.youtube.com" + source);

    track->setTitle(title);

    track->setCover("http://i.ytimg.com/vi/" + id + "/hqdefault.jpg");

    return true;
}

//-------------------------------------------------------------------------------------------------

void WBackendYoutubePrivate::loadMedia(QHash<WAbstractBackend::Quality, QString> * sources,
                                       const QString                             & data) const
{
    QString type = extractType(data);

    if (type.isEmpty()) return;

    WAbstractBackend::Quality quality = extractQuality(data);

    if (quality == WAbstractBackend::QualityInvalid) return;

    QString url = extractUrl(data);

    sources->insert(quality, url);
}

void WBackendYoutubePrivate::loadAdaptative(QHash<WAbstractBackend::Quality, QString> * medias,
                                            QHash<WAbstractBackend::Quality, QString> * audios,
                                            const QString                             & data,
                                            const QString                             & audio) const
{
    QString mime = extractMime(data);

    if (mime.isEmpty()) return;

    WAbstractBackend::Quality quality = extractQualityLabel(data);

    if (quality == WAbstractBackend::QualityInvalid) return;

    QString url = extractUrl(data);

    medias->insert(quality, url);
    audios->insert(quality, audio);
}

//-------------------------------------------------------------------------------------------------

QString WBackendYoutubePrivate::extractUrl(const QString & data) const
{
    QString url       = Sk::sliceIn(data, "url=",         "&");
    QString signature = Sk::sliceIn(data, "signature%3D", "&");

    if (signature.isEmpty())
    {
        signature = Sk::sliceIn(data, QRegExp("^s=|&s="), QRegExp("&"));

        if (signature.isEmpty() == false)
        {
            return WControllerNetwork::decodeUrl(url + "&s=" + signature);
        }
    }

    return WControllerNetwork::decodeUrl(url);
}

//-------------------------------------------------------------------------------------------------

QString WBackendYoutubePrivate::extractType(const QString & data) const
{
    QString type = Sk::sliceIn(data, "type=", "&");

    type = type.mid(8, 3);

    if      (type == "mp4") return "mp4";
    else if (type == "web") return "webM";
    else                    return QString();
}

QString WBackendYoutubePrivate::extractMime(const QString & data) const
{
    QString mime = Sk::sliceIn(data, "mime%3D", "%26");

    mime = mime.mid(10, 3);

    if (mime == "mp4") return "mp4";
    else               return QString();
}

//-------------------------------------------------------------------------------------------------

WAbstractBackend::Quality WBackendYoutubePrivate::extractQuality(const QString & data) const
{
    QString quality = Sk::sliceIn(data, "quality=", "&");

    quality = quality.mid(0, 3);

    if      (quality == "sma") return WAbstractBackend::Quality240;
    else if (quality == "med") return WAbstractBackend::Quality360;
    else if (quality == "lar") return WAbstractBackend::Quality480;
    else if (quality == "hd7") return WAbstractBackend::Quality720;
    else if (quality == "hd1") return WAbstractBackend::Quality1080;
    else if (quality == "hig") return WAbstractBackend::Quality1440;
    else                       return WAbstractBackend::QualityInvalid;
}

WAbstractBackend::Quality WBackendYoutubePrivate::extractQualityLabel(const QString & data) const
{
    QString quality = Sk::sliceIn(data, "quality_label=", "p");

    if      (quality == "240")  return WAbstractBackend::Quality240;
    else if (quality == "360")  return WAbstractBackend::Quality360;
    else if (quality == "480")  return WAbstractBackend::Quality480;
    else if (quality == "720")  return WAbstractBackend::Quality720;
    else if (quality == "1080") return WAbstractBackend::Quality1080;
    else if (quality == "1440") return WAbstractBackend::Quality1440;
    else if (quality == "2160") return WAbstractBackend::Quality2160;
    else                        return WAbstractBackend::QualityInvalid;
}

//-------------------------------------------------------------------------------------------------

QString WBackendYoutubePrivate::extractPlaylistCover(const QString & data, int from) const
{
    QString cover = WControllerNetwork::extractAttribute(data, "data-thumb", from);

    if (cover.isEmpty())
    {
        cover = WControllerNetwork::extractAttribute(data, "src", from);
    }

    return WControllerNetwork::generateUrl(cover, "https://www.youtube.com");
}

//-------------------------------------------------------------------------------------------------

void WBackendYoutubePrivate::applySignatures(WBackendNetSource  * source,
                                             const QVariantList & variants,
                                             const QString      & script) const
{
    QHash<WAbstractBackend::Quality, QString> * medias = &(source->medias);
    QHash<WAbstractBackend::Quality, QString> * audios = &(source->audios);

    QStringList listMedias = variants.at(0).toStringList();
    QStringList listAudios = variants.at(1).toStringList();

#ifdef QT_4
    QScriptValue value = wControllerScript->engine()->evaluate(script);
#else
    QJSValue value = wControllerScript->engine()->evaluate(script);
#endif

    for (int i = 0; i <= WAbstractBackend::Quality2160; i++)
    {
        QString * source = &(listMedias[i]);

        if (source->isEmpty()) continue;

        applySignature(source, &value);

        medias->insert(static_cast<WAbstractBackend::Quality> (i), *source);

        source = &(listAudios[i]);

        if (source->isEmpty()) continue;

        applySignature(source, &value);

        audios->insert(static_cast<WAbstractBackend::Quality> (i), *source);
    }
}

#ifdef QT_4
void WBackendYoutubePrivate::applySignature(QString * source, QScriptValue * value) const
#else
void WBackendYoutubePrivate::applySignature(QString * source, QJSValue * value) const
#endif
{
    QRegExp regExp("&s=([a-zA-Z0-9\\.\\-\\_\\=]+)");

    QString signature = Sk::regExpCap(*source, regExp.pattern(), 1);

#ifdef QT_4
    signature = value->call(QScriptValue(), QScriptValueList() << signature).toString();
#else
    signature = value->call(QJSValueList() << signature).toString();
#endif

    source->replace(regExp, "&sig=" + signature);
}

//-------------------------------------------------------------------------------------------------

QString WBackendYoutubePrivate::getNextUrl(const QString & data) const
{
    int index = data.indexOf("class=\"branded-page-box search-pager");

    if (index == -1) return QString();

    QString source = WControllerNetwork::extractAttribute(data, "href", index);

    return "https://www.youtube.com" + source;
}

//-------------------------------------------------------------------------------------------------
// Ctor / dtor
//-------------------------------------------------------------------------------------------------

WBackendYoutube::WBackendYoutube() : WBackendNet(new WBackendYoutubePrivate(this))
{
    Q_D(WBackendYoutube); d->init();
}

//-------------------------------------------------------------------------------------------------
// WBackendNet implementation
//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QString WBackendYoutube::getId() const
{
    return "youtube";
}

/* Q_INVOKABLE virtual */ QString WBackendYoutube::getTitle() const
{
    return "Youtube";
}

//-------------------------------------------------------------------------------------------------
// WBackendNet reimplementation
//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ bool WBackendYoutube::hasSearch() const
{
    return true;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QString WBackendYoutube::validate() const
{
    return "^youtube.com|^youtu.be";
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QString WBackendYoutube::getHost() const
{
    return "youtube.com";
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QList<WLibraryFolderItem> WBackendYoutube::getLibraryItems() const
{
    WLibraryFolderItem tracks(WLibraryItem::PlaylistSearch);

    tracks.title = tr("Tracks");
    tracks.label = "tracks";

    WLibraryFolderItem channels(WLibraryItem::FolderSearch);

    channels.title = tr("Channels");
    channels.label = "channels";

    WLibraryFolderItem playlists(WLibraryItem::FolderSearch);

    playlists.title = tr("Playlists");
    playlists.label = "playlists";

    QList<WLibraryFolderItem> items;

    items.append(tracks);
    items.append(channels);
    items.append(playlists);

    return items;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QString WBackendYoutube::getTrackId(const QString & url) const
{
    QString source = WControllerNetwork::removeUrlPrefix(url);

    if (source.startsWith("youtu.be"))
    {
        return WControllerNetwork::extractUrlElement(source, 9);
    }
    else if (source.startsWith("youtube.com") == false)
    {
        return QString();
    }

    QRegExp regExp("\\?v=|&v=|/v/|/embed/");

    int index = source.indexOf(regExp);

    if (index != -1)
    {
        index += regExp.matchedLength();

        return WControllerNetwork::extractUrlElement(source, index);
    }
    else return QString();
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */
WBackendNetPlaylistInfo WBackendYoutube::getPlaylistInfo(const QString & url) const
{
    QString source = WControllerNetwork::removeUrlPrefix(url);

    if (source.startsWith("youtube.com") == false)
    {
        return WBackendNetPlaylistInfo();
    }

    QRegExp regExp("/user/|/channel/");

    int index = source.indexOf(regExp);

    if (index == -1)
    {
        regExp.setPattern("\\?p=|&p=|\\?list=|&list=");

        index = source.indexOf(regExp);

        if (index == -1)
        {
            regExp.setPattern("\\?channel_id=|&channel_id=");

            index = source.indexOf(regExp);

            if (index == -1)
            {
                QString id = WControllerNetwork::extractUrlElement(source, 12);

                if (id.isEmpty() || source.indexOf(QRegExp("[/\\?#.]"), 12) != -1)
                {
                     return WBackendNetPlaylistInfo();
                }
                else return WBackendNetPlaylistInfo(WLibraryItem::PlaylistFeed, id);
            }

            index += regExp.matchedLength();

            QString id = "channel/" + WControllerNetwork::extractUrlElement(source, index);

            return WBackendNetPlaylistInfo(WLibraryItem::PlaylistFeed, id);
        }

        index += regExp.matchedLength();

        QString id = WControllerNetwork::extractUrlElement(source, index);

        return WBackendNetPlaylistInfo(WLibraryItem::Playlist, id);
    }
    else
    {
        QString id = WControllerNetwork::extractUrlElement(source, 2, index + 1);

        return WBackendNetPlaylistInfo(WLibraryItem::PlaylistFeed, id);
    }
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */
QString WBackendYoutube::getUrlTrack(const QString & id) const
{
    return "https://www.youtube.com/watch?v=" + id;
}

/* Q_INVOKABLE virtual */
QString WBackendYoutube::getUrlPlaylist(const WBackendNetPlaylistInfo & info) const
{
    if (info.isFeed())
    {
         return "https://www.youtube.com/" + info.id;
    }
    else return "https://www.youtube.com/playlist?list=" + info.id;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */
WBackendNetQuery WBackendYoutube::getQuerySource(const QString & url) const
{
    QString id = getTrackId(url);

    if (id.isEmpty()) return WBackendNetQuery();

    Q_D(const WBackendYoutube);

    QVariantList variants;

    variants.append(id);
    variants.append(d->script);

    WBackendNetQuery query("https://www.youtube.com/get_video_info?video_id=" + id
                           +
                           "&el=detailpage");

    query.data = variants;

    return query;
}

/* Q_INVOKABLE virtual */
WBackendNetQuery WBackendYoutube::getQueryTrack(const QString & url) const
{
    QString id = getTrackId(url);

    if (id.isEmpty())
    {
         return WBackendNetQuery();
    }
    else return WBackendNetQuery("https://www.youtube.com/watch?v=" + id);
}

/* Q_INVOKABLE virtual */
WBackendNetQuery WBackendYoutube::getQueryPlaylist(const QString & url) const
{
    WBackendNetPlaylistInfo info = getPlaylistInfo(url);

    if (info.isValid() == false) return WBackendNetQuery();

    if (info.type == WLibraryItem::PlaylistFeed)
    {
        WBackendNetQuery query("https://www.youtube.com/" + info.id + "/videos?view=0");

        query.id = 1;

        return query;
    }
    else return WBackendNetQuery("https://www.youtube.com/playlist?list=" + info.id);
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */
WBackendNetQuery WBackendYoutube::createQuery(const QString & method,
                                              const QString & label, const QString & q) const
{
    WBackendNetQuery query;

    if (method == "search")
    {
        if (label == "tracks")
        {
            QUrl url("https://www.youtube.com/results");

#ifdef QT_4
            url.addQueryItem("search_query", q);

            url.addQueryItem("filters", "video");
#else
            QUrlQuery urlQuery(url);

            urlQuery.addQueryItem("search_query", q);

            urlQuery.addQueryItem("filters", "video");

            url.setQuery(urlQuery);
#endif

            query.url = url.toString();
            query.id  = 2;
        }
        else if (label == "channels")
        {
            QUrl url("https://www.youtube.com/results");

#ifdef QT_4
            url.addQueryItem("search_query", q);

            url.addQueryItem("filters", "channel");
#else
            QUrlQuery urlQuery(url);

            urlQuery.addQueryItem("search_query", q);

            urlQuery.addQueryItem("filters", "channel");

            url.setQuery(urlQuery);
#endif

            query.url = url.toString();
            query.id  = 1;
        }
        else if (label == "playlists")
        {
            QUrl url("https://www.youtube.com/results");

#ifdef QT_4
            url.addQueryItem("search_query", q);

            url.addQueryItem("filters", "playlist");
#else
            QUrlQuery urlQuery(url);

            urlQuery.addQueryItem("search_query", q);

            urlQuery.addQueryItem("filters", "playlist");

            url.setQuery(urlQuery);
#endif

            query.url = url.toString();
        }
    }
    else if (method == "related" && label == "tracks")
    {
        QUrl url("https://www.youtube.com/watch?v=" + q);

        query.url = url.toString();
        query.id  = 3;
    }

    return query;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */
WBackendNetSource WBackendYoutube::extractSource(const QByteArray       & data,
                                                 const WBackendNetQuery & query) const
{
    WBackendNetSource reply;

    QString content = Sk::readUtf8(data);

    int id = query.id;

    if (id == 2)
    {
        QString javascript = WControllerNetwork::extractJsonHtml(content, "js");

        if (javascript.isEmpty()) return reply;

        javascript = WControllerNetwork::generateUrl(javascript, "https://www.youtube.com");

        QVariantList variants = query.data.toList();

        QStringList script = variants.takeFirst().toStringList();

        if (script.at(0) == javascript)
        {
            Q_D(const WBackendYoutube);

            d->applySignatures(&reply, variants, script.at(1));
        }
        else
        {
            WBackendNetQuery * nextQuery = &(reply.nextQuery);

            nextQuery->url  = javascript;
            nextQuery->id   = 3;
            nextQuery->data = variants;
        }
    }
    else if (id == 3)
    {
        Q_D(const WBackendYoutube);

        int index = content.indexOf("function(a){a=a.split(\"\")");

        QString function = WControllerNetwork::extractScript(content, "function(a)", index);

        QString object = Sk::regExpCap(function, ";([a-zA-Z0-9$]+)\\.", 1);

        object = WControllerNetwork::extractScript(content, "var " + object + '=');

        function.replace("function(a)", "function b(a)");

        QString script = "(function(a){" + object + ';' + function + "return b(a);})";

        QStringList list;

        list.append(query.url);
        list.append(script);

        reply.backup = list;

        d->applySignatures(&reply, query.data.toList(), script);
    }
    else // if (id == 0 || id == 1)
    {
        Q_D(const WBackendYoutube);

        QHash<WAbstractBackend::Quality, QString> * medias = &(reply.medias);
        QHash<WAbstractBackend::Quality, QString> * audios = &(reply.audios);

        QString sources = Sk::sliceIn(content, "url_encoded_fmt_stream_map=", "&");

        sources = WControllerNetwork::decodeUrl(sources);

        QStringList list = sources.split(',');

        foreach (const QString & string, list)
        {
            d->loadMedia(medias, string);
        }

        sources = Sk::sliceIn(content, "adaptive_fmts=", "&");

        sources = WControllerNetwork::decodeUrl(sources);

        list = sources.split(',');

        QString audio;

        for (int i = 0; i < list.length(); i++)
        {
            const QString & string = list.at(i);

            if (string.contains("%22vorbis%22"))
            {
                audio = d->extractUrl(string);

                list.removeAt(i);

                break;
            }
        }

        if (audio.isEmpty())
        {
            for (int i = 0; i < list.length(); i++)
            {
                const QString & string = list.at(i);

                if (string.contains("%22mp4a"))
                {
                    audio = d->extractUrl(string);

                    list.removeAt(i);

                    break;
                }
            }
        }

        foreach (const QString & string, list)
        {
            d->loadAdaptative(medias, audios, string, audio);
        }

        if (medias->isEmpty())
        {
            if (id == 1) return reply;

            QVariantList variants = query.data.toList();

            QString id = variants.first().toString();

            WBackendNetQuery * nextQuery = &(reply.nextQuery);

            nextQuery->url = "https://www.youtube.com/get_video_info?video_id=" + id
                             +
                             "&el=embedded";

            nextQuery->id   = 1;
            nextQuery->data = variants;

            return reply;
        }

        for (int i = 0; i <= WAbstractBackend::Quality2160; i++)
        {
            QString string = medias->value(static_cast<WAbstractBackend::Quality> (i));

            if (string.contains("&s=") == false) continue;

            qDebug("ENCRYPTED");

            QVariantList variants = query.data.toList();

            QString id = variants.takeFirst().toString();

            QStringList listMedias;
            QStringList listAudios;

            for (int i = 0; i <= WAbstractBackend::Quality2160; i++)
            {
                const QString & media = medias->value(static_cast<WAbstractBackend::Quality> (i));
                const QString & audio = audios->value(static_cast<WAbstractBackend::Quality> (i));

                listMedias.append(media);
                listAudios.append(audio);
            }

            variants.append(listMedias);
            variants.append(listAudios);

            WBackendNetQuery * nextQuery = &(reply.nextQuery);

            nextQuery->url  = "https://www.youtube.com/watch?v=" + id;
            nextQuery->id   = 2;
            nextQuery->data = variants;

            break;
        }
    }

    return reply;
}

/* Q_INVOKABLE virtual */
WBackendNetTrack WBackendYoutube::extractTrack(const QByteArray       & data,
                                               const WBackendNetQuery &) const
{
    WBackendNetTrack reply;

    QString content = Sk::readUtf8(data);

    int index = content.indexOf("=\"og:image");

    QString cover = WControllerNetwork::extractAttribute(content, "content", index);

    cover.replace("https://", "http://");

    QString json = Sk::slice(content, "\\\"videoDetails\\\"", "\\\"playerConfig\\\"");

    json.replace("\\\"", "\"");
    json.replace("\\\\", "\\");

    QString title = WControllerNetwork::extractJsonUtf8(json, "title");

    QString author = WControllerNetwork::extractJsonUtf8(json, "author");

    QString duration = WControllerNetwork::extractJson(json, "lengthSeconds");

    QString quality = WControllerNetwork::extractJson(content, "fmt_list");

    index = content.indexOf("itemprop=\"datePublished");

    QString date = WControllerNetwork::extractAttribute(content, "content", index);

    index = content.indexOf("class=\"yt-user-info", index);

    QString feed = WControllerNetwork::extractAttribute(content, "href", index);

    feed = "https://www.youtube.com/" + WControllerNetwork::extractUrlPath(feed);

    WTrack * track = &(reply.track);

    track->setTitle(title);
    track->setCover(cover);

    track->setAuthor(author);
    track->setFeed  (feed);

    track->setDuration(duration.toInt() * 1000);

    track->setDate(QDateTime::fromString(date, "yyyy-MM-dd"));

    if (quality.contains("1280"))
    {
         track->setQuality(WAbstractBackend::Quality720);
    }
    else track->setQuality(WAbstractBackend::Quality480);

    return reply;
}

/* Q_INVOKABLE virtual */
WBackendNetPlaylist WBackendYoutube::extractPlaylist(const QByteArray       & data,
                                                     const WBackendNetQuery & query) const
{
    WBackendNetPlaylist reply;

    QString content = Sk::readUtf8(data);

    int id = query.id;

    if (id == 1) // Feed
    {
        Q_D(const WBackendYoutube);

        if (query.data.toInt() == 0)
        {
            int index = content.indexOf("class=\"channel-header-profile-image");

            QString cover = WControllerNetwork::extractAttribute(content, "src", index);

            cover = WControllerNetwork::generateUrl(cover, "https://www.youtube.com");

            QString title = WControllerNetwork::extractAttributeUtf8(content, "title", index);

            reply.title = title;
            reply.cover = cover;

            index = content.indexOf("data-uix-load-more-href=\"", index);

            if (index != -1)
            {
                QString data = WControllerNetwork::extractAttribute(content,
                                                                    "data-uix-load-more-href",
                                                                    index);

                QString url("https://www.youtube.com" + WControllerNetwork::decodeUrl(data));

                WBackendNetQuery * nextQuery = &(reply.nextQuery);

                nextQuery->url  = url;
                nextQuery->id   = 1;
                nextQuery->data = 1;
            }
        }
        else
        {
            content = WControllerNetwork::extractJsonHtml(content, "content_html");

            content = WControllerNetwork::htmlToUtf8(content);
        }

        QStringList list = Sk::slices(content, "<li class=\"channels-content-item",
                                               "<div class=\"yt-lockup-meta");

        d->loadTracks(&(reply.tracks), list, "class=\"yt-lockup-title");
    }
    else if (id == 2) // Search tracks
    {
        Q_D(const WBackendYoutube);

        QStringList list = Sk::slices(content, "<div class=\"yt-lockup yt-lockup-tile",
                                               "</div></li>");

        d->loadTracks(&(reply.tracks), list, "class=\"yt-lockup-title");

        if (query.data.toInt() == 0)
        {
            QString url = d->getNextUrl(content);

            if (url.isEmpty()) return reply;

            WBackendNetQuery * nextQuery = &(reply.nextQuery);

            nextQuery->url  = url;
            nextQuery->id   = 2;
            nextQuery->data = 1;
        }
    }
    else if (id == 3) // Related tracks
    {
        Q_D(const WBackendYoutube);

        if (query.data.toInt() == 0)
        {
            int index = content.indexOf("\\\"continuation\\\"");

            QString json = Sk::sliceIn(content, "\\\"continuation\\\":\\\"", "\\\"", index + 1);

            if (json.isEmpty() == false)
            {
                QString url("https://www.youtube.com/related_ajax?continuation="
                            +
                            WControllerNetwork::decodeUrl(json));

                WBackendNetQuery * nextQuery = &(reply.nextQuery);

                nextQuery->url  = url;
                nextQuery->id   = 3;
                nextQuery->data = 1;
            }
        }
        else
        {
            int index = content.indexOf("\"body\"");

            content = WControllerNetwork::extractJsonHtml(content, "watch-more-related", index);

            content = WControllerNetwork::htmlToUtf8(content);
        }

        QStringList list = Sk::slices(content, "<li class=\"video-list-item", "</li>");

        d->loadTracks(&(reply.tracks), list, "class=\"content-wrapper");
    }
    else // Playlist
    {
        if (query.data.toInt() == 0)
        {
            int index = content.indexOf("<meta name=\"title");

            QString title = WControllerNetwork::extractAttributeUtf8(content, "content", index);

            reply.title = title;

            if (index != -1)
            {
                QString data = WControllerNetwork::extractAttribute(content,
                                                                    "data-uix-load-more-href",
                                                                    index);

                QString url("https://www.youtube.com" + WControllerNetwork::decodeUrl(data));

                WBackendNetQuery * nextQuery = &(reply.nextQuery);

                nextQuery->url  = url;
                nextQuery->data = 1;
            }
        }
        else
        {
            content = WControllerNetwork::extractJsonHtml(content, "content_html");

            content = WControllerNetwork::htmlToUtf8(content);
        }

        QStringList list = Sk::slices(content, "<tr class=\"pl-video yt-uix-tile", "</td></tr>");

        foreach (const QString & string, list)
        {
            QString id = WControllerNetwork::extractAttribute(string, "data-video-id");

            QString title = WControllerNetwork::extractAttributeUtf8(string, "data-title");

            WTrack track("https://www.youtube.com/watch?v=" + id, WTrack::Default);

            track.setTitle(title);

            track.setCover("http://i.ytimg.com/vi/" + id + "/hqdefault.jpg");

            reply.tracks.append(track);
        }
    }

    return reply;
}

/* Q_INVOKABLE virtual */
WBackendNetFolder WBackendYoutube::extractFolder(const QByteArray       & data,
                                                 const WBackendNetQuery & query) const
{
    Q_D(const WBackendYoutube);

    WBackendNetFolder reply;

    QString content = Sk::readUtf8(data);

    int id = query.id;

    if (id == 1) // Search channels
    {
        QStringList list = Sk::slices(content,
                                      "<div class=\"yt-lockup yt-lockup-tile yt-lockup-channel",
                                      "</div></li>");

        foreach (const QString & string, list)
        {
            int index = string.indexOf("class=\"yt-thumb video-thumb");

            QString cover = d->extractPlaylistCover(string, index);

            // FIXME Youtube
            cover.replace("http://", "https://");

            index = string.indexOf("class=\"yt-lockup-title", index);

            QString source = WControllerNetwork::extractAttributeUtf8(string, "href", index);

            QString title = WControllerNetwork::extractAttributeUtf8(string, "title", index);

            WLibraryFolderItem playlist(WLibraryItem::PlaylistFeed, WLocalObject::Default);

            playlist.source = "https://www.youtube.com" + source + "/videos?view=0";

            playlist.title = title;
            playlist.cover = cover;

            reply.items.append(playlist);
        }
    }
    else // Search playlists
    {
        QStringList list = Sk::slices(content,
                                      "<div class=\"yt-lockup yt-lockup-tile yt-lockup-playlist",
                                      "</div></li>");

        foreach (const QString & string, list)
        {
            int index = string.indexOf("class=\"yt-thumb video-thumb");

            QString cover = d->extractPlaylistCover(string, index);

            index = string.indexOf("class=\"yt-lockup-title", index);

            QString id = WControllerNetwork::extractAttributeUtf8(string, "href", index);

            id = WControllerNetwork::extractUrlValue(id, "list");

            QString title = WControllerNetwork::extractAttributeUtf8(string, "title", index);

            WLibraryFolderItem playlist(WLibraryItem::Playlist, WLocalObject::Default);

            playlist.source = "https://www.youtube.com/playlist?list=" + id;

            playlist.title = title;
            playlist.cover = cover;

            reply.items.append(playlist);
        }
    }

    if (query.data.toInt() == 0)
    {
        QString url = d->getNextUrl(content);

        if (url.isEmpty()) return reply;

        WBackendNetQuery * nextQuery = &(reply.nextQuery);

        nextQuery->url  = url;
        nextQuery->id   = id;
        nextQuery->data = 1;
    }

    return reply;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ void WBackendYoutube::applySource(const WBackendNetQuery  & query,
                                                            const WBackendNetSource & source)
{
    if (query.id == 3)
    {
        Q_D(WBackendYoutube);

        d->script = source.backup.toStringList();
    }
}

#endif // SK_NO_BACKENDYOUTUBE
