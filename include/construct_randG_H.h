/* Authors: Arash Rafiey & Thiago Santos. --> https://github.com/thiagosantos1/  */

/***************************************************************************
* This class is designed to construct a random graph G & H.
* G & H are random. However, H has to have/preserve some properties, such as 
* the rectangle property. 
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


/* Data for the new Graphs and lists */
typedef struct graphdata {

  int ** graph_g;
  int ** graph_h;
  int ** list_G2H; // L1 -  Vertices available in H for each V in G

  int *degrees_g; // Degrees of each vertex in G
  int *degrees_h; // Degrees of each vertex in H

  int numVertG;
  int numVertH;

  int num_E_G; // Number of edges in G
  int num_E_H; // Number of edges in H

  float prob_edgeG;
  float prob_edgeH;

} NEW_GRAPHS;


/* Graph G is random created*/
void construct_G(NEW_GRAPHS *op);

/* Graph H is random created, but with some conditions to contain a rectangle(MinMax)*/
void construct_H(NEW_GRAPHS *op);

/* construct a fixed G & H, for test porpouse */
void contruct_fixedG_H(NEW_GRAPHS *op);

void pairs_rectangles(NEW_GRAPHS *op);
void path_rectangles(NEW_GRAPHS *op);

void save_graphs(NEW_GRAPHS *op);
void save_list(NEW_GRAPHS *op);