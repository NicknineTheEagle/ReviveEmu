cmake_minimum_required(VERSION 3.1)
project(TicketValidator)

set(CMAKE_CXX_STANDARD 11)

add_library(steamvalidateuseridtickets SHARED Steam.cpp logFile.cpp strtools.cpp strtools_unicode.cpp)
target_compile_definitions(steamvalidateuseridtickets PRIVATE VALIDATOR_DLL STEAM_EXPORTS)
target_link_libraries(steamvalidateuseridtickets PRIVATE dl pthread)
