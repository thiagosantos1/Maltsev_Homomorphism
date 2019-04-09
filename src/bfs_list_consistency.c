 /* Authors: Arash Rafiey Thiago Santos. --> https://github.com/thiagosantos1/
   Copyright (C)
*/

#include <stdio.h>   
#include <stdlib.h>
#include <string.h>   
#include <unistd.h>
#include <construct_randG_H.h>
#include <bfs_list_consistency.h>

void bfs_list_consis(BFS_DATA *bf, NEW_GRAPHS *op)
{
  bf->front = 0;
  bf->rear  = -1;
  bf->numVert = op->numVertG * op->numVertH;
  bf->numEdges = 0;

  construct_bfs_graph(bf, op);
  //print_graph(bf, op);

}

void construct_bfs_graph(BFS_DATA *bf, NEW_GRAPHS *op)
{
  bf->graph = malloc( bf->numVert * sizeof(int *));
  for(int i=0; i<op->numVertG * op->numVertH; i++){
    bf->graph[i] = malloc(bf->numVert * sizeof(int));
    memset(bf->graph[i],0,bf->numVert * sizeof(int) );
  }

}

int * encode(int x, int y, int a, int b)
{

}

int * decode(int x_, int y_)
{

}

void print_graph(BFS_DATA *bf, NEW_GRAPHS *op)
{
  for(int i=0; i<bf->numVert; i++){
    for(int j=0; j<bf->numVert; j++){
      printf("%d ", bf->graph[i][j]);
    }
    printf("\n");
  }
  printf("N_V_G: %d\tN_V_H: %d\n", op->numVertG, op->numVertH );
  printf("Num Vertice in new Graph: %d\n", bf->numVert);
}

