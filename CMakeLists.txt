cmake_minimum_required(VERSION 3.16)
project(Hauptaufgabe_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        Hauptaufgabe_2
        main.cpp
        SteinerTreeApproximator.cpp
        SteinerTreeApproximator.h
        STPFileParser.cpp STPFileParser.h
        dijkstra/DijkstraGraph.cpp
        dijkstra/DijkstraGraph.h
        graph/Graph.cpp graph/Graph.h
        dijkstra/FibonacciHeap.cpp
        dijkstra/FibonacciHeap.h
        typedefs.h
        DelaunayGraphFactory.cpp
        DelaunayGraphFactory.h
        kruskal/kruskal_ind.cpp
        kruskal/kruskal_ind.h
        dijkstra/StandardHeap.cpp
        dijkstra/StandardHeap.h
        graph/RectilinearGraph.cpp
        graph/RectilinearGraph.h
)
