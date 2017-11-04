cmake_minimum_required (VERSION 3.1 FATAL_ERROR)

# Include cmake scripts
include(cmake/GetGitRevisionDescription.cmake)
include(cmake/UninstallTarget.cmake)

# Get git revision
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
string(SUBSTRING "${GIT_SHA1}" 0 12 GIT_REV)

# TODO(kjayakum): Generate header file with relevant meta data
# Meta information about the project
set(META_PROJECT_NAME			"xbee_plus")
set(META_PROJECT_DESCRIPTION	"Cross-Platform C++11 Library for Xbee Radios")
set(META_AUTHOR_ORGANIZATION	"Northrop Grumman Collaboration Project, Cal Poly")
set(META_AUTHOR_DOMAIN			"https://github.com/NGCP/xbeeplus")
set(META_AUTHOR_MAINTAINER		"kjayakum@calpoly.edu")
set(META_VERSION_MAJOR			"1")
set(META_VERSION_MINOR			"0")
set(META_VERSION_PATCH			"0")
set(META_VERSION_REVISION		"${GIT_REV}")
set(META_VERSION				"${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_NAME_VERSION			"${META_PROJECT_NAME} v${META_VERSION} (${META_VERSION_REVISION})")
set(CMAKE_CXX_STANDARD			11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project options
option(BUILD_SHARED_LIBS		"Build shared instead of static libraries."					ON)
#option(OPTION_SELF_CONTAINED	"Create a self-contained install with all dependencies."	OFF)
#option(OPTION_BUILD_TESTS		"Build tests."												OFF)
#option(OPTION_BUILD_DOCS		"Build documentation."										OFF)
option(OPTION_BUILD_EXAMPLES	"Build examples."											OFF)

# Get BOOST Libraries
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(Threads REQUIRED)

# Set BOOST options
set(Boost_USE_STATIC_LIBS			OFF)
set(Boost_USE_MULTITHREADED			ON)
set(Boost_USE_STATIC_RUNTIME		OFF)
set(Boost_ALL_DYN_LINK				ON)   # force dynamic linking for all libraries

# Set project name
set(project ${META_PROJECT_NAME})

# Set all library sources
set(LIB_SOURCES
	src/ReceivePacket.cpp
	src/TransmitRequest.cpp
	src/Utility.cpp
	src/SerialXbee.cpp
)

# Set all library headers
set(LIB_HEADERS
	include/Xbee.hpp
	include/Frame.hpp
	include/RecievePacket.hpp
	include/TransmitRequest.hpp
	include/Utility.hpp
#	include/SerialXbee.hpp
)
include_directories(${Boost_INCLUDE_DIRS})

# TODO(kjayakum): Support Cross-Platform Builds
# TODO(kjayakum): Allow '-g' flag to be set by cmake DEBUG variable
# Library target
add_library(${META_PROJECT_NAME} SHARED ${LIB_SOURCES})

# Compile options for library
target_compile_options(${META_PROJECT_NAME} PUBLIC -fPIC -Wall -Werror -g)

# Link targets for library 
target_link_libraries(${META_PROJECT_NAME} PUBLIC Threads::Threads)
target_link_libraries(${META_PROJECT_NAME} PUBLIC	${Boost_THREAD_LIBRARY}
													${Boost_SYSTEM_LIBRARY})

# Install target for library
install(TARGETS ${META_PROJECT_NAME}
		LIBRARY DESTINATION lib
		RUNTIME DESTINATION bin
		)

# Uninstall target for library
add_uninstall_target()

# TODO(kjayakum): Support Cross-Platform builds
# TODO(kjayakum): Enable this to be built via cmake test option
# Test target
# add_executable(xbeetest test/xbeetest.cpp)

# Compile options for test
#target_compile_options(xbeetest PUBLIC -Wall -Werror -g)

# Link targets for test
#target_link_libraries(xbeetest PUBLIC -lboost_system -lpthread -lboost_thread -lxbee_plus)

# Check for system dir install
#set(SYSTEM_DIR_INSTALL FALSE)
#if("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr" OR "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
#    set(SYSTEM_DIR_INSTALL TRUE)
#endif()