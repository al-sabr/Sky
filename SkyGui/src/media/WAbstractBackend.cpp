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

#include "WAbstractBackend.h"

#ifndef SK_NO_ABSTRACTBACKEND

// Sk includes
#include <WControllerPlaylist>

#ifdef QT_LATEST

//=================================================================================================
// WBackendNode
//=================================================================================================

WBackendNode::WBackendNode() : _geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
{
    _source = QRectF(0.0, 0.0, 1.0, 1.0);

    setGeometry(&_geometry);
}

//-------------------------------------------------------------------------------------------------
// Interface
//-------------------------------------------------------------------------------------------------

void WBackendNode::setRect(const QRectF & rect)
{
    QSGGeometry::updateTexturedRectGeometry(&_geometry, rect, _source);

    markDirty(QSGNode::DirtyGeometry);
}

#endif

//=================================================================================================
// WAbstractBackendPrivate
//=================================================================================================

#include "WAbstractBackend_p.h"

WAbstractBackendPrivate::WAbstractBackendPrivate(WAbstractBackend * p) : WPrivate(p) {}

void WAbstractBackendPrivate::init()
{
    parentItem = NULL;

    filter = NULL;

    state     = WAbstractBackend::StateStopped;
    stateLoad = WAbstractBackend::StateLoadDefault;

    started = false;
    ended   = false;

    currentTime = -1;
    duration    = -1;

    progress = 0.0;

    speed = 1.0;

    volume = 1.0;

    repeat = false;

    output  = WAbstractBackend::OutputMedia;
    quality = WAbstractBackend::Quality720;

    outputActive  = WAbstractBackend::OutputInvalid;
    qualityActive = WAbstractBackend::QualityInvalid;

    fillMode = WAbstractBackend::PreserveAspectFit;

    deleting = false;
}

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------

void WAbstractBackendPrivate::clearCurrentTime()
{
    if (currentTime == -1) return;

    Q_Q(WAbstractBackend);

    currentTime = -1;

    emit q->currentTimeChanged();
}

//-------------------------------------------------------------------------------------------------

void WAbstractBackendPrivate::setStarted(bool started)
{
    if (this->started == started) return;

    Q_Q(WAbstractBackend);

    this->started = started;

    emit q->startedChanged();
}

//=================================================================================================
// WAbstractBackend
//=================================================================================================

WAbstractBackend::WAbstractBackend()
    : QObject(), WPrivatable(new WAbstractBackendPrivate(this))
{
    Q_D(WAbstractBackend); d->init();
}

//-------------------------------------------------------------------------------------------------
// Protected

WAbstractBackend::WAbstractBackend(WAbstractBackendPrivate * p)
    : QObject(), WPrivatable(p)
{
    Q_D(WAbstractBackend); d->init();
}

//-------------------------------------------------------------------------------------------------
// Interface
//-------------------------------------------------------------------------------------------------

#ifdef QT_LATEST

/* Q_INVOKABLE */ WBackendNode * WAbstractBackend::createNode() const
{
    return backendCreateNode();
}

#endif

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE */ const QSizeF & WAbstractBackend::getSize() const
{
    Q_D(const WAbstractBackend); return d->size;
}

/* Q_INVOKABLE */ void WAbstractBackend::setSize(const QSizeF & size)
{
    Q_D(WAbstractBackend);

    if (d->size == size) return;

    d->size = size;

    backendSetSize(size);
}

//-------------------------------------------------------------------------------------------------

#ifdef QT_LATEST

/* Q_INVOKABLE */ void WAbstractBackend::synchronize(WBackendFrame * frame)
{
    backendSynchronize(frame);
}

#endif

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE */ void WAbstractBackend::drawFrame(QPainter * painter, const QRect & rect)
{
    backendDrawFrame(painter, rect);
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE */ void WAbstractBackend::updateFrame()
{
    backendUpdateFrame();
}

/* Q_INVOKABLE */ QImage WAbstractBackend::getFrame() const
{
    return backendGetFrame();
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE */ QRectF WAbstractBackend::getRect() const
{
    return backendRect();
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE */ bool WAbstractBackend::deleteBackend()
{
    Q_D(WAbstractBackend);

    if (d->deleting) return false;

    if (backendDelete())
    {
        delete this;

        return true;
    }
    else
    {
        d->deleting = true;

        setParent    (NULL);
        setParentItem(NULL);

        setState(StateStopped);

        return false;
    }
}

//-------------------------------------------------------------------------------------------------
// WBackendInterface implementation
//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ QString WAbstractBackend::source() const
{
    Q_D(const WAbstractBackend); return d->source;
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ bool WAbstractBackend::sourceIsVideo() const
{
    Q_D(const WAbstractBackend);

    return wControllerPlaylist->sourceIsVideo(d->source);
}

/* Q_INVOKABLE virtual */ bool WAbstractBackend::sourceIsAudio() const
{
    Q_D(const WAbstractBackend);

    return wControllerPlaylist->sourceIsAudio(d->source);
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ void WAbstractBackend::loadSource(const QString & url, int duration,
                                                                                 int currentTime)
{
    Q_D(WAbstractBackend);

    if (d->source != url)
    {
        if (d->state == StatePaused || url.isEmpty()) stop();

        d->source = url;

        setDuration   (duration);
        setCurrentTime(currentTime);

        backendSetSource(url);

        emit sourceChanged();
    }
    else if (d->state == StatePlaying)
    {
        backendPause();

        setDuration   (duration);
        setCurrentTime(currentTime);

        setStateLoad(StateLoadResuming);

        backendPlay();
    }
    else
    {
        if (d->state == StatePaused) stop();

        setDuration   (duration);
        setCurrentTime(currentTime);
    }
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ void WAbstractBackend::play()
{
    Q_D(WAbstractBackend);

    if (d->state == StatePlaying || d->source.isEmpty()) return;

    if (backendPlay()) setState(StatePlaying);
}

/* Q_INVOKABLE virtual */ void WAbstractBackend::replay()
{
    Q_D(WAbstractBackend);

    if (d->source.isEmpty()) return;

    d->clearCurrentTime();

    if (backendPlay()) setState(StatePlaying);
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ void WAbstractBackend::pause()
{
    Q_D(WAbstractBackend);

    if (d->state != StatePlaying) return;

    if (backendPause()) setState(StatePaused);
}

/* Q_INVOKABLE virtual */ void WAbstractBackend::stop()
{
    Q_D(WAbstractBackend);

    if (d->state == StateStopped) return;

    if (backendStop()) setState(StateStopped);
}

/* Q_INVOKABLE virtual */ void WAbstractBackend::clear()
{
    loadSource(QString());
}

//-------------------------------------------------------------------------------------------------

/* Q_INVOKABLE virtual */ void WAbstractBackend::seek(int msec)
{
    Q_D(WAbstractBackend);

    if (d->currentTime == msec) return;

    d->currentTime = msec;

    if (d->started) backendSeek(msec);

    emit currentTimeChanged();
}

//-------------------------------------------------------------------------------------------------
// Protected functions
//-------------------------------------------------------------------------------------------------

void WAbstractBackend::setState(State state)
{
    Q_D(WAbstractBackend);

    if (d->filter) d->filter->filterState(&state);

    if (d->state == state) return;

    d->state = state;

    if (state == StateStopped)
    {
        setStateLoad(StateLoadDefault);

        d->setStarted(false);

        d->clearCurrentTime();
    }
    else if (state == StatePlaying)
    {
        d->setStarted(true);
    }
    else setStateLoad(StateLoadDefault);

    emit stateChanged();
}

void WAbstractBackend::setStateLoad(StateLoad stateLoad)
{
    Q_D(WAbstractBackend);

    if (d->filter) d->filter->filterStateLoad(&stateLoad);

    if (d->stateLoad == stateLoad) return;

    d->stateLoad = stateLoad;

    emit stateLoadChanged();
}

//-------------------------------------------------------------------------------------------------

void WAbstractBackend::setEnded(bool ended)
{
    Q_D(WAbstractBackend);

    if (d->filter) d->filter->filterEnded(&ended);

    if (d->ended == ended) return;

    if (ended)
    {
        if (d->repeat)
        {
            d->clearCurrentTime();

            backendPlay();
        }
        else
        {
            d->ended = true;

            d->clearCurrentTime();

            emit endedChanged();

            emit this->ended();
        }
    }
    else
    {
        d->ended = false;

        emit endedChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void WAbstractBackend::setCurrentTime(int msec)
{
    Q_D(WAbstractBackend);

    if (d->filter) d->filter->filterCurrentTime(&msec);

    if (d->currentTime == msec) return;

    d->currentTime = msec;

    if (msec != -1) setEnded(false);

    emit currentTimeChanged();
}

void WAbstractBackend::setDuration(int msec)
{
    Q_D(WAbstractBackend);

    if (d->filter) d->filter->filterDuration(&msec);

    if (d->duration == msec) return;

    d->duration = msec;

    emit durationChanged();
}

//-------------------------------------------------------------------------------------------------

void WAbstractBackend::setProgress(qreal progress)
{
    Q_D(WAbstractBackend);

    if (d->filter) d->filter->filterProgress(&progress);

    if (d->progress == progress) return;

    d->progress = progress;

    emit progressChanged();
}

//-------------------------------------------------------------------------------------------------

void WAbstractBackend::setOutputActive(Output output)
{
    Q_D(WAbstractBackend);

    if (d->filter) d->filter->filterOutputActive(&output);

    if (d->outputActive == output) return;

    d->outputActive = output;

    emit outputActiveChanged();
}

void WAbstractBackend::setQualityActive(Quality quality)
{
    Q_D(WAbstractBackend);

    if (d->filter) d->filter->filterQualityActive(&quality);

    if (d->qualityActive == quality) return;

    d->qualityActive = quality;

    emit qualityActiveChanged();
}

//-------------------------------------------------------------------------------------------------

void WAbstractBackend::deleteNow()
{
    Q_D(WAbstractBackend);

    if (d->deleting) delete this;
}

//-------------------------------------------------------------------------------------------------
// Protected virtual functions
//-------------------------------------------------------------------------------------------------

/* virtual */ void WAbstractBackend::backendSeek(int)
{
    qWarning("WAbstractBackend::backendSeek: Not supported.");
}

//-------------------------------------------------------------------------------------------------

/* virtual */ void WAbstractBackend::backendSetSpeed(qreal)
{
    qWarning("WAbstractBackend::backendSetSpeed: Not supported.");
}

//-------------------------------------------------------------------------------------------------

/* virtual */ void WAbstractBackend::backendSetOutput(Output)
{
    qWarning("WAbstractBackend::backendSetOutput: Not supported.");
}

/* virtual */ void WAbstractBackend::backendSetQuality(Quality)
{
    qWarning("WAbstractBackend::backendSetQuality: Not supported.");
}

//-------------------------------------------------------------------------------------------------

/* virtual */ void WAbstractBackend::backendSetFillMode(FillMode)
{
    qWarning("WAbstractBackend::backendSetFillMode: Not supported.");
}

//-------------------------------------------------------------------------------------------------

/* virtual */ void WAbstractBackend::backendSetSize(const QSizeF &)
{
    qWarning("WAbstractBackend::backendSetSize: Not supported.");
}

//-------------------------------------------------------------------------------------------------

#ifdef QT_LATEST

/* virtual */ void WAbstractBackend::backendSynchronize(WBackendFrame *)
{
    qWarning("WAbstractBackend::backendSynchronize: Not supported.");
}

#endif

//-------------------------------------------------------------------------------------------------

/* virtual */ void WAbstractBackend::backendDrawFrame(QPainter *, const QRect &)
{
    qWarning("WAbstractBackend::backendDrawFrame: Not supported.");
}

//-------------------------------------------------------------------------------------------------

/* virtual */ void WAbstractBackend::backendUpdateFrame()
{
    qWarning("WAbstractBackend::backendUpdateFrame: Not supported.");
}

/* virtual */ QImage WAbstractBackend::backendGetFrame() const
{
    qWarning("WAbstractBackend::backendGetFrame: Not supported.");

    return QImage();
}

//-------------------------------------------------------------------------------------------------

/* virtual */ QRectF WAbstractBackend::backendRect() const
{
    qWarning("WAbstractBackend::backendRect: Not supported.");

    return QRectF();
}

//-------------------------------------------------------------------------------------------------
// Properties
//-------------------------------------------------------------------------------------------------

WDeclarativePlayer * WAbstractBackend::parentItem() const
{
    Q_D(const WAbstractBackend); return d->parentItem;
}

void WAbstractBackend::setParentItem(WDeclarativePlayer * parent)
{
    Q_D(WAbstractBackend);

    if (d->parentItem == parent) return;

    d->parentItem = parent;

    emit parentItemChanged();
}

//-------------------------------------------------------------------------------------------------

WBackendFilter * WAbstractBackend::filter() const
{
    Q_D(const WAbstractBackend); return d->filter;
}

void WAbstractBackend::setFilter(WBackendFilter * filter)
{
    Q_D(WAbstractBackend);

    if (d->filter == filter) return;

    d->filter = filter;

    emit filterChanged();
}

//-------------------------------------------------------------------------------------------------

void WAbstractBackend::setSource(const QString & url)
{
    loadSource(url);
}

//-------------------------------------------------------------------------------------------------

WAbstractBackend::State WAbstractBackend::state() const
{
    Q_D(const WAbstractBackend); return d->state;
}

WAbstractBackend::StateLoad WAbstractBackend::stateLoad() const
{
    Q_D(const WAbstractBackend); return d->stateLoad;
}

//-------------------------------------------------------------------------------------------------

bool WAbstractBackend::isLoading() const
{
    Q_D(const WAbstractBackend); return (d->stateLoad != StateLoadDefault);
}

//-------------------------------------------------------------------------------------------------

bool WAbstractBackend::isDefault() const
{
    Q_D(const WAbstractBackend); return (d->stateLoad == StateLoadDefault);
}

bool WAbstractBackend::isStarting() const
{
    Q_D(const WAbstractBackend); return (d->stateLoad == StateLoadStarting);
}

bool WAbstractBackend::isResuming() const
{
    Q_D(const WAbstractBackend); return (d->stateLoad == StateLoadResuming);
}

bool WAbstractBackend::isBuffering() const
{
    Q_D(const WAbstractBackend); return (d->stateLoad == StateLoadBuffering);
}

//-------------------------------------------------------------------------------------------------

bool WAbstractBackend::isPlaying() const
{
    Q_D(const WAbstractBackend); return (d->state == StatePlaying);
}

bool WAbstractBackend::isPaused() const
{
    Q_D(const WAbstractBackend); return (d->state == StatePaused);
}

bool WAbstractBackend::isStopped() const
{
    Q_D(const WAbstractBackend); return (d->state == StateStopped);
}

//-------------------------------------------------------------------------------------------------

bool WAbstractBackend::hasStarted() const
{
    Q_D(const WAbstractBackend); return d->started;
}

bool WAbstractBackend::hasEnded() const
{
    Q_D(const WAbstractBackend); return d->ended;
}

//-------------------------------------------------------------------------------------------------

int WAbstractBackend::currentTime() const
{
    Q_D(const WAbstractBackend); return d->currentTime;
}

int WAbstractBackend::duration() const
{
    Q_D(const WAbstractBackend); return d->duration;
}

//-------------------------------------------------------------------------------------------------

qreal WAbstractBackend::progress() const
{
    Q_D(const WAbstractBackend); return d->progress;
}

//-------------------------------------------------------------------------------------------------

qreal WAbstractBackend::speed() const
{
    Q_D(const WAbstractBackend); return d->speed;
}

void WAbstractBackend::setSpeed(qreal speed)
{
    Q_D(WAbstractBackend);

    if (d->filter) d->filter->filterSpeed(&speed);

    if (d->speed == speed) return;

    d->speed = speed;

    backendSetSpeed(speed);

    emit speedChanged();
}

//-------------------------------------------------------------------------------------------------

qreal WAbstractBackend::volume() const
{
    Q_D(const WAbstractBackend); return d->volume;
}

void WAbstractBackend::setVolume(qreal volume)
{
    Q_D(WAbstractBackend);

    if (d->filter) d->filter->filterVolume(&volume);

    if (d->volume == volume) return;

    d->volume = volume;

    backendSetVolume(volume);

    emit volumeChanged();
}

//-------------------------------------------------------------------------------------------------

bool WAbstractBackend::repeat() const
{
    Q_D(const WAbstractBackend); return d->repeat;
}

void WAbstractBackend::setRepeat(bool repeat)
{
    Q_D(WAbstractBackend);

    if (d->repeat == repeat) return;

    d->repeat = repeat;

    emit repeatChanged();
}

//-------------------------------------------------------------------------------------------------

WAbstractBackend::Output WAbstractBackend::output() const
{
    Q_D(const WAbstractBackend); return d->output;
}

void WAbstractBackend::setOutput(Output output)
{
    Q_D(WAbstractBackend);

    if (d->filter) d->filter->filterOutput(&output);

    if (d->output == output) return;

    d->output = output;

    backendSetOutput(output);

    emit outputChanged();
}

WAbstractBackend::Quality WAbstractBackend::quality() const
{
    Q_D(const WAbstractBackend); return d->quality;
}

void WAbstractBackend::setQuality(Quality quality)
{
    Q_D(WAbstractBackend);

    if (d->filter) d->filter->filterQuality(&quality);

    if (d->quality == quality) return;

    d->quality = quality;

    backendSetQuality(quality);

    emit qualityChanged();
}

//-------------------------------------------------------------------------------------------------

WAbstractBackend::Output WAbstractBackend::outputActive() const
{
    Q_D(const WAbstractBackend); return d->outputActive;
}

WAbstractBackend::Quality WAbstractBackend::qualityActive() const
{
    Q_D(const WAbstractBackend); return d->qualityActive;
}

//-------------------------------------------------------------------------------------------------

WAbstractBackend::FillMode WAbstractBackend::fillMode() const
{
    Q_D(const WAbstractBackend); return d->fillMode;
}

void WAbstractBackend::setFillMode(FillMode fillMode)
{
    Q_D(WAbstractBackend);

    if (d->filter) d->filter->filterFillMode(&fillMode);

    if (d->fillMode == fillMode) return;

    d->fillMode = fillMode;

    backendSetFillMode(fillMode);

    emit fillModeChanged();
}

//=================================================================================================
// WBackendFilter
//=================================================================================================

/* virtual */ void WBackendFilter::filterState    (WAbstractBackend::State     *) {}
/* virtual */ void WBackendFilter::filterStateLoad(WAbstractBackend::StateLoad *) {}

/* virtual */ void WBackendFilter::filterEnded(bool *) {}

/* virtual */ void WBackendFilter::filterCurrentTime(int *) {}
/* virtual */ void WBackendFilter::filterDuration   (int *) {}

/* virtual */ void WBackendFilter::filterProgress(qreal *) {}

/* virtual */ void WBackendFilter::filterOutput      (WAbstractBackend::Output  *) {}
/* virtual */ void WBackendFilter::filterOutputActive(WAbstractBackend::Output  *) {}

/* virtual */ void WBackendFilter::filterQuality      (WAbstractBackend::Quality *) {}
/* virtual */ void WBackendFilter::filterQualityActive(WAbstractBackend::Quality *) {}

/* virtual */ void WBackendFilter::filterSpeed(qreal *) {}

/* virtual */ void WBackendFilter::filterVolume(qreal *) {}

/* virtual */ void WBackendFilter::filterFillMode(WAbstractBackend::FillMode *) {}

#endif // SK_NO_ABSTRACTBACKEND
