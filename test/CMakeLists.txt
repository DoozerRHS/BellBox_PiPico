cmake_minimum_required(VERSION 3.30)

#Cmake for BellBoxUnit Tester Code - generates a googletest EXE that you can Cmake RunTests with

#variable for this project name
set (This BellBoxTests)

set(Sources 
    BellBoxButtonTests.cpp
    BellBoxBellTests.cpp
)

add_executable(${This} ${Sources} )

target_link_libraries(${This} PUBLIC
    gtest_main
    BellBoxTest)


add_test(
    NAME ${This}
    COMMAND ${This}
)