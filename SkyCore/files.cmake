set(CPP
  src/3rdparty/qtsingleapplication/qtlockedfile.cpp
  src/3rdparty/qtsingleapplication/qtlocalpeer.cpp

  src/global/Sk_p.cpp

  src/controllers/WController.cpp
  src/controllers/WControllerApplication.cpp
  src/controllers/WControllerDeclarative.cpp
  src/controllers/WControllerDownload.cpp
  src/controllers/WControllerFile.cpp
  src/controllers/WControllerNetwork.cpp

  src/controllers/WControllerPlugin.cpp
  src/controllers/WControllerScript.cpp

  src/controllers/WControllerXml.cpp

  src/kernel/WCoreApplication.cpp
  src/kernel/WCrypter.cpp
  src/kernel/WListId.cpp

  src/io/WAbstractLoader.cpp
  src/io/WFileWatcher.cpp
  src/io/WLoaderNetwork.cpp
  src/io/WCache.cpp
  src/io/WLocalObject.cpp
  src/io/WUnzipper.cpp
  
  #src/io/WYamlReader.cpp

  src/network/WCookieJar.cpp

  src/plugin/WPlugin.cpp

  src/thread/WAbstractThreadAction.cpp
  src/thread/WAbstractThreadReply.cpp
  src/thread/WThreadActions.cpp
)
#[[
if(WIN32)
  list(APPEND CPP src/3rdparty/qtsingleapplication/qtlockedfile_win.cpp)
else()
  list(APPEND CPP src/3rdparty/qtsingleapplication/qtlockedfile_unix.cpp)
endif()
]]
set( HPP
  include/3rdparty/qtsingleapplication/QtLockedFile
  include/3rdparty/qtsingleapplication/qtlockedfile.h
  include/3rdparty/qtsingleapplication/qtlocalpeer.h

  include/global/Sk
  include/global/Sk.h
  include/global/Sk_p.h

  include/controllers/WController
  include/controllers/WController_p.h
  include/controllers/WController.h
  include/controllers/WControllerApplication
  include/controllers/WControllerApplication.h
  include/controllers/WControllerApplication_p.h
  include/controllers/WControllerDeclarative
  include/controllers/WControllerDeclarative.h
  include/controllers/WControllerDownload
  include/controllers/WControllerDownload.h
  include/controllers/WControllerDownload_p.h
  include/controllers/WControllerFile
  include/controllers/WControllerFile.h
  include/controllers/WControllerFile_p.h
  include/controllers/WControllerNetwork
  include/controllers/WControllerNetwork.h
  include/controllers/WControllerNetwork_p.h
  
  include/controllers/WControllerPlugin
  include/controllers/WControllerPlugin.h
  include/controllers/WControllerScript
  include/controllers/WControllerScript.h

  include/controllers/WControllerXml
  include/controllers/WControllerXml.h

  include/private/controllers/WController
  include/private/controllers/WController.h
  include/private/controllers/WControllerApplication
  include/private/controllers/WControllerApplication.h
  include/private/controllers/WControllerApplication_p
  include/private/controllers/WControllerDownload_p
  include/private/controllers/WControllerDownload_p.h
  include/private/controllers/WControllerFile_p
  include/private/controllers/WControllerFile_p.h
  include/private/controllers/WControllerNetwork_p
  include/private/controllers/WControllerNetwork_p.h
  include/private/controllers/WController_p
  include/private/controllers/WController_p.h

  include/io/WAbstractLoader
  include/io/WAbstractLoader.h
  include/io/WAbstractLoader_p.h
  include/io/WFileWatcher
  include/io/WFileWatcher.h
  include/io/WFileWatcher_p.h
  include/io/WLoaderNetwork
  include/io/WLoaderNetwork.h
  include/io/WLoaderNetwork_p.h
  include/io/WCache
  include/io/WCache_p.h
  include/io/WCache.h
  include/io/WLocalObject
  include/io/WLocalObject_p.h
  include/io/WLocalObject.h
  
  include/io/WUnzipper
  include/io/WUnzipper.h

  # include/io/WYamlReader
  # include/io/WYamlReader.h

  include/kernel/WCoreApplication
  include/kernel/WCoreApplication.h
  include/kernel/WCrypter
  include/kernel/WCrypter.h
  include/kernel/WListId
  include/kernel/WListId.h

  include/network/WCookieJar
  include/network/WCookieJar.h
  include/network/WCookieJar_p.h

  include/plugin/WPlugin
  include/plugin/WPlugin.h
  include/plugin/WPluginInterface.h
  include/plugin/WPlugin_p.h

  include/private/plugin/WPlugin_p
  include/private/plugin/WPlugin_p.h

  include/private/Sk_p
  include/private/Sk_p.h

  include/private/io/WAbstractLoader_p
  include/private/io/WAbstractLoader_p.h
  include/private/io/WCache_p
  include/private/io/WCache_p.h
  include/private/io/WFileWatcher_p
  include/private/io/WFileWatcher_p.h
  include/private/io/WLoaderNetwork_p
  include/private/io/WLoaderNetwork_p.h
  include/private/io/WLocalObject_p
  include/private/io/WLocalObject_p.h

  include/private/kernel/WListId
  include/private/kernel/WListId.h

  include/private/network/WCookieJar_p
  include/private/network/WCookieJar_p.h

  include/private/thread/WAbstractThreadAction_p
  include/private/thread/WAbstractThreadAction_p.h
  include/private/thread/WAbstractThreadReply_p
  include/private/thread/WAbstractThreadReply_p.h
  include/private/thread/WThreadActions_p
  include/private/thread/WThreadActions_p.h

  include/thread/WAbstractThreadAction
  include/thread/WAbstractThreadAction.h
  include/thread/WAbstractThreadAction_p.h
  include/thread/WAbstractThreadReply
  include/thread/WAbstractThreadReply.h
  include/thread/WAbstractThreadReply_p.h
  include/thread/WThreadActions
  include/thread/WThreadActions.h
  include/thread/WThreadActions_p.h
)