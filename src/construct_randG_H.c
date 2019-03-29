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

  //construct_G(&graphs);
  //construct_H(&graphs);
  contruct_fixedG_H(&graphs);

  // complete H to mantain the rectangle property 
  printf("\nBefore\n");
  for(int i =0; i<graphs.numVertH; i++){
    for(int j =0; j<graphs.numVertH; j++){
      printf("%d ",graphs.graph_h[i][j] );
    }
    printf("\n");
  }
  pairs_rectangles(&graphs);
  path_rectangles(&graphs);
  printf("\nAfter\n");
  for(int i =0; i<graphs.numVertH; i++){
    for(int j =0; j<graphs.numVertH; j++){
      printf("%d ",graphs.graph_h[i][j] );
    }
    printf("\n");
  }
  
  return 0;
}


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
  for(i =0; i<op->numVertG; i++){
    degree = (int) pow(2,op->degrees_g[i] *2);
    max_degree = degree > max_degree ? degree:max_degree;
    op->numVertH += degree;
  }

  // each vertex of G is "assign" in H 2 to the power of degree of that vertice
  op->list_G2H = malloc(op->numVertG * sizeof(int * ));
  for(i=0; i<op->numVertG; i++){
    // +1 because one position is to how many vertices for that g
    op->list_G2H[i] = malloc( (max_degree +1 ) * sizeof(int) ); 
    op->list_G2H[i][0] = (int) pow(2,op->degrees_g[i] *2);
    for(x=1; x<=op->list_G2H[i][0]; x++){
      op->list_G2H[i][x] = last_vert;
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
    for(j=1; j<=op->list_G2H[i][0]; j++){ // All possible assigns in H for vertice i in G
      for(x=1; x<=op->list_G2H[i+1][0]; x++){ // All possible assign in H for vertice i+1
        if( ((float)rand() / (float)RAND_MAX) >= op->prob_edgeH ){
          op->graph_h[op->list_G2H[i][j]][op->list_G2H[i+1][x]] = 1;
          op->graph_h[op->list_G2H[i+1][x]][op->list_G2H[i][j]] = 1;
          op->num_E_H +=2;
          op->degrees_h[op->list_G2H[i][j]]++;
          op->degrees_h[op->list_G2H[i+1][x]]++;

        }
      }
    }
  }
  
}

void contruct_fixedG_H(NEW_GRAPHS *op)
{
  FILE * fp = fopen("../etc/graph_G.txt", "r");
  if(fp == NULL){
    fprintf(stderr, "\nError opening file %s\n","graph_G" );
    exit(0);
  }
  int i,j,x,y,b,counter;
  char str1[10000],str2[10000];
  fscanf(fp, "%s %s", str1,str2);
  op->numVertG = atoi(str1);
  op->num_E_G = atoi(str2);

  op->graph_g = malloc(op->numVertG * sizeof(int *));
  for(i=0; i<op->numVertG; i++){
    op->graph_g[i] = malloc(op->numVertG * sizeof(int));
    memset(op->graph_g[i],0,op->numVertG * sizeof(int) );
  }
  // initialize degrees to zero
  op->degrees_g = (int *)calloc(op->numVertG, sizeof(int)); 

  for (int x=0; x< op->num_E_G; x++) {
    fscanf(fp, "%s %s", str1,str2);
    i = atoi(str1);
    j = atoi(str2);
    op->graph_g[i][j] = 1;
    op->degrees_g[i]++;
  }

  //graph h
  fp = fopen("../etc/graph_H.txt", "r");
  if(fp == NULL){
    fprintf(stderr, "\nError opening file %s\n","graph_H" );
    exit(0);
  }
  fscanf(fp, "%s %s", str1,str2);
  op->numVertH = atoi(str1);
  op->num_E_H = atoi(str2);

  op->graph_h = malloc(op->numVertH * sizeof(int *));
  for(i=0; i<op->numVertH; i++){
    op->graph_h[i] = malloc(op->numVertH * sizeof(int));
    memset(op->graph_h[i],0,op->numVertH * sizeof(int) );
  }

  // initialize degrees to zero
  op->degrees_h = (int*) calloc(op->numVertH, sizeof(int));

  for (x=0; x< op->num_E_H; x++) {
    fscanf(fp, "%s %s", str1,str2);
    i = atoi(str1);
    j = atoi(str2);
    op->graph_h[i][j] = 1;
    op->degrees_h[i]++;
  }

  // list
  fp = fopen("../etc/list_file.txt", "r");
  if(fp == NULL){
    fprintf(stderr, "\nError opening file %s\n","list_file" );
    exit(0);
  }

  op->list_G2H = malloc(op->numVertG * sizeof(int *));
  for(i=0; i<op->numVertG; i++){
    op->list_G2H[i] = malloc(op->numVertH * sizeof(int));
    memset(op->list_G2H[i],0,op->numVertH * sizeof(int) );
  }

  for (x=0; x < op->numVertG; x++){ // assume everyone has a none-empty list 
    
    fscanf(fp,"%d",&y); // the first one from G; 
    fscanf (fp,"%d",&counter); // how many in the list of y 
    op->list_G2H[y][0]=counter; 
    for (i=0; i < counter; i++){   
      fscanf (fp,"%d",&b);   // b is the element to be in L1(y) 
      op->list_G2H[y][i+1]=b;
    }  
  }

  // printf("G:%d\n", op->numVertG );
  // for(int i=0; i<op->numVertG; i++){
  //   for(int j=0; j<op->numVertG; j++){
  //     printf("%d ", op->graph_g[i][j]);
  //   }
  //   printf("\n");
  // }

  // printf("\nH:%d\n", op->numVertH);
  // for(int i=0; i<op->numVertH; i++){
  //   for(int j=0; j<op->numVertH; j++){
  //     printf("%d ", op->graph_h[i][j]);
  //   }
  //   printf("\n");
  // }

}

void pairs_rectangles(NEW_GRAPHS *op)
{
  int x,y,i,j,a,b, i_, j_, ab_;
  // for every xy in G
  for(x=0; x<op->numVertG; x++){
    for(y=1+x; y<op->numVertG; y++ ){
      if(op->graph_g[x][y] >0){

        // all pairs in L(x)
        for(i=1; i<=op->list_G2H[x][0] -1; i++){
          for(j=i+1; j<=op->list_G2H[x][0]; j++){
            a = op->list_G2H[x][i];
            b = op->list_G2H[x][j];

            // all in L(y)
            for(i_=1; i_<=op->list_G2H[y][0]; i_++){
              ab_ = op->list_G2H[y][i_];
              // check for crossing/intersection
              if(op->graph_h[ab_][a] &&  op->graph_h[ab_][b]){

                // then, all nbrs of a must be nbr of b and vice-versa
                for(j_=0; j_<op->numVertH; j_++){

                  if(op->graph_h[a][j_] >0 && op->graph_h[b][j_] <1 ){
                    op->graph_h[b][j_] = 1;
                    op->graph_h[j_][b] = 1;
                    op->degrees_h[b]++;
                    op->degrees_h[j_]++;
                    op->num_E_G +=2;
                  }

                  if(op->graph_h[b][j_] >0 && op->graph_h[a][j_] <1 ){
                    op->graph_h[a][j_] = 1;
                    op->graph_h[j_][a] = 1;
                    op->degrees_h[a]++;
                    op->degrees_h[j_]++;
                    op->num_E_G +=2;
                  }
                }
              }

            }

          }
        }
      }

    }

  }
  
}
void path_rectangles(NEW_GRAPHS *op)
{

}





