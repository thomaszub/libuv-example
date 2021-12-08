FIND_PATH(UV_INCLUDE_DIR NAMES uv.h)
FIND_LIBRARY(UV_LIBRARY NAMES uv libuv)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(uv DEFAULT_MSG UV_LIBRARY UV_INCLUDE_DIR)

IF (UV_INCLUDE_DIR AND UV_LIBRARY)
    SET(UV_FOUND TRUE)
ELSE (UV_INCLUDE_DIR AND UV_LIBRARY)
    SET(UV_FOUND FALSE)
ENDIF (UV_INCLUDE_DIR AND UV_LIBRARY)

IF (UV_FOUND)
    MESSAGE(STATUS "Found libuv: ${UV_LIBRARY}, ${UV_INCLUDE_DIR}")
ELSE (UV_FOUND)
    MESSAGE(FATAL_ERROR "Could not find libuv.")
ENDIF (UV_FOUND)