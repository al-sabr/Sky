# Reminder setup for successful compilation #1
# Reminder setup for successful compilation #2
# Reminder setup for successful compilation #3
# Reminder setup for successful compilation #4

set(CPP
	src/3rdparty/qtsingleapplication/qtsingleapplication.cpp
	
	# src/controllers/WControllerPlaylist.cpp
	src/controllers/WControllerView.cpp
	
	# src/declarative/WDeclarativeAnimated.cpp
	# src/declarative/WDeclarativeApplication.cpp
	# src/declarative/WDeclarativeBorderImage.cpp
	# src/declarative/WDeclarativeBorders.cpp
	src/declarative/WDeclarativeContextualPage.cpp
	# src/declarative/WDeclarativeImage.cpp
	# src/declarative/WDeclarativeImageBase.cpp
	# src/declarative/WDeclarativeImageSvg.cpp
	src/declarative/WDeclarativeItem.cpp
	# src/declarative/WDeclarativeListView.cpp
	src/declarative/WDeclarativeMouseArea.cpp
	# src/declarative/WDeclarativeMouseWatcher.cpp
	# src/declarative/WDeclarativePlayer.cpp
	# src/declarative/WDeclarativeText.cpp
	# src/declarative/WDeclarativeTextSvg.cpp
	
	src/graphicsview/WAbstractView.cpp
	src/graphicsview/WResizer.cpp
	src/graphicsview/WView.cpp
	# src/graphicsview/WViewDrag.cpp
	# src/graphicsview/WViewResizer.cpp
	# src/graphicsview/WWindow.cpp
	
	# src/image/WImageFilter.cpp
	# src/image/WImageFilterColor.cpp
	# src/image/WImageFilterMask.cpp
	# src/image/WPixmapCache.cpp
	
	# src/io/WBackendLoader.cpp
	
	# src/kernel/WAbstractTab.cpp
	# src/kernel/WAbstractTabs.cpp
	# src/kernel/WActionCue.cpp
	# src/kernel/WApplication.cpp
	# src/kernel/WInputCue.cpp
	
	# src/media/WAbstractBackend.cpp
	# src/media/WAbstractHook.cpp
	# src/media/WBackendNet.cpp
	# src/media/WBookmarkTrack.cpp
	# src/media/WLibraryFolder.cpp
	# src/media/WLibraryFolderRelated.cpp
	# src/media/WLibraryItem.cpp
	# src/media/WPlaylist.cpp
	# src/media/WTabTrack.cpp
	# src/media/WTabsTrack.cpp
	# src/media/WTrack.cpp
	
	src/models/WModelCompletionGoogle.cpp
	src/models/WModelContextual.cpp
	# src/models/WModelLibraryFolder.cpp
	src/models/WModelList.cpp
	# src/models/WModelPlaylist.cpp
	src/models/WModelRange.cpp
	# src/models/WModelTabs.cpp

)

if(QT_MAJOR_VERSION EQUAL 5)
	# list(APPEND CPP
	# 	src/declarative/WDeclarativeTexture.cpp
	# 	src/declarative/WDeclarativeItemPaint.cpp
	# )
endif()

set( HPP
	include/3rdparty/qtsingleapplication/QtSingleApplication
	include/3rdparty/qtsingleapplication/qtsingleapplication.h

	# include/controllers/WControllerPlaylist
	# include/controllers/WControllerPlaylist.h
	# include/controllers/WControllerPlaylist_p.h
	include/controllers/WControllerView
	include/controllers/WControllerView.h
	include/controllers/WControllerView_p.h
	
	# include/declarative/WDeclarativeAnimated
	# include/declarative/WDeclarativeAnimated.h
	# include/declarative/WDeclarativeAnimated_p.h
	# include/declarative/WDeclarativeApplication
	# include/declarative/WDeclarativeApplication.h
	# include/declarative/WDeclarativeBorderImage
	# include/declarative/WDeclarativeBorderImage.h
	# include/declarative/WDeclarativeBorderImage_p.h
	# include/declarative/WDeclarativeBorders
	# include/declarative/WDeclarativeBorders.h
	include/declarative/WDeclarativeContextualPage
	include/declarative/WDeclarativeContextualPage.h
	# include/declarative/WDeclarativeImage
	# include/declarative/WDeclarativeImage.h
	# include/declarative/WDeclarativeImageBase
	# include/declarative/WDeclarativeImageBase.h
	# include/declarative/WDeclarativeImageBase_p.h
	# include/declarative/WDeclarativeImageSvg
	# include/declarative/WDeclarativeImageSvg.h
	# include/declarative/WDeclarativeImageSvg_p.h
	# include/declarative/WDeclarativeImage_p.h
	include/declarative/WDeclarativeItem
	include/declarative/WDeclarativeItem.h
	include/declarative/WDeclarativeItem_p.h
	# include/declarative/WDeclarativeListView
	# include/declarative/WDeclarativeListView.h
	# include/declarative/WDeclarativeListView_p.h
	include/declarative/WDeclarativeMouseArea
	include/declarative/WDeclarativeMouseArea.h
	include/declarative/WDeclarativeMouseArea_p.h
	# include/declarative/WDeclarativeMouseWatcher
	# include/declarative/WDeclarativeMouseWatcher.h
	# include/declarative/WDeclarativeMouseWatcher_p.h
	# include/declarative/WDeclarativePlayer
	# include/declarative/WDeclarativePlayer.h
	# include/declarative/WDeclarativePlayer_p.h
	# include/declarative/WDeclarativeText
	# include/declarative/WDeclarativeText.h
	# include/declarative/WDeclarativeTextSvg
	# include/declarative/WDeclarativeTextSvg.h
	# include/declarative/WDeclarativeTextSvg_p.h
	
	include/graphicsview/WAbstractView
	include/graphicsview/WAbstractView.h
	include/graphicsview/WAbstractView_p.h
	include/graphicsview/WResizer
	include/graphicsview/WResizer.h
	include/graphicsview/WResizer_p.h
	include/graphicsview/WView
	include/graphicsview/WView.h
	# include/graphicsview/WViewDrag
	# include/graphicsview/WViewDrag.h
	# include/graphicsview/WViewResizer
	# include/graphicsview/WViewResizer.h
	# include/graphicsview/WView_p.h
	# include/graphicsview/WWindow
	# include/graphicsview/WWindow.h
	# include/graphicsview/WWindow_p.h
	
	# include/image/WImageFilter
	# include/image/WImageFilter.h
	# include/image/WImageFilterColor
	# include/image/WImageFilterColor.h
	# include/image/WImageFilterMask
	# include/image/WImageFilterMask.h
	# include/image/WImageFilter_p.h
	# include/image/WPixmapCache
	# include/image/WPixmapCache.h
	
	# include/io/WBackendLoader
	# include/io/WBackendLoader.h
	# include/io/WBackendLoader_p.h
	
	# include/kernel/WAbstractTab
	# include/kernel/WAbstractTab.h
	# include/kernel/WAbstractTab_p.h
	# include/kernel/WAbstractTabs
	# include/kernel/WAbstractTabs.h
	# include/kernel/WAbstractTabs_p.h
	# include/kernel/WActionCue
	# include/kernel/WActionCue.h
	# include/kernel/WActionCue_p.h
	# include/kernel/WApplication
	# include/kernel/WApplication.h
	# include/kernel/WInputCue
	# include/kernel/WInputCue.h
	# include/kernel/WInputCue_p.h
	
	# include/media/WAbstractBackend
	# include/media/WAbstractBackend.h
	# include/media/WAbstractBackend_p.h
	# include/media/WAbstractHook
	# include/media/WAbstractHook.h
	# include/media/WAbstractHook_p.h
	# include/media/WBackendNet
	# include/media/WBackendNet.h
	# include/media/WBackendNet_p.h
	# include/media/WBookmarkTrack
	# include/media/WBookmarkTrack.h
	# include/media/WBookmarkTrack_p.h
	# include/media/WLibraryFolder
	# include/media/WLibraryFolder.h
	# include/media/WLibraryFolderRelated
	# include/media/WLibraryFolderRelated.h
	# include/media/WLibraryFolderRelated_p.h
	# include/media/WLibraryFolder_p.h
	# include/media/WLibraryItem
	# include/media/WLibraryItem.h
	# include/media/WLibraryItem_p.h
	# include/media/WPlaylist
	# include/media/WPlaylist.h
	# include/media/WPlaylist_p.h
	# include/media/WTabTrack
	# include/media/WTabTrack.h
	# include/media/WTabTrack_p.h
	# include/media/WTabsTrack
	# include/media/WTabsTrack.h
	# include/media/WTabsTrack_p.h
	# include/media/WTrack
	# include/media/WTrack.h
	# include/media/WTrack_p.h
	
	include/models/WModelCompletionGoogle
	include/models/WModelCompletionGoogle.h
	include/models/WModelCompletionGoogle_p.h
	
	include/models/WModelContextual
	include/models/WModelContextual.h
	include/models/WModelContextual_p.h
	# include/models/WModelLibraryFolder
	# include/models/WModelLibraryFolder.h
	# include/models/WModelLibraryFolder_p.h
	include/models/WModelList
	include/models/WModelList.h
	# include/models/WModelPlaylist
	# include/models/WModelPlaylist.h
	# include/models/WModelPlaylist_p.h
	include/models/WModelRange
	include/models/WModelRange.h
	# include/models/WModelTabs
	# include/models/WModelTabs.h
	
	# include/private/controllers/WControllerPlaylist_p
	# include/private/controllers/WControllerPlaylist_p.h
	include/private/controllers/WControllerView_p
	include/private/controllers/WControllerView_p.h
	
	# include/private/declarative/WDeclarativeAnimated_p
	# include/private/declarative/WDeclarativeAnimated_p.h
	# include/private/declarative/WDeclarativeBorderImage_p
	# include/private/declarative/WDeclarativeBorderImage_p.h
	# include/private/declarative/WDeclarativeImageBase_p
	# include/private/declarative/WDeclarativeImageBase_p.h
	# include/private/declarative/WDeclarativeImageSvg_p
	# include/private/declarative/WDeclarativeImageSvg_p.h
	# include/private/declarative/WDeclarativeImage_p
	# include/private/declarative/WDeclarativeImage_p.h
	include/private/declarative/WDeclarativeItem_p
	include/private/declarative/WDeclarativeItem_p.h
	# include/private/declarative/WDeclarativeListView_p
	# include/private/declarative/WDeclarativeListView_p.h
	include/private/declarative/WDeclarativeMouseArea_p
	include/private/declarative/WDeclarativeMouseArea_p.h
	# include/private/declarative/WDeclarativeMouseWatcher_p
	# include/private/declarative/WDeclarativeMouseWatcher_p.h
	# include/private/declarative/WDeclarativePlayer_p
	# include/private/declarative/WDeclarativePlayer_p.h
	# include/private/declarative/WDeclarativeTextSvg_p
	# include/private/declarative/WDeclarativeTextSvg_p.h
	
	include/private/graphicsview/WAbstractView_p
	include/private/graphicsview/WAbstractView_p.h
	include/private/graphicsview/WResizer_p
	include/private/graphicsview/WResizer_p.h
	include/private/graphicsview/WView_p
	include/private/graphicsview/WView_p.h
	# include/private/graphicsview/WWindow_p
	# include/private/graphicsview/WWindow_p.h
	
	# include/private/image/WImageFilter_p
	# include/private/image/WImageFilter_p.h
	
	# include/private/io/WBackendLoader_p
	# include/private/io/WBackendLoader_p.h
	
	# include/private/kernel/WAbstractTab_p
	# include/private/kernel/WAbstractTab_p.h
	# include/private/kernel/WAbstractTabs_p
	# include/private/kernel/WAbstractTabs_p.h
	# include/private/kernel/WActionCue_p
	# include/private/kernel/WActionCue_p.h
	# include/private/kernel/WInputCue_p
	# include/private/kernel/WInputCue_p.h
	
	# include/private/media/WAbstractBackend_p
	# include/private/media/WAbstractBackend_p.h
	# include/private/media/WAbstractHook_p
	# include/private/media/WAbstractHook_p.h
	# include/private/media/WBackendNet_p
	# include/private/media/WBackendNet_p.h
	# include/private/media/WBookmarkTrack_p
	# include/private/media/WBookmarkTrack_p.h
	# include/private/media/WLibraryFolderRelated_p
	# include/private/media/WLibraryFolderRelated_p.h
	# include/private/media/WLibraryFolder_p
	# include/private/media/WLibraryFolder_p.h
	# include/private/media/WLibraryItem_p
	# include/private/media/WLibraryItem_p.h
	# include/private/media/WPlaylist_p
	# include/private/media/WPlaylist_p.h
	# include/private/media/WTabTrack_p
	# include/private/media/WTabTrack_p.h
	# include/private/media/WTabsTrack_p
	# include/private/media/WTabsTrack_p.h
	# include/private/media/WTrack_p
	# include/private/media/WTrack_p.h

	include/private/models/WModelCompletionGoogle_p
	include/private/models/WModelContextual_p
	# include/private/models/WModelLibraryFolder_p
	# include/private/models/WModelPlaylist_p

)

if(QT_MAJOR_VERSION EQUAL 5)
	list(APPEND HPP
		# include/declarative/WDeclarativeItemPaint
		# include/declarative/WDeclarativeItemPaint.h
		# include/declarative/WDeclarativeItemPaint_p.h

		# include/private/declarative/WDeclarativeItemPaint_p
		# include/private/declarative/WDeclarativeItemPaint_p.h

		# include/declarative/WDeclarativeTexture
		# include/declarative/WDeclarativeTexture.h
		# include/declarative/WDeclarativeTexture_p.h
		
		# include/private/declarative/WDeclarativeTexture_p
		# include/private/declarative/WDeclarativeTexture_p.h

	)
endif()

