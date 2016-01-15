#

# Try to find OGLPLUS library and include path.
# Once done this will define
#
# OGLPLUS_FOUND
# OGLPLUS_INCLUDE_DIR
#
include(FindPackageHandleStandardArgs)

if (WIN32)

    find_path( OGLPLUS_INCLUDE_DIR
        NAMES
            oglplus/all.hpp
        PATHS
            ${OGLPLUS_LOCATION}/include
            $ENV{OGLPLUS_LOCATION}/include
            $ENV{PROGRAMFILES}/OGLPLUS/include
            ${PROJECT_SOURCE_DIR}/extern/glew/include
            ${OGLPLUS_LOCATION}
            $ENV{OGLPLUS_LOCATION}
            DOC "The directory where oglplus/all.hpp resides" )

endif ()

if (${CMAKE_HOST_UNIX})
    find_path( OGLPLUS_INCLUDE_DIR
        NAMES
            oglplus/all.hpp
        PATHS
            ${OGLPLUS_LOCATION}/include
            $ENV{OGLPLUS_LOCATION}/include
            /usr/include
            /usr/local/include
            /sw/include
            /opt/local/include
            NO_DEFAULT_PATH
            DOC "The directory where oglplus/all.hpp resides"
    )
endif ()

find_package_handle_standard_args(OGLPLUS DEFAULT_MSG
    OGLPLUS_INCLUDE_DIR
)
mark_as_advanced( OGLPLUS_FOUND )