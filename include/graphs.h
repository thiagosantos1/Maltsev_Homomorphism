/* Authors: Arash Rafiey Thiago Santos. --> https://github.com/thiagosantos1/  */

/***************************************************************************
* This class is designed to have all the information needed for other 
* classes that uses any information about the graphs G & H.
* 
* The User(main) has visibility to only the methods included here.
*
* The Idea of this implementation is to solve linear equations, using 
* Graph and Maltsev theory. After appling the theory, the graph and
* its list left can be used to find a solution for the linear equation
* Using Homomorphism.
* 
* Copyright (C)
*
****************************************************************************/

#include <stdio.h>  
#include <stdlib.h>
#include <string.h>  
#include <unistd.h>
#include <math.h>


typedef 
  signed char
uchar;

typedef struct graphdata {

  uchar ** graph_g;
  uchar ** graph_h;
  uchar ** list_G2H; // Vertices available in H for each V in G
  uchar **** pair_list_G2H; // pair list aboung vertices in G for a mapping in H

  int *degrees_g; // Degrees of each vertex in G
  int *degrees_h; // Degrees of each vertex in H

  int num_vert_G;
  int num_vert_H;

  int num_E_G; // Number of edges in G
  int num_E_H; // Number of edges in H

} GRAPHS;


typedef struct userdata {
  char *graph_g;  // Graph G file name
  char *graph_h;  // Graph H file name
  char *list_homom;     // List os possible assignments from G to H

} USER_PARAMS;

/* Read graphs and the list in the class readgraphs.c, by reading all the files set by the user in user params*/
void read_graphs(GRAPHS *op, USER_PARAMS *ip);

