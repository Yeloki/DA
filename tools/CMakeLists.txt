cmake_minimum_required(VERSION 3.20)
project(tools_test)

set(CMAKE_CXX_STANDARD 20)


set(SRC_EXTRA
        containers/vector.hpp containers/string.hpp
        containers/vector_tools.hpp
        containers/avl_tree.hpp
        )

set(MAIN_EXEC main.cpp ${SRC_EXTRA})
add_executable(${PROJECT_NAME} ${MAIN_EXEC})

