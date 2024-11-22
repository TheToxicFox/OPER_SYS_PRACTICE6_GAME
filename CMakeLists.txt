cmake_minimum_required(VERSION 3.10)
project(GPraktika6_GAME VERSION 1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SERVER_SOURCE "server.c")
set(CLIENT_SOURCE "client.c")

add_executable(server ${SERVER_SOURCE})


add_executable(client ${CLIENT_SOURCE})


target_link_libraries(server PRIVATE pthread)
target_link_libraries(client PRIVATE pthread)
