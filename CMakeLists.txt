cmake_minimum_required(VERSION 3.1)

project(fantastic-train)

find_library(PTHREAD_LIBRARY pthread)
find_library(GL_LIBRARY GL)
find_library(GLU_LIBRARY GLU)
find_library(X11_LIBRARY X11)
find_library(GLUT_LIBRARY glut)
find_library(PNG_LIBRARY png)
find_library(FTGL_LIBRARY ftgl)

 include_directories(Includes)

 set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -Wall -Wextra -std=gnu++11 -g")

add_executable(main Src/main.cpp Src/bases.cpp Src/routines.cpp Src/math.cpp Src/geometry.cpp Src/grapher.cpp Src/pathfinder.cpp)

 target_link_libraries(main ${PTHREAD_LIBRARY} ${GL_LIBRARY} ${GLU_LIBRARY} ${GLUT_LIBRARY} ${X11_LIBRARY} ${PNG_LIBRARY} ${FTGL_LIBRARY} )
