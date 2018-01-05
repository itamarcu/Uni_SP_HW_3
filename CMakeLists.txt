cmake_minimum_required(VERSION 3.8)
project(C_hw_3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        makefile
        main.c
        SPArrayList.c
        SPArrayList.h
        SPArrayListUnitTest.c
        SPFIARGame.c
        SPFIARGame.h
        SPFIARGameUnitTest.c
        SPFIARParser.c
        SPFIARParser.h
        SPFIARParserUnitTest.c
        SPMinimax.c
        SPMinimax.h
        SPMinimaxNode.c
        SPMinimaxNode.h
        SPMainAux.c
        SPMainAux.h
        SPMinimaxUnitTest.c
        SPMinimaxNodeUnitTest.c)

add_executable(C_hw_3 ${SOURCE_FILES})