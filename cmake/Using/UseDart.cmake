MACRO(USE_DART)
  USE_ASSIMP()
  set(CMAKE_PREFIX_PATH "$ENV{EXTERNLIBS}/dart" ${CMAKE_PREFIX_PATH})
  set(CMAKE_PREFIX_PATH "$ENV{EXTERNLIBS}/ccd" ${CMAKE_PREFIX_PATH})
  set(CMAKE_PREFIX_PATH "$ENV{EXTERNLIBS}/fcl" ${CMAKE_PREFIX_PATH})
  set(CMAKE_PREFIX_PATH "$ENV{EXTERNLIBS}/octomap" ${CMAKE_PREFIX_PATH})
  set(CMAKE_PREFIX_PATH "$ENV{EXTERNLIBS}/tinyxml2" ${CMAKE_PREFIX_PATH})
  set(required_components gui-osg)
  set(required_libraries dart dart-utils dart-gui-osg)


  find_package(ccd)
  find_package(fcl)
  find_package(DART 6.6.0 REQUIRED COMPONENTS ${required_components} CONFIG)
  #covise_find_package(dart)
  IF ((NOT DART_FOUND) AND (${ARGC} LESS 1))
    USING_MESSAGE("Skipping because of missing DART")
    RETURN()
  ENDIF((NOT DART_FOUND) AND (${ARGC} LESS 1))
  IF(NOT DART_USED AND DART_FOUND)
    SET(DART_USED TRUE)
    INCLUDE_DIRECTORIES(SYSTEM ${DART_INCLUDE_DIRS} ${dart_gui-osg_INCLUDE_DIRS})
    SET(EXTRA_LIBS ${EXTRA_LIBS} ${DART_LIBRARIES})
  ENDIF()
ENDMACRO(USE_DART)

