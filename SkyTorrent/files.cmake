set(CPP
	src/controllers/WControllerTorrent.cpp
	
	src/io/WLoaderTorrent.cpp
	
	src/media/WBackendTorrent.cpp
	src/media/WHookTorrent.cpp
	
	src/torrent/WTorrentEngine.cpp

)

if(QT_DEFAULT_MAJOR_VERSION EQUAL 5)
	list(APPEND CPP
		
	)
endif()

set(HPP

	include/controllers/WControllerTorrent
	include/controllers/WControllerTorrent.h
	include/controllers/WControllerTorrent_p.h

	include/io/WLoaderTorrent
	include/io/WLoaderTorrent.h
	include/io/WLoaderTorrent_p.h

	include/media/WBackendTorrent
	include/media/WBackendTorrent.h
	include/media/WHookTorrent
	include/media/WHookTorrent.h
	include/media/WHookTorrent_p.h

	include/torrent/WTorrentEngine
	include/torrent/WTorrentEngine.h
	include/torrent/WTorrentEngine_p.h

	include/private/controllers/WControllerTorrent_p
	include/private/controllers/WControllerTorrent_p.h

	include/private/io/WLoaderTorrent_p
	include/private/io/WLoaderTorrent_p.h

	include/private/torrent/WTorrentEngine_p
	include/private/torrent/WTorrentEngine_p.h


)

if(QT_DEFAULT_MAJOR_VERSION EQUAL 5)
	list(APPEND HPP

	)
endif()

