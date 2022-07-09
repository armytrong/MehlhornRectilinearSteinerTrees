#pragma once

//FIXED das Resultat returnen, nicht output parameter
//FIXED variablen ausschreiben
//FIXED definition in das c-file und wenn nicht, muss die Funktion inline sein
//Als kruskal routine ok, aber es wäre sinnvoll noch eine Instanz-klasse oder so bereitzustellen und dann die als input zu nehmen in einer
//approximate_steiner_tree funktion oder so

class Graph;

class DelaunayGraph;

Graph kruskal(const Graph &input_graph);
DelaunayGraph kruskal(const DelaunayGraph &delaunay_graph);

