set(CPP
	src/io/WLoaderWeb.cpp

)

if(QT_DEFAULT_MAJOR_VERSION EQUAL 5)
	list(APPEND CPP
		
	)
endif()

set(HPP

	include/io/WLoaderWeb
	include/io/WLoaderWeb_p.h
	include/io/WLoaderWeb.h
	include/private/io/WLoaderWeb_p
	include/private/io/WLoaderWeb_p.h

)

if(QT_DEFAULT_MAJOR_VERSION EQUAL 5)
	list(APPEND HPP

	)
endif()

