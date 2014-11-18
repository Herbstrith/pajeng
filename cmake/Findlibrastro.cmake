#
# Find the LIBRASTRO library
#
# LIBRASTRO_INCLUDE_PATH - Directories to include to use LIBRASTRO
# LIBRASTRO_LIBRARY_PATH - Files to link against to use LIBRASTRO
# LIBRASTRO_FOUND - When false, don't try to use LIBRASTRO
#
# LIBRASTRO_PATH can be used to make it simpler to find the various include
# directories and compiled libraries when LIBRASTRO was not installed in the
# usual/well-known directories (e.g. because you made an in tree-source
# compilation or because you installed it in an "unusual" directory).
# Just set LIBRASTRO_PATH it to your specific installation directory
#
FIND_LIBRARY(LIBRASTRO_LIBRARY
NAMES rastro
PATHS /usr/lib /usr/local/lib ${LIBRASTRO_PATH}/lib)
IF(LIBRASTRO_LIBRARY)
MESSAGE ( STATUS "Found LIBRASTRO library: ${LIBRASTRO_LIBRARY}" )
GET_FILENAME_COMPONENT(LIBRASTRO_LIBRARY_tmp "${LIBRASTRO_LIBRARY}" PATH)
SET (LIBRASTRO_LIBRARY_PATH ${LIBRASTRO_LIBRARY_tmp})
ENDIF(LIBRASTRO_LIBRARY)
FIND_PATH( LIBRASTRO_INCLUDE_tmp rastro.h
PATHS
${LIBRASTRO_GUESSED_INCLUDE_PATH}
${LIBRASTRO_PATH}/include/
/usr/include/
/usr/local/include/
)
IF(LIBRASTRO_INCLUDE_tmp)
SET (LIBRASTRO_INCLUDE_PATH "${LIBRASTRO_INCLUDE_tmp}" CACHE PATH "")
ENDIF(LIBRASTRO_INCLUDE_tmp)
IF( LIBRASTRO_INCLUDE_PATH )
IF( LIBRASTRO_LIBRARY_PATH )
SET( LIBRASTRO_FOUND TRUE )
ENDIF ( LIBRASTRO_LIBRARY_PATH )
ENDIF( LIBRASTRO_INCLUDE_PATH )
IF( NOT LIBRASTRO_FOUND )
MESSAGE(STATUS "LIBRASTRO installation was not found. Please provide LIBRASTRO_PATH:")
MESSAGE(STATUS " - through the GUI when working with ccmake, ")
MESSAGE(STATUS " - as a command line argument when working with cmake e.g.")
MESSAGE(STATUS " cmake .. -DLIBRASTRO_PATH:PATH=/usr/local/librastro/ ")
SET(LIBRASTRO_PATH "" CACHE PATH "Root of LIBRASTRO install tree." )
ENDIF( NOT LIBRASTRO_FOUND )









