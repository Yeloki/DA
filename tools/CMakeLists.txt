cmake_minimum_required(VERSION 3.20)
project(tools_test)

set(CMAKE_CXX_STANDARD 23)


set(SRC_EXTRA)

set(MAIN_EXEC src/main.cpp ${SRC_EXTRA})
add_executable(${PROJECT_NAME} ${MAIN_EXEC})

