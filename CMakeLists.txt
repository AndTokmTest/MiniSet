cmake_minimum_required(VERSION 3.0)
project(MiniSet)

set(CMAKE_CXX_STANDARD 26)

# add_compile_options(-c -Wall -Wextra -O3 -std=c++26)
add_compile_options(-c -Wall -O3 -std=c++26)

# include all components
add_executable(${PROJECT_NAME}
        main.cpp
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        pthread
)