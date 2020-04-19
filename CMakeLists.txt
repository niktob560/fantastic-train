cmake_minimum_required(VERSION 3.1)

project(fantastic-train)

include_directories(Includes ./waypointmanager/Inc ./waypointmanager ./)


find_library(PTHREAD_LIBRARY pthread)
find_library(GL_LIBRARY GL)
find_library(GLU_LIBRARY GLU)
find_library(X11_LIBRARY X11)
find_library(GLUT_LIBRARY glut)
find_library(PNG_LIBRARY png)
find_library(FTGL_LIBRARY ftgl)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -std=gnu++11 -g -Wparentheses -Wsequence-point -Wreturn-type -Wswitch -Wuninitialized -Wno-write-strings -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wduplicated-branches -Wduplicated-cond -Wextra-semi -Wfloat-equal -Wlogical-op -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo -Werror -IInclude -Wall -Wextra -Wpedantic -pedantic-errors -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -fno-devirtualize -fno-use-cxa-atexit ")
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_executable(main Src/main.cpp Src/bases.cpp Src/routines.cpp Src/math.cpp Src/geometry.cpp Src/grapher.cpp Src/pathfinder.cpp)

target_link_libraries(main ${PTHREAD_LIBRARY} ${GL_LIBRARY} ${GLU_LIBRARY} ${GLUT_LIBRARY} ${X11_LIBRARY} ${PNG_LIBRARY} ${FTGL_LIBRARY})



set_target_properties(main PROPERTIES
      IMPORTED_LOCATION ~/Documents/C++/waypointmanager/Build/libwaypointmanager.so
)
