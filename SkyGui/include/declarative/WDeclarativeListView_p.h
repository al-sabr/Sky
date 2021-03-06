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

#ifndef WDECLARATIVELISTVIEW_P_H
#define WDECLARATIVELISTVIEW_P_H

/*  W A R N I N G
    -------------

    This file is not part of the Sk API. It exists purely as an
    implementation detail. This header file may change from version to
    version without notice, or even be removed.

    We mean it.
*/

// Qt includes
#include <QTimer>
#include <QAbstractItemModel>

// Private includes
#include <private/declarative/WDeclarativeItem_p>
#include <SkyGuiExports.h>

#ifndef SK_NO_DECLARATIVELISTVIEW

// Forward declarations
#ifdef QT_4
class QDeclarativeItem;
#else
class QQuickItem;
#endif

//-------------------------------------------------------------------------------------------------
// WDeclarativeListItem
//-------------------------------------------------------------------------------------------------

class SKY_GUI_EXPORT WDeclarativeListItem{
public:
    WDeclarativeListItem();

    virtual ~WDeclarativeListItem();

public: // Variables
#ifdef QT_4
    QDeclarativeItem    * object;
    QDeclarativeContext * context;
#else
    QQuickItem  * object;
    QQmlContext * context;
#endif
};

//-------------------------------------------------------------------------------------------------
// WDeclarativeListViewPrivate
//-------------------------------------------------------------------------------------------------

class SKY_GUI_EXPORT WDeclarativeListViewPrivate : public WDeclarativeItemPrivate
{
public:
    WDeclarativeListViewPrivate(WDeclarativeListView * p);

    /* virtual */ ~WDeclarativeListViewPrivate();

    void init(Qt::Orientation orientation);

public: // Functions
    void reset();

    qreal updatePosition();
    void  applyPosition ();

    WDeclarativeListItem * createItem(int index);

    void updateItem(int index, WDeclarativeListItem * item);

    void clearItems();

    void setSize(qreal size);

    int getLast(int first, int count);

public: // Slots
    void onReset();

    void onInsert(const QModelIndex & parent, int start, int end);
    void onRemove(const QModelIndex & parent, int start, int end);

    void onMove(const QModelIndex & sourceParent, int sourceStart, int sourceEnd,
                const QModelIndex & destinationParent, int destinationRow);

    void onDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);

    void onLayoutChanged();

    void onSizeChanged();

    void onClearItemsOld();

public:
    QAbstractItemModel * model;

#ifdef QT_4
    QDeclarativeComponent * delegate;
#else
    QQmlComponent * delegate;
#endif

    QHash<int, WDeclarativeListItem *> items;

    QList<WDeclarativeListItem *> itemsOld;

    Qt::Orientation orientation;

    int count;

    qreal itemSize;

    qreal size;

    qreal start;
    qreal end;

    bool clearDelayed;
    int  clearDelay;

    QTimer timer;

protected:
    W_DECLARE_PUBLIC(WDeclarativeListView)
};

#endif // SK_NO_DECLARATIVELISTVIEW
#endif // WDECLARATIVELISTVIEW_P_H
