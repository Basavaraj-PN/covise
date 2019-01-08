# - Find LAMURE
# Find the LAMURE includes and library
#
#  LAMURE_INCLUDE_DIR - Where to find LAMURE includes
#  LAMURE_LIBRARIES   - List of libraries when using LAMURE
#  LAMURE_FOUND       - True if LAMURE was found

IF(LAMURE_INCLUDE_DIR)
  SET(LAMURE_FIND_QUIETLY TRUE)
ENDIF(LAMURE_INCLUDE_DIR)

FIND_PATH(LAMURE_INCLUDE_DIR "lamure/config.h"
  PATHS
  $ENV{LAMURE_HOME}/include
  $ENV{EXTERNLIBS}/Lamure/include
  ~/Library/Frameworks/include
  /Library/Frameworks/include
  /usr/local/include
  /usr/include
  /sw/include # Fink
  /opt/local/include # DarwinPorts
  /opt/csw/include # Blastwave
  /opt/include
  DOC "LAMURE - Headers"
)

SET(LAMURE_NAMES lamure_rendering)
SET(LAMURE_DBG_NAMES lamure_renderingd)
SET(LAMUREVT_NAMES lamure_virtual_texturing)
SET(LAMUREVT_DBG_NAMES lamure_virtual_texturing)
SET(LAMURE_COMMON_NAMES lamure_common)
SET(LAMURE_COMMON_DBG_NAMES lamure_commond)
SET(LAMURE_RENDERING_NAMES lamure_rendering)
SET(LAMURE_RENDERING_DBG_NAMES lamure_renderingd)
SET(LAMURE_PVS_COMMON_NAMES lamure_pvs_common)
SET(LAMURE_PVS_COMMON_DBG_NAMES lamure_pvs_common)

FIND_LIBRARY(LAMURE_LIBRARY NAMES ${LAMURE_NAMES}
  PATHS
  $ENV{LAMURE_HOME}
  $ENV{EXTERNLIBS}/Lamure
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  PATH_SUFFIXES bin lib lib64 lib/Release
  DOC "LAMURE - Library"
)

FIND_LIBRARY(LAMUREVT_LIBRARY NAMES ${LAMUREVT_NAMES}
  PATHS
  $ENV{LAMURE_HOME}
  $ENV{EXTERNLIBS}/Lamure
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  PATH_SUFFIXES bin lib lib64 lib/Release
  DOC "LAMUREVT - Library"
)

FIND_LIBRARY(LAMURE_COMMON_LIBRARY NAMES ${LAMURE_COMMON_NAMES}
  PATHS
  $ENV{LAMURE_HOME}
  $ENV{EXTERNLIBS}/Lamure
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  PATH_SUFFIXES bin lib lib64 lib/Release
  DOC "LAMURE_COMMON - Library"
)
FIND_LIBRARY(LAMURE_RENDERING_LIBRARY NAMES ${LAMURE_RENDERING_NAMES}
  PATHS
  $ENV{LAMURE_HOME}
  $ENV{EXTERNLIBS}/Lamure
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  PATH_SUFFIXES bin lib lib64 lib/Release
  DOC "LAMURE_RENDERING - Library"
)
FIND_LIBRARY(LAMURE_PVS_COMMON_LIBRARY NAMES ${LAMURE_PVS_COMMON_NAMES}
  PATHS
  $ENV{LAMURE_HOME}
  $ENV{EXTERNLIBS}/Lamure
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  PATH_SUFFIXES bin lib lib64 lib/Release
  DOC "LAMURE_PVS_COMMON - Library"
)

INCLUDE(FindPackageHandleStandardArgs)

IF(MSVC)
  # VisualStudio needs a debug version
  FIND_LIBRARY(LAMURE_LIBRARY_DEBUG NAMES ${LAMURE_DBG_NAMES}
    PATHS
    $ENV{LAMURE_HOME}/lib/Debug
    $ENV{EXTERNLIBS}/Lamure/lib/Debug
    DOC "LAMURE - Library (Debug)"
  )
  FIND_LIBRARY(LAMUREVT_LIBRARY_DEBUG NAMES ${LAMUREVT_DBG_NAMES}
    PATHS
    $ENV{LAMURE_HOME}/lib/Debug
    $ENV{EXTERNLIBS}/Lamure/lib/Debug
    DOC "LAMUREVT - Library (Debug)"
  )
  FIND_LIBRARY(LAMURE_COMMON_LIBRARY_DEBUG NAMES ${LAMURE_COMMON_DBG_NAMES}
    PATHS
    $ENV{LAMURE_HOME}/lib/Debug
    $ENV{EXTERNLIBS}/Lamure/lib/Debug
    DOC "LAMURE_COMMON - Library (Debug)"
  )
  FIND_LIBRARY(LAMURE_RENDERING_LIBRARY_DEBUG NAMES ${LAMURE_RENDERING_DBG_NAMES}
    PATHS
    $ENV{LAMURE_HOME}/lib/Debug
    $ENV{EXTERNLIBS}/Lamure/lib/Debug
    DOC "LAMURE_COMMON - Library (Debug)"
  )
  FIND_LIBRARY(LAMURE_PVS_COMMON_LIBRARY_DEBUG NAMES ${LAMURE_PVS_COMMON_DBG_NAMES}
    PATHS
    $ENV{LAMURE_HOME}/lib/Debug
    $ENV{EXTERNLIBS}/Lamure/lib/Debug
    DOC "LAMURE_PVS_COMMON - Library (Debug)"
  )
  
  
  IF(LAMURE_LIBRARY_DEBUG AND LAMURE_LIBRARY)
    SET(LAMURE_LIBRARIES optimized ${LAMURE_LIBRARY} debug ${LAMURE_LIBRARY_DEBUG} optimized ${LAMUREVT_LIBRARY} debug ${LAMUREVT_LIBRARY_DEBUG} optimized ${LAMURE_COMMON_LIBRARY} debug ${LAMURE_COMMON_LIBRARY_DEBUG} optimized ${LAMURE_RENDERING_LIBRARY} debug ${LAMURE_RENDERING_LIBRARY_DEBUG} optimized ${LAMURE_PVS_COMMON_LIBRARY} debug ${LAMURE_PVS_COMMON_LIBRARY_DEBUG})
  ENDIF(LAMURE_LIBRARY_DEBUG AND LAMURE_LIBRARY)

  FIND_PACKAGE_HANDLE_STANDARD_ARGS(LAMURE DEFAULT_MSG LAMURE_LIBRARY LAMURE_LIBRARY_DEBUG LAMUREVT_LIBRARY LAMUREVT_LIBRARY_DEBUG LAMURE_INCLUDE_DIR)

  MARK_AS_ADVANCED(LAMURE_LIBRARY LAMURE_LIBRARY_DEBUG LAMUREVT_LIBRARY LAMUREVT_LIBRARY_DEBUG LAMURE_COMMON_LIBRARY LAMURE_COMMON_LIBRARY_DEBUG LAMURE_RENDERING_LIBRARY LAMURE_RENDERING_LIBRARY_DEBUG LAMURE_PVS_COMMON_LIBRARY LAMURE_PVS_COMMON_LIBRARY_DEBUG LAMURE_INCLUDE_DIR)
  
ELSE(MSVC)
  # rest of the world
  SET(LAMURE_LIBRARIES ${LAMURE_LIBRARY} ${LAMUREVT_LIBRARY} ${LAMURE_COMMON_LIBRARY} ${LAMURE_RENDERING_LIBRARY} ${LAMURE_PVS_COMMON_LIBRARY})

  FIND_PACKAGE_HANDLE_STANDARD_ARGS(LAMURE DEFAULT_MSG LAMURE_LIBRARY LAMUREVT_LIBRARY LAMURE_INCLUDE_DIR)
  
  MARK_AS_ADVANCED(LAMURE_LIBRARY LAMUREVT_LIBRARY LAMURE_INCLUDE_DIRLAMURE_COMMON_LIBRARY  LAMURE_RENDERING_LIBRARY  LAMURE_PVS_COMMON_LIBRARY )
  
ENDIF(MSVC)

IF(LAMURE_FOUND)
  SET(LAMURE_INCLUDE_DIRS ${LAMURE_INCLUDE_DIR})
ENDIF(LAMURE_FOUND)
