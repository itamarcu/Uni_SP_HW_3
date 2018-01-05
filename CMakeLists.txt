cmake_minimum_required(VERSION 3.9)
project(Software_Project_3)

set(CMAKE_CXX_STANDARD 11)

add_executable(Software_Project_3
        main.c
        makefile
        SPArrayList.c
        SPArrayList.h
        SPArrayListUnitTest.c
        SPFIARGame.c
        SPFIARGame.h
        SPFIARGameUnitTest.c
        SPFIARParser.c
        SPFIARParser.h
        SPFIARParserUnitTest.c
        SPMainAux.c
        SPMainAux.h
        SPMinimax.c
        SPMinimax.h
        SPMinimaxNode.c
        SPMinimaxNode.h
        SPMinimaxNodeUnitTest.c
        SPMinimaxUnitTest.c
        unit_test_util.h)
