//=================================================================================================
/*
    Copyright (C) 2015-2020 Sky kit authors. <http://omega.gg/Sky>

    Author: Benjamin Arnaud. <http://bunjee.me> <bunjee@omega.gg>

    This file is part of SkyTouch.

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

import QtQuick 1.0
import Sky     1.0

Slider
{
    id: sliderStream

    //---------------------------------------------------------------------------------------------
    // Properties
    //---------------------------------------------------------------------------------------------

    property bool active: true

    property int currentTime: -1
    property int duration   : -1

    property real progress: 0.0

    //---------------------------------------------------------------------------------------------
    // Style

    property int durationAnimation: st.sliderStream_durationAnimation

    property int intervalProgress: st.sliderStream_intervalProgress

    property real opacityProgressA: st.sliderStream_opacityProgressA
    property real opacityProgressB: st.sliderStream_opacityProgressB

    //---------------------------------------------------------------------------------------------
    // Private

    property int pState: 0

    property int pProgressWidth: (background.width - margins * 2) * pProgress

    property real pProgress: 0.0

    //---------------------------------------------------------------------------------------------
    // Aliases
    //---------------------------------------------------------------------------------------------
    // Style

    property alias colorProgress: itemProgress.color

    //---------------------------------------------------------------------------------------------
    // Settings
    //---------------------------------------------------------------------------------------------

    enabled: (duration > 0)

    // NOTE: We want the background to be behind the itemProgress.
    background.z: -1

    foreground.opacity: (pState) ? opacityProgressA
                                 : opacityProgressB

    //---------------------------------------------------------------------------------------------
    // Style

    colorFront: (active) ? st.sliderStream_colorActive
                         : st.slider_colorFront

    //---------------------------------------------------------------------------------------------
    // Events
    //---------------------------------------------------------------------------------------------

    onCurrentTimeChanged: pUpdate()
    onDurationChanged   : pUpdate()

    onProgressChanged: pUpdateProgress()

    onPStateChanged: pUpdateProgress()

    //---------------------------------------------------------------------------------------------
    // Animations
    //---------------------------------------------------------------------------------------------

    Behavior on pProgress
    {
        id: behaviorProgress

        enabled: false

        PropertyAnimation
        {
            duration: durationAnimation

            easing.type: st.easing
        }
    }

    //---------------------------------------------------------------------------------------------
    // Functions
    //---------------------------------------------------------------------------------------------
    // Private

    function pUpdate()
    {
        maximum = duration;

        if (slider.pressed == false && slider.drag.active == false)
        {
            value = currentTime;
        }
    }

    function pUpdateProgress()
    {
        if (progress == 0.0)
        {
            pState = 0;

            if (pProgress != 1.0)
            {
                pProgress = progress;
            }

            return;
        }
        else if (progress == -1.0)
        {
            pState = 1;

            pProgress = 1.0;

            return;
        }

        if (progress == 1.0)
        {
             pState = 0;
        }
        else pState = 2;

        behaviorProgress.enabled = true;

        pProgress = progress;

        behaviorProgress.enabled = false;
    }

    //---------------------------------------------------------------------------------------------
    // Childs
    //---------------------------------------------------------------------------------------------

    Rectangle
    {
        id: itemProgress

        anchors.left  : background.left
        anchors.top   : background.top
        anchors.bottom: background.bottom

        anchors.margins: margins

        width: Math.max(st.dp32, pProgressWidth)

        radius: sliderStream.radius

        z: -1

        visible: (opacity != 0.0)
        opacity: 0.0

        color: st.slider_colorFront

//#QT_4
        smooth: true
//#END

        states:
        [
            State { name: "buffering"; when: (pState == 1) },
            State { name: "loading";   when: (pState == 2) }
        ]

        transitions: Transition
        {
            SequentialAnimation
            {
                PauseAnimation { duration: durationAnimation }

                ScriptAction
                {
                    script:
                    {
                        if (pState == 0)
                        {
                            behaviorOpacity.enabled = true;

                            itemProgress.opacity = 0.0;

                            behaviorOpacity.enabled = false;
                        }
                        else if (pState == 1)
                        {
                            animationBuffer.running = true;
                        }
                        else // if (pState == 2)
                        {
                            animationLoad.running = true;
                        }
                    }
                }
            }
        }

        onStateChanged:
        {
            animationBuffer.running = false;
            animationLoad  .running = false;

            if (pState)
            {
                behaviorOpacity.enabled = true;

                itemProgress.opacity = opacityProgressB;

                behaviorOpacity.enabled = false;
            }
            else if (pProgress == 0)
            {
                itemProgress.opacity = 0;
            }
        }

        Behavior on opacity
        {
            id: behaviorOpacity

            enabled: false

            PropertyAnimation
            {
                duration: durationAnimation

                easing.type: st.easing
            }
        }

        SequentialAnimation on opacity
        {
            id: animationBuffer

            running: false

            loops: Animation.Infinite

            PropertyAnimation
            {
                to: 0.0

                duration: intervalProgress

                easing.type: st.easing
            }

            PropertyAnimation
            {
                to: opacityProgressB

                duration: intervalProgress

                easing.type: st.easing
            }
        }

        SequentialAnimation on opacity
        {
            id: animationLoad

            running: false

            loops: Animation.Infinite

            PropertyAnimation
            {
                to: opacityProgressA

                duration: intervalProgress

                easing.type: st.easing
            }

            PropertyAnimation
            {
                to: opacityProgressB

                duration: intervalProgress

                easing.type: st.easing
            }
        }
    }
}
