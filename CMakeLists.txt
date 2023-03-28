cmake_minimum_required(VERSION 3.10)

project(my_project)

# Set C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add server executable
add_executable(server
    Server/Server.cpp
)

# Add client executable
add_executable(client
    Client/Client.cpp
    Client/Client.h
    Client/Threads.cpp
    Client/Threads.h
)

# Link client executable with threads library
target_link_libraries(client
    pthread
)
