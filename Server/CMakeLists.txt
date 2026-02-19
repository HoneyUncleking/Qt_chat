cmake_minimum_required(VERSION 3.16)
project(KalaNetServer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# پیدا کردن Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Network)

# مشخص کردن پوشه Common
set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Common)

set(SOURCES
    main.cpp
    Database.cpp
    ServerHandler.cpp
    ClientHandler.cpp
    Server.cpp
)

set(HEADERS
    Database.h
    ServerHandler.h
    ClientHandler.h
    Server.h
    ${COMMON_DIR}/Protocol.h
    ${COMMON_DIR}/HashUtils.h
    ${COMMON_DIR}/ValidationUtils.h
)

add_executable(KalaNetServer ${SOURCES} ${HEADERS})

target_include_directories(KalaNetServer PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${COMMON_DIR}
)

target_link_libraries(KalaNetServer Qt6::Core Qt6::Network)