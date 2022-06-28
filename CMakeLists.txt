cmake_minimum_required(VERSION 3.16)
project(Hauptaufgabe_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Hauptaufgabe_2
        main.cpp
        SteinerTreeApproximator.cpp
        SteinerTreeApproximator.h
        STPFileParser.cpp STPFileParser.h
        dijkstra/DijkstraGraph.cpp
        dijkstra/DijkstraGraph.h
        graph/Graph.cpp graph/Graph.h
        heap/FibonacciHeap.cpp
        heap/FibonacciHeap.h
        typedefs.h
        MehlhornDelaunayGraphFactory.cpp
        MehlhornDelaunayGraphFactory.h
        kruskal/kruskal_ind.cpp
        kruskal/kruskal_ind.h
        heap/StandardHeap.cpp
        heap/StandardHeap.h
        graph/RectilinearGraph.cpp
        graph/RectilinearGraph.h
        heap/BinaryHeap.cpp heap/BinaryHeap.h heap/Heap.h googletest/MainTest.cpp delaunay/DelaunayGraph.cpp delaunay/DelaunayGraph.h delaunay/DelaunaySet.cpp delaunay/DelaunaySet.h delaunay/DelaunayPriorityQueue.cpp delaunay/DelaunayPriorityQueue.h)


add_subdirectory(googletest)
