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


# Initialise NUClear
ADD_SUBDIRECTORY(nuclear)

# file(GLOB SRC_FILES src/*.cpp)
# add_executable(client ${SRC_FILES})

# target_link_libraries(client wsock32 ws2_32 
# 	${OCULUS_SDK_LIBRARIES}
# 	${GLEW_SDK_LIBRARIES}
# 	${OOGL_LIBRARY}
# 	)
