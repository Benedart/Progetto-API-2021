# Prova Finale di API 2020/21

This is my submission for the "Progetto di API 2020/21" at Politecnico di Milano.

Grade: 30L

## GraphRanker

The goal of the "Prova Finale di API 2021" is to manage a ranking between weighted direct graphs.
The ranking keeps track of the k "best" graphs

The program will receive in input two parameters, only once (on the first row of the file, separated by a space):
- d: the number of nodes of the graphs
- k: the length of the ranking

A sequence of commands between
- AggiungiGrafo [adjacency-matrix]
- TopK

d, k and the number of graphs are representable with 32-bit integers. 

### AggiungiGrafo

Requires adding a graph to those considered to draw up the ranking. It is followed by the adjacency matrix of the graph itself, printed one row for each line, with the elements separated by commas.

The nodes of the graph are to be considered logically labeled with an index integer between 0 and d-1; the node in position 0 is the one whose outgoing star is described by the first row of the matrix.
The weights of the arcs of the element graph are integers in the interval [0, 2^32 - 1]. 

### TopK

Consider each graph from the beginning of the program up to the TopK command labeled with an integer index corresponding to the number of graphs read before it (starting from 0)
TopK prompts the program to print the integer indices of k graphs having k values smaller of the following metric:
- Sum of the shortest paths between node 0 and all the other nodes of the graph reachable from 0
- If there are multiple graphs with the same metric value, the first arrived gets the precedence
- The distances of the nodes not reachable from 0 are considered null
- The k integer indices are printed, on a single line, separated by a space, in any order 
