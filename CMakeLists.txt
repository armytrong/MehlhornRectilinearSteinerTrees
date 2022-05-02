cmake_minimum_required(VERSION 3.16)
project(Hauptaufgabe_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Hauptaufgabe_2 main.cpp SteinerTreeAproximator.cpp SteinerTreeAproximator.h STPFileParser.cpp STPFileParser.h dijkstra/DijkstraGraph.cpp dijkstra/DijkstraGraph.h graph/Graph.cpp graph/Graph.h graph/Edge.cpp graph/Edge.h dijkstra/FibonacciHeap.cpp dijkstra/FibonacciHeap.h)
