include(FetchContent)

FetchContent_Declare(
        vlc-qt
        GIT_REPOSITORY "https://github.com/vlc-qt/vlc-qt"
        GIT_TAG "1.1.1"
)

FetchContent_MakeAvailable(vlc-qt)