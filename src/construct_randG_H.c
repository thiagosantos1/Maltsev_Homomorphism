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
  
  // default values
  // can pass values by argument via argv, manually or by using Makefile
  graphs.numVertG = 6;
  graphs.prob_edgeG = 0.3;
  graphs.prob_edgeH = 0.2;
  graphs.max_list_size = 8;

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

  //contruct_fixedG_H(&graphs); // uses graph G & H & list from etc - Good for testing porpouse
  bfs_Gconnected(&graphs);

  pairs_rectangles(&graphs);  
  
  create_pairs_G2H(&graphs);

  path_rectangles(&graphs);

  save_graphs(&graphs);
  save_list(&graphs);
  
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
      if( ((float)rand() / (float)RAND_MAX) <= op->prob_edgeG ){
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

  op->degress_G2H = malloc(op->numVertG * sizeof(int));
  memset(op->degress_G2H,0,op->numVertG * sizeof(int));


  // calculate size of H
  // For this implementation, we are using the sum of (2 to the degree of each vertex)
  op->num_E_H  = 0;
  op->numVertH = 0;
  int max_degree = 0; // Used to then create the size of matrix H(save space)
  int degree, last_vert = 0, i, i_, x, j;
  for(i =0; i<op->numVertG; i++){
    if(op->degrees_g[i] < 12 )
      degree = (int) pow(2,op->degrees_g[i] );
    else
      degree = op->max_list_size;

    degree = degree > op->max_list_size ? op->max_list_size:degree;
    max_degree = degree > max_degree ? degree:max_degree;
    op->numVertH += degree;
    op->degress_G2H[i] = degree;
  }
  

  // each vertex of G is "assign" in H 2 to the power of degree of that vertice
  op->list_G2H = malloc(op->numVertG * sizeof(int * ));
  for(i=0; i<op->numVertG; i++){
    // +1 because one position is to how many vertices for that g
    op->list_G2H[i] = malloc( (max_degree +1 ) * sizeof(int) ); 
    op->list_G2H[i][0] = op->degress_G2H[i];
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

  for(i=0; i< op->numVertG; i++){ // for each vertice of G
    for(i_=0; i_< op->numVertG; i_++){ // for each vertice of G

      if(i != i_){
        for(j=1; j<=op->list_G2H[i][0]; j++){ // All possible assigns in H for vertice i in G
          // make sure j goes to at least of vertice in the list of i_
          int sure_edge = (rand() % op->list_G2H[i_][0]) + 1;
          if(op->graph_h[op->list_G2H[i][j]][op->list_G2H[i_][sure_edge]] != 1){
            op->graph_h[op->list_G2H[i][j]][op->list_G2H[i_][sure_edge]] = 1;
            op->graph_h[op->list_G2H[i_][sure_edge]][op->list_G2H[i][j]] = 1;
            op->num_E_H +=2;
            op->degrees_h[op->list_G2H[i][j]]++;
            op->degrees_h[op->list_G2H[i_][sure_edge]]++;
          }
          for(x=1; x<=op->list_G2H[i_][0]; x++){ // All possible assign in H for vertice i_

            if( (((float)rand() / (float)RAND_MAX) <= op->prob_edgeH) && sure_edge != x){
              if(op->graph_h[op->list_G2H[i][j]][op->list_G2H[i_][x]] != 1){
                op->graph_h[op->list_G2H[i][j]][op->list_G2H[i_][x]] = 1;
                op->graph_h[op->list_G2H[i_][x]][op->list_G2H[i][j]] = 1;
                op->num_E_H +=2;
                op->degrees_h[op->list_G2H[i][j]]++;
                op->degrees_h[op->list_G2H[i_][x]]++;
              }
            }
          }
        }
      }
    }
  }

}

void bfs_Gconnected(NEW_GRAPHS *op)
{
  int queue[op->numVertG*30];   // for bfs exploration
  int visited[op->numVertG*30]; // also used to control de exploration
  int front=0;   // to remove the next one in the front of queue
  int rear=-1;    // to insert at end of queue

  memset(queue,0,op->numVertG * sizeof(int) );
  memset(visited,0,op->numVertG * sizeof(int) );
  bfs(queue, visited, front, rear, op, 0);
  int done = 0;

  while(!done){
    done = 1;
    for(int i=0; i<op->numVertG; i++){
      if(!visited[i]){
        done=0;
        make_rand_connection(op,i);
        front = 0;
        rear = -1;
        memset(queue,0,op->numVertG * sizeof(int) );
        memset(visited,0,op->numVertG * sizeof(int) );
        bfs(queue, visited, front, rear, op, 0);
      }
    }
  }
}

void bfs(int queue[], int visited[], int front, int rear, NEW_GRAPHS *op, int vert)
{
  for(int i=0; i<op->numVertG; i++){
    if(op->graph_g[vert][i] >0 && !visited[i])
      queue[++rear] = i;
  }

  if(front <= rear){
    visited[queue[front]] = 1;
    vert = queue[front];
    front++;
    bfs(queue,visited,front,rear,op,vert);
  }
}

void make_rand_connection(NEW_GRAPHS *op, int x)
{
  int done = 0;
  while(!done){
    done =1;
    int y = (rand() % op->numVertG);
    if(y!= x && op->graph_g[x][y] <1){
      op->graph_g[x][y] = 1;
      op->graph_g[y][x] = 1;
      op->degrees_g[x]++;
      op->degrees_g[y]++;
      op->num_E_G +=2;
      done = 1;
    }
  }
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
                    op->num_E_H +=2;
                  }

                  if(op->graph_h[b][j_] >0 && op->graph_h[a][j_] <1 ){
                    op->graph_h[a][j_] = 1;
                    op->graph_h[j_][a] = 1;
                    op->degrees_h[a]++;
                    op->degrees_h[j_]++;
                    op->num_E_H +=2;
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

  int x,y,a,a_,b,b_,c_,c,d_,d,z,e,e_,rand_f,not_e;

  int updating = 1;

  // by adding and edge, it may create another crossing problem. Make sure to check it
  while( updating >0){
    updating = 0;
    // for every xy in V(G)
    for(x=0; x<op->numVertG; x++){
      for(y=0; y<op->numVertG; y++){

        // for every a,b in L(x)
        for(a_=1; a_<=op->list_G2H[x][0]; a_++){
          a = op->list_G2H[x][a_];
          for(b_=1; b_<=op->list_G2H[x][0]; b_++){
            b = op->list_G2H[x][b_];
            
            // for every c,d in L(y)
            for(c_=1; c_<=op->list_G2H[y][0]; c_++){
              c = op->list_G2H[y][c_];
              for(d_=1; d_<=op->list_G2H[y][0]; d_++){
                d = op->list_G2H[y][d_];
                
                // if (a,c), (a,d), (b,c) in L(x,y)  
                if( (op->pair_list_G2H[x][y][a][c] >0) & (op->pair_list_G2H[x][y][a][d] >0) & (op->pair_list_G2H[x][y][b][c] >0) ){
                  // if (b,d) not in L(x,y)
                  if(op->pair_list_G2H[x][y][b][d] <1){

                    // for every yz in E(G)
                    for(z=0; z<op->numVertG; z++){
                      if( (z!= y) & (op->graph_h[y][z] >0) ){
                        not_e = 1;
                        for(e_=1; e_<=op->list_G2H[z][0]; e_++){
                          e = op->list_G2H[z][e_];
                          if( (op->pair_list_G2H[x][z][b][e] >0) & (op->graph_h[e][d] > 0) ){
                            not_e = 0; // so there's an e. Then
                            break;
                          }
                        }
                        if(not_e >0){ //
                          rand_f = op->list_G2H[z][(rand() % op->list_G2H[z][0]) + 1];
                          // make 100% sure {rand_f, d} is not an edge
                          if(op->graph_h[rand_f][d] <1){
                            op->pair_list_G2H[x][z][b][rand_f] = 1;
                            op->graph_h[rand_f][d] = 1;
                            op->graph_h[d][rand_f] = 1;
                            op->num_E_H +=2;
                            op->degrees_h[rand_f]++;
                            op->degrees_h[d]++;
                            updating = 1;
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
      }
    }
  }

}

void save_graphs(NEW_GRAPHS *op)
{
  FILE * fp;
  
  fp=fopen("../etc/graph_G_out.txt","w"); 

  if (fp ==NULL ) {
    printf("Error openning file\n");
    return;
  } 
  
  fprintf (fp,"%d ", op->numVertG);
  fprintf (fp,"%d", op->num_E_G);

  for(int i =0; i<op->numVertG; i++){
    for(int j =1+i; j<op->numVertG; j++){
      if(op->graph_g[i][j] >0){
        fprintf(fp, "\n%d %d\n%d %d", i,j,j,i);
      }
    }
  }

  fp=fopen("../etc/graph_H_out.txt","w"); 

  if (fp ==NULL ) {
    printf("Error openning file\n");
    return;
  } 
  
  fprintf (fp,"%d ", op->numVertH);
  fprintf (fp,"%d", op->num_E_H);

  for(int i =0; i<op->numVertH; i++){
    for(int j =1+i; j<op->numVertH; j++){
      if(op->graph_h[i][j] >0){
        fprintf(fp, "\n%d %d\n%d %d", i,j,j,i);
      }
    }
  }

  fclose(fp);

}
void save_list(NEW_GRAPHS *op)
{
  FILE * fp;
  
  fp=fopen("../etc/list_file_out.txt","w"); 

  if (fp ==NULL ) {
    printf("Error openning file\n");
    return;
  } 

  for(int i=0; i<op->numVertG; i++){
    // +1 because one position is to how many vertices for that g
    fprintf (fp,"%d %d", i, op->list_G2H[i][0]);
    for(int x=1; x<=op->list_G2H[i][0]; x++){
      fprintf (fp," %d", op->list_G2H[i][x]);
    }
    if(i<op->numVertG-1)
      fprintf(fp,"\n");
  }

}

void print_degrees(NEW_GRAPHS *op, int graph)
{
  if(graph ==1){
    printf("\n");
    for(int i =0; i<op->numVertG; i++)
      printf("%d ",  op->degrees_g[i]);
    printf("\n");
  }else{
    printf("\n");
    for(int i =0; i<op->numVertH; i++)
      printf("%d ",  op->degrees_h[i]);
    printf("\n");
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

}

void create_pairs_G2H(NEW_GRAPHS *op)
{
  int x,y,a,b;
  int temp_a,temp_b;

  // allocate memory for pair lists 
  op->pair_list_G2H = (int ****) malloc(op->numVertG * sizeof(int *)); 
  for (x=0; x< op->numVertG; x++)
    op->pair_list_G2H[x]= (int ***) malloc(op->numVertG * sizeof(int *)); 
  
  for (x=0; x< op->numVertG; x++)
    for (y=0; y<op->numVertG; y++)
      op->pair_list_G2H[x][y]=(int **) malloc(op->numVertH * sizeof(int *)); 
  
  for (x=0; x< op->numVertG; x++)
    for (y=0; y<op->numVertG; y++)
      for (a=0; a < op->numVertH; a++)
        op->pair_list_G2H[x][y][a]=(int *) malloc(op->numVertH * sizeof(int *));

  // initially everything is zero 
  for (x=0;x < op->numVertG; x++) 
    for (y=0; y < op->numVertG; y++)
      for (a=0; a< op->numVertH; a++) 
        for (b=0; b < op->numVertH; b++) 
          op->pair_list_G2H[x][y][a][b]=0;


  for (x=0;x < op->numVertG; x++) {
    for (y=x+1; y < op->numVertG; y++){
      for (a=1; a<= op->list_G2H[x][0]; a++){
        for (b=1; b <= op->list_G2H[y][0]; b++){
          if ( (op->list_G2H[x][a]>-1) && (op->list_G2H[y][b] > -1) ){
            temp_a = op->list_G2H[x][a];
            temp_b = op->list_G2H[y][b];
            op->pair_list_G2H[x][y][temp_a][temp_b] = 1;
            op->pair_list_G2H[y][x][temp_b][temp_a] = 1; 
          }
        }
      }
    } 
  }

  for (x=0;x < op->numVertG; x++) 
    for (a=1; a<= op->list_G2H[x][0]; a++) 
      if (op->list_G2H[x][a] > -1)
        op->pair_list_G2H[x][x][op->list_G2H[x][a]][op->list_G2H[x][a]]=1; 

  for (x=0;x < op->numVertG; x++){
    for (y=0; y < op->numVertG; y++){

      if (op->graph_g[x][y]==1){
        for (a=1; a<= op->list_G2H[x][0]; a++){
          if (op->list_G2H[x][a] > -1 ){
            for (b=1; b <= op->list_G2H[y][0]; b++)  { 
              temp_a=op->list_G2H[x][a];
              temp_b=op->list_G2H[y][b];
              if ( (op->list_G2H[y][b] >-1) && op->graph_h[temp_a][temp_b]==1){  
                op->pair_list_G2H[x][y][temp_a][temp_b] = 1;
                op->pair_list_G2H[y][x][temp_b][temp_a] = 1;
              }
              else {
                op->pair_list_G2H[x][y][temp_a][temp_b] = 0; 
                op->pair_list_G2H[y][x][temp_b][temp_a] = 0;
              }
            }
          }
        }
      }

    }
  }
}