cmake_minimum_required(VERSION 3.10)
project(Steam)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

set(COMMON_MACROS STEAM_EXPORTS GNUC POSIX _POSIX _FILE_OFFSET_BITS=64 Z_HAVE_UNISTD_H)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
	set(PLATFORM_MACROS LINUX _LINUX)
elseif(CMAKE_SYSTEM_NAME MATCHES Darwin)
	set(PLATFORM_MACROS OSX _OSX)
endif()

add_library(steam SHARED)
target_compile_definitions(steam PRIVATE ${COMMON_MACROS} ${PLATFORM_MACROS})
target_link_libraries(steam PRIVATE dl pthread)
if(CMAKE_SYSTEM_NAME MATCHES Darwin)
	set_target_properties(steam PROPERTIES VERSION 1.0.0 SOVERSION 1)
endif()

file(GLOB MAIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB ZLIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/zlib/*.c")
list(REMOVE_ITEM MAIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/registry.cpp")
list(REMOVE_ITEM MAIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Win32MiniDump.cpp")
target_sources(steam PRIVATE
${MAIN_SOURCES}
${ZLIB_SOURCES}
)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
	add_library(steamvalidateuseridtickets SHARED Steam.cpp logFile.cpp strtools.cpp strtools_unicode.cpp)
	target_compile_definitions(steamvalidateuseridtickets PRIVATE VALIDATOR_DLL ${COMMON_MACROS} ${PLATFORM_MACROS})
	target_link_libraries(steamvalidateuseridtickets PRIVATE dl pthread)
endif()
