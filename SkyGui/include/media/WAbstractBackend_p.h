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

#ifndef WABSTRACTBACKEND_P_H
#define WABSTRACTBACKEND_P_H

/*  W A R N I N G
    -------------

    This file is not part of the Sk API. It exists purely as an
    implementation detail. This header file may change from version to
    version without notice, or even be removed.

    We mean it.
*/

#include <private/Sk_p>
#include <SkyGuiExports.h>

#ifndef SK_NO_ABSTRACTBACKEND

class SKY_GUI_EXPORT WAbstractBackendPrivate : public WPrivate
{
public:
    WAbstractBackendPrivate(WAbstractBackend * p);

    void init();

public: // Functions
    void clearCurrentTime();

    void setStarted(bool started);

public: // Variable
    WDeclarativePlayer * parentItem;

    WBackendFilter * filter;

    QString source;

    WAbstractBackend::State     state;
    WAbstractBackend::StateLoad stateLoad;

    bool started;
    bool ended;

    int currentTime;
    int duration;

    qreal progress;

    qreal speed;

    qreal volume;

    bool repeat;

    WAbstractBackend::Output  output;
    WAbstractBackend::Quality quality;

    WAbstractBackend::Output  outputActive;
    WAbstractBackend::Quality qualityActive;

    WAbstractBackend::FillMode fillMode;

    QSizeF size;

    bool deleting;

protected:
    W_DECLARE_PUBLIC(WAbstractBackend)
};

#endif // SK_NO_ABSTRACTBACKEND
#endif // WABSTRACTBACKEND_P_H
