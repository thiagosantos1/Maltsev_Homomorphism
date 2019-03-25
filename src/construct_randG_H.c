/* Authors: Arash Rafiey Thiago Santos. --> https://github.com/thiagosantos1/
   Copyright (C)
*/

#include <stdio.h>  
#include <stdlib.h>
#include <string.h>  
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <construct_randG_H.h>


void construct_G(NEW_GRAPHS *op)
{

  op->degrees_g = malloc(op->numVertG * sizeof(int));
  memset(op->degrees_g,0,op->numVertG * sizeof(int));

  op->graph_g = malloc(op->numVertG * sizeof(int * ));
  for(int i=0; i<op->numVertG; i++){
    op->graph_g[i] = malloc(op->numVertG * sizeof(int));
    memset(op->graph_g[i],0,op->numVertG * sizeof(int) );
  }
  op->num_E_G = 0;

  // ramdmoly assign edges . No need to go over the whole matrix
  for (int i = 0; i < op->numVertG; i++){
    for (int j = 1+i; j < op->numVertG; j++){
      if( ((float)rand() / (float)RAND_MAX) >= op->prob_edgeG ){
        op->graph_g[i][j] = 1;
        op->graph_g[j][i] = 1;
        op->degrees_g[i]++;
        op->degrees_g[j]++;
        op->num_E_G +=2;
      }
    }
  }
}

void construct_H(NEW_GRAPHS *op)
{

  // calculate size of H
  // For this implementation, we are using the sum of (2 to the degree of each vertex)
  op->num_E_H  = 0;
  op->numVertH = 0;
  int max_degree = 0; // Used to then create the size of matrix H(save space)
  int degree, last_vert = 0, i, x, j;
  int ** G_H; 
  for(i =0; i<op->numVertG; i++){
    degree = (int) pow(2,op->degrees_g[i]);
    max_degree = degree > max_degree ? degree:max_degree;
    op->numVertH += degree;
  }

  // each vertex of G is "assign" in H 2 to the power of degree of that vertice
  G_H = malloc(op->numVertG * sizeof(int * ));
  for(i=0; i<op->numVertG; i++){
    // +1 because one position is to how many vertices for that g
    G_H[i] = malloc( (max_degree +1 ) * sizeof(int) ); 
    G_H[i][0] = (int) pow(2,op->degrees_g[i]);
    for(x=1; x<=G_H[i][0]; x++){
      G_H[i][x] = last_vert;
      last_vert++;
    }
  }

  op->graph_h = malloc(op->numVertH * sizeof(int *));
  for(i=0; i<op->numVertH; i++){
    op->graph_h[i] = malloc(op->numVertH * sizeof(int));
    memset(op->graph_h[i],0,op->numVertH * sizeof(int) );
  }
  op->num_E_H =0;
  op->degrees_h = malloc(op->numVertH * sizeof(int));
  memset(op->degrees_h,0,op->numVertH * sizeof(int));
  // Randomly create H and its edges
  for(i=0; i< op->numVertG - 1; i++){ // for each vertice of G
    for(j=1; j<=G_H[i][0]; j++){ // All possible assigns in H for vertice i in G
      for(x=1; x<=G_H[i+1][0]; x++){ // All possible assign in H for vertice i+1
        if( ((float)rand() / (float)RAND_MAX) >= op->prob_edgeH ){
          op->graph_h[G_H[i][j]][G_H[i+1][x]] = 1;
          op->graph_h[G_H[i+1][x]][G_H[i][j]] = 1;
          op->num_E_H +=2;
          op->degrees_h[G_H[i][j]]++;
          op->degrees_h[G_H[i+1][x]]++;

        }
      }
    }
  }
}

int main(int argc, char const *argv[])
{
  
  NEW_GRAPHS graphs;

  graphs.numVertG = 4;
  graphs.prob_edgeG = 0.5;
  graphs.prob_edgeH = 0.5;

  if(argc ==4){
    graphs.numVertG = atoi(argv[1]);
    graphs.prob_edgeG = atof(argv[2]);
    graphs.prob_edgeH = atof(argv[3]);
  }else if(argc ==3){
    graphs.numVertG = atoi(argv[1]);
    graphs.prob_edgeG = atof(argv[2]);
  }else if(argc ==2){
    graphs.numVertG = atoi(argv[1]);
  }

  time_t t;
  srand((unsigned) time(&t));

  construct_G(&graphs);
  construct_H(&graphs);
  
  return 0;
}






