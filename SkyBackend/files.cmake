set(CPP
	src/io/WBackendCache.cpp
	src/io/WBackendIndex.cpp
	src/io/WBackendIndex_patch.cpp
	
	src/media/WBackendBing.cpp
	src/media/WBackendDailymotion.cpp
	src/media/WBackendDuckDuckGo.cpp
	src/media/WBackendLastFm.cpp
	src/media/WBackendOpenSubtitles.cpp
	src/media/WBackendSoundCloud.cpp
	src/media/WBackendTmdb.cpp
	src/media/WBackendUniversal.cpp
	src/media/WBackendUniversal_patch.cpp
	src/media/WBackendVimeo.cpp
	src/media/WBackendYoutube.cpp

)

set( HPP
	include/SkBackend
	include/SkBackend.h
	
	include/io/WBackendCache.h
	include/io/WBackendIndex
	include/io/WBackendIndex.h
	include/io/WBackendIndex_p.h
	
	include/media/WBackendBing
	include/media/WBackendBing.h
	include/media/WBackendDailymotion
	include/media/WBackendDailymotion.h
	include/media/WBackendDuckDuckGo
	include/media/WBackendDuckDuckGo.h
	include/media/WBackendLastFm
	include/media/WBackendLastFm.h
	include/media/WBackendOpenSubtitles
	include/media/WBackendOpenSubtitles.h
	include/media/WBackendSoundCloud
	include/media/WBackendSoundCloud.h
	include/media/WBackendTmdb
	include/media/WBackendTmdb.h
	include/media/WBackendUniversal
	include/media/WBackendUniversal.h
	include/media/WBackendUniversal_p.h
	include/media/WBackendVimeo
	include/media/WBackendVimeo.h
	include/media/WBackendYoutube
	include/media/WBackendYoutube.h
	
	include/private/io/WBackendIndex_p
	include/private/io/WBackendIndex_p.h
	
	include/private/media/WBackendUniversal_p
	include/private/media/WBackendUniversal_p.h

)