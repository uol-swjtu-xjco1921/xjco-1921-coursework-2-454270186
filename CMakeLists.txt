cmake_minimum_required(VERSION 3.0)
project(Xiaofei_map)

# add include path
include_directories(
    ./include
    /usr/include/plplot
    ./test
)

# add src file
file(GLOB SOURCES "./src/*.c" "./test/unit_test.c")

# gtk3
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# add excutable file
add_executable(main main.c ${SOURCES})
add_definitions(-DRUN_MODE=${RUN_MODE})

target_compile_options(main PRIVATE -g)

target_link_libraries(main -lplplot ${GTK3_LIBRARIES})
target_include_directories(main PRIVATE ${GTK3_INCLUDE_DIRS})