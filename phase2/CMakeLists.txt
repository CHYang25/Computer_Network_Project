cmake_minimum_required(VERSION 3.2) # current 3.22.1
project( phase2 )

# find_package(nlohmann_json REQUIRED)

include_directories( ./include ./include/nlohmann)

add_library(${PROJECT_NAME} SHARED
           ./src/server/server.cpp
           ./src/server/httpm.cpp
)

add_executable( server ./src/server/server.cpp ./src/server/httpm.cpp)
# target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
