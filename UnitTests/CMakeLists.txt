set(TARGET_NAME FrameworkTests)
cmake_minimum_required(VERSION 3.12)
project(FrameworkTests LANGUAGES CXX)

include(sources.cmake)
add_executable(${TARGET_NAME} ${SOURCES})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}\\..\\Framework)
target_include_directories(${TARGET_NAME} PUBLIC ${Boost_include_path})
target_include_directories(${TARGET_NAME} PUBLIC ${libpq_include_path})
target_include_directories(${TARGET_NAME} PUBLIC ${gtest_include})

target_link_directories(${TARGET_NAME} PUBLIC ${Boost_lib_path})


if(WIN32)
    target_link_libraries(${TARGET_NAME} PRIVATE ${libpq_lib_path}/libpq.lib)
    target_link_libraries(${TARGET_NAME} PRIVATE ${gtest_lib_path}/gtest_maind.lib)
    target_link_libraries(${TARGET_NAME} PRIVATE ${gtest_lib_path}/gtestd.lib)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

if(NOT WIN32)
    target_link_libraries(${TARGET_NAME} PRIVATE ${libpq_lib_path}/libpq.a)
    target_link_libraries(${TARGET_NAME} PRIVATE ${gtest_lib_path}/gtest_maind.a)
    target_link_libraries(${TARGET_NAME} PRIVATE ${gtest_lib_path}/gtestd.a)
endif()