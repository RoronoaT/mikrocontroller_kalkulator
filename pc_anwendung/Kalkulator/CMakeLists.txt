cmake_minimum_required(VERSION 3.10)
project(MeuProjetoWxWidgets)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})


set(SOURCES
    src/main.cpp
    src/frame.cpp
)

set(HEADERS
    src/main.h
    src/frame.h
)

add_executable(kalkulator ${SOURCES} ${HEADERS})

target_link_libraries(kalkulator ${wxWidgets_LIBRARIES})
