/* Authors: Arash Rafiey & Thiago Santos. --> https://github.com/thiagosantos1/  */
 
/*************************************************************************** 
* After creating a random graph G & H, we may end up with a list homomorphism with dead vertices. 
* Thus, we must run some kind of list consistency
* The idea of using bfs is to make sure that from a vertice a from H we can reach another vertice b in H, 
* where a is in the list of x and b is the list of z and there is a path from x to z in G.
* 
*
* The Idea of this implementation is to solve linear equations, using 
* Graph and Maltsev theory. After appling the theory, the graph and
* its list left can be used to find a solution for the linear equation
* Using Homomorphism.
*  
* Copyright (C)
* 
****************************************************************************/

typedef struct bfsdata {

  int ** graph;   // This is a new graph G*H X G*H. for each vertice in G and for each vertice in H, we create a new vertice in this new graph
  int *  queue;   // for bfs exploration
  int *  visited; // also used to control de exploration
  int    front;   // to remove the next one in the front of queue
  int    rear;    // to insert at end of queue

  int numVert;
  int numEdges;

} BFS_DATA;


// main function for bfs list consistency, to be called by other functions
void bfs_list_consis(BFS_DATA *bf, NEW_GRAPHS *op);

// Based on the current graph G & H and list, we then construct the new graph 
void construct_bfs_graph(BFS_DATA *bf, NEW_GRAPHS *op);

void print_graph(BFS_DATA *bf, NEW_GRAPHS *op);

// return the pair {x_, y_} encode for x,y & a,b 
int * encode(int x, int y, int a, int b);

// return the decode {x,y}, {a,b} for the encode {x_, y_} 
int * decode(int x_, int y_);