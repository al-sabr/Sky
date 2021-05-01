//=================================================================================================
/*
    Copyright (C) 2015-2020 Sky kit authors. <http://omega.gg/Sky>

    Author: Benjamin Arnaud. <http://bunjee.me> <bunjee@omega.gg>

    This file is part of SkCore.

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

#ifndef SK_H_
#define SK_H_

#include <QtGlobal>
#include <SkyCoreExports.h>

class WPrivate;

//-------------------------------------------------------------------------------------------------

#define W_DECLARE_PRIVATE_COPY(Class)                       \
                                                            \
inline Class##Private * d_func()                            \
{                                                           \
    return reinterpret_cast<Class##Private *> (sk_d);       \
}                                                           \
                                                            \
inline const Class##Private * d_func() const                \
{                                                           \
    return reinterpret_cast<const Class##Private *> (sk_d); \
}                                                           \
                                                            \
friend class Class##Private;                                \

#define W_DECLARE_PRIVATE(Class) \
                                 \
Q_DISABLE_COPY        (Class)    \
W_DECLARE_PRIVATE_COPY(Class)    \

//-------------------------------------------------------------------------------------------------

#define C_STR toLatin1().constData()
#define C_UTF toUtf8  ().constData()

#define C_URL toString().C_STR

//-------------------------------------------------------------------------------------------------

/**
 * \internal
 * @class WPrivatable
 */
class SKY_CORE_EXPORT WPrivatable
{
protected:
    WPrivatable(WPrivate * p);

    virtual ~WPrivatable();

    WPrivate * sk_d;
};

#endif // SK_H_
