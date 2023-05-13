cmake_minimum_required(VERSION 3.20)
project(lab-1)

set(CMAKE_CXX_STANDARD 20)

include_directories(../tools)

set(SRC_EXTRA
        ../tools/containers/vector.hpp
        ../tools/containers/string.hpp
        ../tools/containers/vector_tools.hpp
        )

#set(MAIN_EXEC src/main.cpp ${SRC_EXTRA})
set(MAIN_EXEC src/result.cpp)
add_executable(${PROJECT_NAME} ${MAIN_EXEC})

