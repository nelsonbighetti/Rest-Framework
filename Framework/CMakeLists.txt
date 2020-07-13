set(TARGET_NAME Framework)
cmake_minimum_required(VERSION 3.12)
project(Framework LANGUAGES CXX)

include(sources.cmake)
add_executable(${TARGET_NAME} ${SOURCES})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET_NAME} PUBLIC ${Boost_include_path})
target_include_directories(${TARGET_NAME} PUBLIC ${libpq_include_path})
target_link_directories(${TARGET_NAME} PUBLIC ${Boost_lib_path})

if(WIN32)
    target_link_libraries(${TARGET_NAME} PRIVATE ${libpq_lib_path}/libpq.lib)
endif()

if(NOT WIN32)
    target_link_libraries(${TARGET_NAME} PRIVATE ${libpq_lib_path}/libpq.lib)
endif()