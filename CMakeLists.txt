CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# Maps to a solution file (Tutorial.sln). The solution will
# have all targets (exe, lib, dll) as projects (.vcproj)
PROJECT(NUPresenceClient)

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# We use additional modules that cmake needs to know about
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Find some shared packages we need
FIND_PACKAGE(OculusSDK REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(GLFW REQUIRED)
FIND_PACKAGE(OOGL REQUIRED)

# Set include directories and libraries:
INCLUDE_DIRECTORIES(SYSTEM ${OCULUS_SDK_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${GLEW_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${OOGL_INCLUDE_DIR})

SET(NUCLEAR_ADDITIONAL_SHARED_LIBRARIES
    ${OCULUS_SDK_LIBRARIES}
    ${GLEW_SDK_LIBRARIES}
    ${OOGL_LIBRARY}
)

# Initialise NUClear
ADD_SUBDIRECTORY(nuclear)

# file(GLOB SRC_FILES src/*.cpp)
# add_executable(client ${SRC_FILES})

include_directories(${OCULUS_SDK_INCLUDE_DIRS} SYSTEM)
include_directories(${GLEW_INCLUDE_DIRS} SYSTEM)
include_directories(${OOGL_INCLUDE_DIR} SYSTEM)

# target_link_libraries(client wsock32 ws2_32 
# 	${OCULUS_SDK_LIBRARIES}
# 	${GLEW_SDK_LIBRARIES}
# 	${OOGL_LIBRARY}
# 	)
