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
  construct_bfs_graph(bf, op);

}

void construct_bfs_graph(BFS_DATA *bf, NEW_GRAPHS *op)
{
  printf("%d %d\n", op->numVertG, op->numVertH);
  printf("%d %d\n", bf->front, bf->rear);
}