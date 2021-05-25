set(CPP
	src/controllers/WControllerMedia.cpp
	
	src/media/WBackendSubtitle.cpp
	src/media/WBackendVlc.cpp
	
	src/vlc/WVlcEngine.cpp
	src/vlc/WVlcPlayer.cpp

)

set(HPP

	include/3rdparty/vlc/mmxRgb.h
	include/3rdparty/opengl/glext.h

	include/controllers/WControllerMedia
	include/controllers/WControllerMedia.h
	include/controllers/WControllerMedia_p.h

	include/media/WBackendSubtitle
	include/media/WBackendSubtitle_p.h
	include/media/WBackendSubtitle.h
	include/media/WBackendVlc
	include/media/WBackendVlc.h
	include/media/WBackendVlc_p.h

	include/vlc/WVlcEngine
	include/vlc/WVlcEngine.h
	include/vlc/WVlcEngine_p.h
	include/vlc/WVlcPlayer
	include/vlc/WVlcPlayer.h
	include/vlc/WVlcPlayer_p.h

	include/private/controllers/WControllerMedia_p

	include/private/media/WBackendSubtitle_p
	include/private/media/WBackendVlc_p

	include/private/vlc/WVlcEngine_p
	include/private/vlc/WVlcPlayer_p

)
