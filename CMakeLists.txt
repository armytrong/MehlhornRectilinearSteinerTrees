cmake_minimum_required(VERSION 3.16)
project(Hauptaufgabe_2)

set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fsanitize=address -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o3")
include_directories(.)

add_executable(Hauptaufgabe_2
        main.cpp
        STPFileParser.cpp STPFileParser.h
        dijkstra/DijkstraGraph.cpp
        dijkstra/DijkstraGraph.h
        heap/FibonacciHeap.cpp
        heap/FibonacciHeap.h
        typedefs.h
        heap/StandardHeap.cpp
        heap/StandardHeap.h
        heap/PriorityQueue.h
        delaunay/DelaunayGraph.cpp
        delaunay/DelaunayGraph.h
        delaunay/DelaunaySet.cpp
        delaunay/DelaunaySet.h
        delaunay/DelaunayPriorityQueue.cpp
        delaunay/DelaunayPriorityQueue.h
        ArgumentHandler.cpp
        ArgumentHandler.h mehlhorn/MehlhornGraph.cpp mehlhorn/MehlhornGraph.h graph/CoordinateGraph.cpp graph/CoordinateGraph.h)
