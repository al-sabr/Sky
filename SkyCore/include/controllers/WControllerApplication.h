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

#ifndef WCONTROLLERAPPLICATION_H
#define WCONTROLLERAPPLICATION_H

// Qt includes
#include <QEventLoop>
#include <QVariant>
#include <QTime>
#include <QStringList>

// Sk includes
#include <WController>
#include <SkyCoreExports.h>

#ifndef SK_NO_CONTROLLERAPPLICATION

// Forward declarations
class QDateTime;
class QMimeData;
class WControllerApplicationPrivate;

// Defines
#define sk WControllerApplication::instance()
#define Sk WControllerApplication

class SKY_CORE_EXPORT WControllerApplication : public QObject, public WPrivatable
{
    Q_OBJECT

    Q_ENUMS(Type)

    Q_PROPERTY(Type type READ type CONSTANT)

    Q_PROPERTY(bool isSingle   READ isSingle   CONSTANT)
    Q_PROPERTY(bool isMultiple READ isMultiple CONSTANT)

    Q_PROPERTY(bool isCore READ isCore CONSTANT)
    Q_PROPERTY(bool isGui  READ isGui  CONSTANT)

    Q_PROPERTY(bool qrc READ isQrc WRITE setQrc NOTIFY qrcChanged)

    Q_PROPERTY(bool osWin   READ osWin   CONSTANT)
    Q_PROPERTY(bool osMac   READ osMac   CONSTANT)
    Q_PROPERTY(bool osLinux READ osLinux CONSTANT)

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)

    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)

    Q_PROPERTY(QString versionSky READ versionSky CONSTANT)
    Q_PROPERTY(QString versionQt  READ versionQt  CONSTANT)

    Q_PROPERTY(QString applicationUrl READ applicationUrl WRITE setApplicationUrl
               NOTIFY applicationUrlChanged)

#ifndef SK_CONSOLE
    Q_PROPERTY(Mode defaultMode READ defaultMode WRITE setDefaultMode NOTIFY defaultModeChanged)

    Q_PROPERTY(int defaultScreen READ defaultScreen WRITE setDefaultScreen
               NOTIFY defaultScreenChanged)

    Q_PROPERTY(int defaultWidth READ defaultWidth WRITE setDefaultWidth
               NOTIFY defaultWidthChanged)

    Q_PROPERTY(int defaultHeight READ defaultHeight WRITE setDefaultHeight
               NOTIFY defaultHeightChanged)

    Q_PROPERTY(int defaultMargins READ defaultMargins WRITE setDefaultMargins
               NOTIFY defaultMarginsChanged)

    Q_PROPERTY(bool screenDimEnabled READ screenDimEnabled WRITE setScreenDimEnabled
               NOTIFY screenDimEnabledChanged)

    Q_PROPERTY(bool screenSaverEnabled READ screenSaverEnabled WRITE setScreenSaverEnabled
               NOTIFY screenSaverEnabledChanged)

    Q_PROPERTY(bool cursorVisible READ cursorVisible WRITE setCursorVisible
               NOTIFY cursorVisibleChanged)
#endif

public: // Enums
    enum Type { Single = 0, Multiple };

#ifndef SK_CONSOLE
    enum Mode { Normal = 0, Maximized, FullScreen, FullScreenMaximized };
#endif

private:
    WControllerApplication();

    void initController();

public: // Interface
#if defined(SK_CONSOLE) == false && defined(SK_NO_QML) == false
    Q_INVOKABLE void startScript  ();
    Q_INVOKABLE void restartScript();

    Q_INVOKABLE void clearComponentCache() const;
#endif

    Q_INVOKABLE void quit();

    /*Q_INVOKABLE QPoint cursorPosition() const;
    Q_INVOKABLE void   setCursorPosition(const QPoint & position);
    Q_INVOKABLE void   clearCursorPosition();*/

#ifndef SK_CONSOLE
    Q_INVOKABLE QString clipboardText() const;
    Q_INVOKABLE void    setClipboardText(const QString & text);
#endif

public: // Static functions
    Q_INVOKABLE static bool fuzzyCompare(qreal valueA, qreal valueB);

    Q_INVOKABLE static QList<int> variantsToInts(const QVariantList & variants);

    Q_INVOKABLE static QMimeData * duplicateMime(const QMimeData * source);

#ifdef QT_4
    Q_INVOKABLE static int keypad(int modifiers);
#else
    Q_INVOKABLE static Qt::KeyboardModifiers keypad(Qt::KeyboardModifiers flags);
#endif

    //---------------------------------------------------------------------------------------------
    // Version

    // NOTE: Returns 0 when equal, -1 when lower, and 1 when higher.
    Q_INVOKABLE static int versionCheck(const QString & versionA,
                                        const QString & versionB);

    Q_INVOKABLE static bool versionIsLower(const QString & versionA,
                                           const QString & versionB);

    Q_INVOKABLE static bool versionIsHigher(const QString & versionA,
                                            const QString & versionB);

    Q_INVOKABLE static QString getVersionLite(const QString & version);

    //---------------------------------------------------------------------------------------------
    // File

    Q_INVOKABLE static void setCurrentPath(const QString & path);

    Q_INVOKABLE static bool fileExists(const QString & fileName);

    //---------------------------------------------------------------------------------------------
    // Process

    Q_INVOKABLE static bool runUpdate();

    Q_INVOKABLE static bool runAdmin(const QString & fileName, const QString & parameters);

    Q_INVOKABLE static void wait(int msec, QEventLoop::ProcessEventsFlags flags
                                           = QEventLoop::AllEvents);

    Q_INVOKABLE static void processEvents(QEventLoop::ProcessEventsFlags flags
                                          = QEventLoop::AllEvents);

    Q_INVOKABLE static void processEvents(QEventLoop::ProcessEventsFlags flags, int maxtime);

    //---------------------------------------------------------------------------------------------
    // String

    Q_INVOKABLE static int indexSkip(const QString & string, const QString & match, int skip);

    Q_INVOKABLE static QString trim      (const QString & string);
    Q_INVOKABLE static QString simplify  (const QString & string);
    Q_INVOKABLE static QString reverse   (const QString & string);
    Q_INVOKABLE static QString capitalize(const QString & string);
    Q_INVOKABLE static QString quote     (const QString & string);

    Q_INVOKABLE static void replaceFirst(QString * string, const QString & before,
                                                           const QString & after,
                                                           int             from = 0);

    Q_INVOKABLE static QString slice(const QString & string, int start, int end);

    Q_INVOKABLE static QString slice(const QString & string, const QString & start,
                                                             const QString & end,
                                                             int             from = 0);

    Q_INVOKABLE static QString slice(const QString & string, const QRegExp & start,
                                                             const QRegExp & end,
                                                             int             from = 0);

    Q_INVOKABLE static QString sliceIn(const QString & string, const QString & start,
                                                               const QString & end,
                                                               int             from = 0);

    Q_INVOKABLE static QString sliceIn(const QString & string, const QRegExp & start,
                                                               const QRegExp & end,
                                                               int             from = 0);

    Q_INVOKABLE static QStringList slices(const QString & string, const QString & start,
                                                                  const QString & end,
                                                                  int             from = 0);

    Q_INVOKABLE static QStringList slices(const QString & string, const QRegExp & start,
                                                                  const QRegExp & end,
                                                                  int             from = 0);

    Q_INVOKABLE static QStringList slicesIn(const QString & string, const QString & start,
                                                                    const QString & end,
                                                                    int             from = 0);

    Q_INVOKABLE static QStringList slicesIn(const QString & string, const QRegExp & start,
                                                                    const QRegExp & end,
                                                                    int             from = 0);

    Q_INVOKABLE static QString extractText(QString * string, const QString & pattern);
    Q_INVOKABLE static QString extractText(QString * string, const QRegExp & regExp);

    Q_INVOKABLE static QString extractLine(QString * string);
    Q_INVOKABLE static QString extractWord(QString * string);

    Q_INVOKABLE static QString regExpCap(const QString & string,
                                         const QString & pattern, int cap, int from = 0);

    Q_INVOKABLE static void skipCharacters(QString * string, const QChar & character);

    Q_INVOKABLE static void skipSpaces(QString * string);

    Q_INVOKABLE static bool checkEscaped(const QString & string, int from);

    //---------------------------------------------------------------------------------------------
    // Read

    Q_INVOKABLE static QString readAscii(const QByteArray & array);

    Q_INVOKABLE static QString readCodec(const QByteArray & array, const QString & codec);
    Q_INVOKABLE static QString readUtf8 (const QByteArray & array);

    Q_INVOKABLE static QString latinToUtf8(const QString & string);

    Q_INVOKABLE static QString unicodeToUtf8(const QString & string);

    Q_INVOKABLE static QString detectCodec(const QByteArray & array);

    //---------------------------------------------------------------------------------------------
    // Generate

    Q_INVOKABLE static QByteArray generateRandomString(int length);

    Q_INVOKABLE static QByteArray generateHmacSha1(const QByteArray & bytes,
                                                   const QByteArray & key);

    //---------------------------------------------------------------------------------------------
    // Text

#ifndef SK_CONSOLE
    Q_INVOKABLE static QString textElided(const QString     & text,
                                          const QFont       & font,
                                          int                 width,
                                          Qt::TextElideMode   mode = Qt::ElideRight);

    Q_INVOKABLE static int textWidth (const QFont & font, const QString & text);
    Q_INVOKABLE static int textHeight(const QFont & font);
#endif

    //---------------------------------------------------------------------------------------------
    // Message

    // NOTE: This is useful to extract the argument from QtSingleApplication::messageReceived().
    Q_INVOKABLE static QString extractArgument(const QString & message);

    // NOTE: On Android this returns the 'intent' text.
    Q_INVOKABLE static QString getMessage();

#ifdef Q_OS_ANDROID
    Q_INVOKABLE static QString getIntentText();
#endif

    //---------------------------------------------------------------------------------------------
    // Time

    Q_INVOKABLE static QTime     getTime();
    Q_INVOKABLE static QDateTime getDate();

    Q_INVOKABLE static QDateTime dateFromSecs(qint64 seconds);

    Q_INVOKABLE static int getMsecs  (const QTime & time);
    Q_INVOKABLE static int getElapsed(const QTime & time);

    Q_INVOKABLE static QString dateToString      (const QDateTime & date, const QString & format);
    Q_INVOKABLE static QString dateToStringNumber(const QDateTime & date);

    Q_INVOKABLE static QString currentDateString(const QString & format);

    Q_INVOKABLE static QString dateToText(const QDateTime & date);

signals:
    //void controllerCreated  (WController * controller);
    //void controllerDestroyed(WController * controller);

#ifdef SK_MOBILE
    // NOTE: This is useful on Android to notify that the 'intent' has changed.
    void messageUpdated();
#endif

    void aboutToQuit();

    void qrcChanged();

    void nameChanged();
    void iconChanged();

    void versionChanged();

    void applicationUrlChanged();

#ifndef SK_CONSOLE
    void defaultModeChanged();

    void defaultScreenChanged();

    void defaultWidthChanged ();
    void defaultHeightChanged();

    void defaultMarginsChanged();

    void screenDimEnabledChanged  ();
    void screenSaverEnabledChanged();

    void cursorVisibleChanged();
#endif

public: // Properties
    Type type() const;

    bool isSingle  () const;
    bool isMultiple() const;

    bool isCore() const;
    bool isGui () const;

    bool isQrc() const;
    void setQrc(bool qrc);

    bool osWin  () const;
    bool osMac  () const;
    bool osLinux() const;

    QString name() const;
    void    setName(const QString & name);

    QString icon() const;
    void    setIcon(const QString & icon);

    QString version() const;
    void    setVersion(const QString & version);

    static QString versionSky();
    static QString versionQt ();

    QString applicationUrl() const;
    void    setApplicationUrl(const QString & url);

#ifndef SK_CONSOLE
    Mode defaultMode() const;
    void setDefaultMode(Mode mode);

    int  defaultScreen() const;
    void setDefaultScreen(int screen);

    int  defaultWidth() const;
    void setDefaultWidth(int width);

    int  defaultHeight() const;
    void setDefaultHeight(int height);

    int  defaultMargins() const;
    void setDefaultMargins(int ratio);

    bool screenDimEnabled() const;
    void setScreenDimEnabled(bool enabled);

    bool screenSaverEnabled() const;
    void setScreenSaverEnabled(bool enabled);

    bool cursorVisible() const;
    void setCursorVisible(bool visible);
#endif

private:
    W_DECLARE_PRIVATE   (WControllerApplication)
    W_DECLARE_CONTROLLER(WControllerApplication)

    Q_PRIVATE_SLOT(d_func(), void onAboutToQuit())

    friend class WController;
    friend class WControllerPrivate;
    friend class WCoreApplication;
    friend class WApplication;
    friend class WMockApplication;
};

#include <private/controllers/WControllerApplication_p>

#endif // SK_NO_CONTROLLERAPPLICATION
#endif // WCONTROLLERAPPLICATION_H
