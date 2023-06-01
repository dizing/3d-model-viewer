# - Try to find the CHECK libraries
#  Once done this will define
#
#  CHECK_FOUND - system has check
#  CHECK_INCLUDE_DIR - the check include directory
#  CHECK_LIBRARIES - check library
#
#  This configuration file for finding libcheck is originally from
#  the opensync project. The originally was downloaded from here:
#  opensync.org/browser/branches/3rd-party-cmake-modules/modules/FindCheck.cmake
#
#  Copyright (c) 2007 Daniel Gollub <dgollub@suse.de>
#  Copyright (c) 2007 Bjoern Ricks  <b.ricks@fh-osnabrueck.de>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.


# INCLUDE( FindPkgConfig )

# # Take care about check.pc settings
# PKG_SEARCH_MODULE( CHECK Check )

# # Look for CHECK include dir and libraries
# IF( NOT CHECK_FOUND )
# 	IF ( CHECK_INSTALL_DIR )
# 		MESSAGE ( STATUS "Using override CHECK_INSTALL_DIR to find Check" )
# 		SET ( CHECK_INCLUDE_DIR  "${CHECK_INSTALL_DIR}/include" )
# 		SET ( CHECK_INCLUDE_DIRS "${CHECK_INCLUDE_DIR}" )
# 		FIND_LIBRARY( CHECK_LIBRARY NAMES check PATHS "${CHECK_INSTALL_DIR}/lib" )
# 		FIND_LIBRARY( COMPAT_LIBRARY NAMES compat PATHS "${CHECK_INSTALL_DIR}/lib" )
# 		SET ( CHECK_LIBRARIES "${CHECK_LIBRARY}" "${COMPAT_LIBRARY}" )
# 	ELSE ( CHECK_INSTALL_DIR )
# 		FIND_PATH( CHECK_INCLUDE_DIR check.h )
# 		FIND_LIBRARY( CHECK_LIBRARIES NAMES check )
# 	ENDIF ( CHECK_INSTALL_DIR )

# 	IF ( CHECK_INCLUDE_DIR AND CHECK_LIBRARIES )
# 		SET( CHECK_FOUND 1 )
# 		IF ( NOT Check_FIND_QUIETLY )
# 			MESSAGE ( STATUS "Found CHECK: ${CHECK_LIBRARIES}" )
# 		ENDIF ( NOT Check_FIND_QUIETLY )
# 	ELSE ( CHECK_INCLUDE_DIR AND CHECK_LIBRARIES )
# 		IF ( Check_FIND_REQUIRED )
# 			MESSAGE( FATAL_ERROR "Could NOT find CHECK" )
# 		ELSE ( Check_FIND_REQUIRED )
# 			IF ( NOT Check_FIND_QUIETLY )
# 				MESSAGE( STATUS "Could NOT find CHECK" )	
# 			ENDIF ( NOT Check_FIND_QUIETLY )
# 		ENDIF ( Check_FIND_REQUIRED )
# 	ENDIF ( CHECK_INCLUDE_DIR AND CHECK_LIBRARIES )
# ENDIF( NOT CHECK_FOUND )

# # Hide advanced variables from CMake GUIs
# MARK_AS_ADVANCED( CHECK_INCLUDE_DIR CHECK_LIBRARIES )



#[=======================================================================[.rst:
FindCheck
--------

Find the Check (Unit Testing Framework for C) library

Imported targets
^^^^^^^^^^^^^^^^

This module defines the following :prop_tgt:`IMPORTED` targets:

``Check::Check``
  The Check library, if found.

Result variables
^^^^^^^^^^^^^^^^

This module will set the following variables in your project:

``Check_FOUND``
  If false, do not try to use Check.
``CHECK_INCLUDE_DIR``
  where to find check.h, etc.
``CHECK_LIBRARIES``
  the libraries needed to use Check.
``CHECK_VERSION``
  the version of the Check library found

#]=======================================================================]

find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(PkgCheck IMPORTED_TARGET GLOBAL check)
endif ()

if (PkgCheck_FOUND)
  set(CHECK_INCLUDE_DIR ${PkgCheck_INCLUDE_DIRS} CACHE FILEPATH "check include path")
  set(CHECK_LIBRARIES ${PkgCheck_LIBRARIES} CACHE STRING "check libraries")
  set(CHECK_VERSION ${PkgCheck_VERSION})
  add_library(Check::Check ALIAS PkgConfig::PkgCheck)
  set(Check_FOUND ON)
else ()
  find_path(CHECK_INCLUDE_DIR check.h
    HINTS
    "${CHECK_DIR}"
    "${CHECK_DIR}/include"
    )

  # Check for PIC and non-PIC libraries. If PIC present, use that
  # in preference (as per Debian check.pc).
  find_library(CHECK_LIBRARY NAMES check_pic libcheck_pic
    HINTS
    "${CHECK_DIR}"
    "${CHECK_DIR}/lib"
    )

  if (NOT CHECK_LIBRARY)
    find_library(CHECK_LIBRARY NAMES check libcheck
      HINTS
      "${CHECK_DIR}"
      "${CHECK_DIR}/lib"
      )
  endif ()

  set(_CHECK_LIBARIES "")

  # Check may need the math, subunit and rt libraries on Unix
  if (UNIX)
    find_library(CHECK_MATH_LIBRARY m)
    find_library(CHECK_RT_LIBRARY rt)
    find_library(CHECK_SUBUNIT_LIBRARY subunit)

    if (CHECK_MATH_LIBRARY)
      list(APPEND _CHECK_LIBARIES "${CHECK_MATH_LIBRARY}")
    endif ()
    if (CHECK_RT_LIBRARY)
      list(APPEND _CHECK_LIBARIES "${CHECK_RT_LIBRARY}")
    endif ()
    if (CHECK_SUBUNIT_LIBRARY)
      list(APPEND _CHECK_LIBARIES "${CHECK_SUBUNIT_LIBRARY}")
    endif ()
  endif()

  set(CHECK_LIBRARIES ${_CHECK_LIBARIES} ${CHECK_LIBRARY} CACHE STRING "check libraries")

  if (CHECK_INCLUDE_DIR AND CHECK_LIBRARY)
    if (NOT TARGET Check::Check)
      add_library(Check::Check UNKNOWN IMPORTED)
      set_target_properties(Check::Check PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CHECK_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${CHECK_LIBRARIES}"
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${CHECK_LIBRARY}"
        )
    endif ()

    if (NOT CHECK_VERSION AND CHECK_INCLUDE_DIR AND EXISTS "${CHECK_INCLUDE_DIR}/check.h")
      file(STRINGS "${CHECK_INCLUDE_DIR}/check.h" CHECK_H REGEX "^#define CHECK_M[A-Z]+_VERSION")
      string(REGEX REPLACE "^.*\(([0-9]+)\).*\(([0-9]+)\).*\(([0-9]+)\).*$" "\\1.\\2.\\3" CHECK_VERSION "${CHECK_H}")
    endif ()
  endif()

  list(APPEND CHECK_LIBRARIES "${CHECK_LIBRARY}")

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(Check
    REQUIRED_VARS CHECK_LIBRARIES CHECK_INCLUDE_DIR
    VERSION_VAR CHECK_VERSION
    )

endif()

mark_as_advanced(CHECK_INCLUDE_DIR CHECK_LIBRARIES CHECK_LIBRARY
  CHECK_MATH_LIBRARY CHECK_RT_LIBRARY CHECK_SUBUNIT_LIBRARY)
