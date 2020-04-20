# fantastic-train
Library for searching shortest way in a field of obstacles using Dijkstra's algorithm
  
## How to add this thing to your CMake project:
* Clone project
  ``` 
  $ git clone https://github.com/niktob560/fantastic-train 
  ```
* Get the waypointmanager library:
  ```
  $ git submodule init 
  $ git submodule update 
  ```
* Compile waypointmanager library:
     ```
     $ mkdir waypointmanager/Build
     $ cd waypointmanager/Build
     $ cmake ..
     $ make waypointmanager
     $ cd -
     ```
* Compile this library:
     ```
     $ mkdir Build
     $ cd Build
     $ cmake ..
     $ make fantastic-train
     ```
* Change your CMakeLists.txt:
  * ``LINK_DIRECTORIES(/path/to/thing/Build/)`` 
  * ``add_library(libfantastic-train SHARED IMPORTED)``
  * ``add_dependencies(libfantastic-train /path/to/thing/Build/libfantastic-train.so)``
  * ``target_link_libraries(target libfantastic-train.so)``
* Be awesome
