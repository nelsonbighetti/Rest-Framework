
# RestFramework

## Description

At the moment, this solution allows you to process the simplest HTTP request and convert it into a request to the PostgreSQL database. In the future, it is planned to expand the number of supported queries and database systems.

## Requirements:

+ Boost 1.68
+ LibPQ 12 
+ GTest (for unit-tests)

## Build instructions:

1. Open root CMakeLists.txt
2. Specify paths: 
+ Boost_include_path
+ Boost_lib_path
+ libpq_include_path
+ libpq_lib_path
+ gtest_include
+ gtest_lib_path
3. Uncomment "set" instuctions 
4. Run CMake 
5. Build project


## Project structure:

**Framework:**
 - HttpFsm      - Implementeation of Finite State Machine. Parses HTTP requests and converts them to database requests. Functionality can be easily extended by adding additional states. Multiple instances are allowed.
 - PgConnection - PostgreSQL database connection layer.
 - TcpServer    - Simple TCP Server, implemented on Boost Asio.
 
**UnitTests** - Unit-tests for Finite State Machine. 
