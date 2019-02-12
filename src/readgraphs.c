/* Authors: Arash Rafiey Thiago Santos. --> https://github.com/thiagosantos1/
   Copyright (C)
*/

#include <graphs.h>  
#include <readgraphs.c.h>


void makegraph(GRAPHS *op, USER_PARAMS * ip)
{

  init_graph_g(op,ip->graph_g);
  init_graph_h(op,ip->graph_h);
  init_list(op,ip->list_homom); 

}

void init_graph_g(GRAPH *op, char * file_name)
{
  FILE * fp = fopen(file_name, "r");
  if(fp == NULL){
    fprintf(stderr, "\nError opening file %s\n",file_name );
    exit(0);
  }
  int i,j;
  char str1[10000],str2[10000];
  fscanf(fp, "%s %s", str1,str2);
  op->num_vert_G = atoi(str1);
  op->num_E_G = atoi(str2);

  op->graph_g = malloc(op->num_vert_G * sizeof(uchar *));
  for(i=0; i<op->num_vert_G; i++){
    op->graph_g[i] = malloc(op->num_vert_G * sizeof(uchar));
    memset(op->graph_g[i],0,op->num_vert_G);
  }

  // initialize degrees to zero
  op->degrees_g = (int *)calloc(op->num_vert_G, sizeof(int)); 

  while( (fscanf(fp, "%s %s", str1,str2)) ==2){
    i = atoi(str1);
    j = atoi(str2);
    op->graph_g[i][j] = 1;
    op->degrees_g[i]++;
  }

}

void init_graph_h(GRAPH *op, char * file_name)
{
  FILE * fp = fopen(file_name, "r");
  if(fp == NULL){
    fprintf(stderr, "\nError opening file %s\n",file_name );
    exit(0);
  }
  int i,j;
  char str1[10000],str2[10000];
  fscanf(fp, "%s %s", str1,str2);
  op->num_vert_H = atoi(str1);
  op->num_E_H = atoi(str2);

  op->graph_h = malloc(op->num_vert_h * sizeof(uchar *));
  for(i=0; i<op->num_vert_h; i++){
    op->graph_h[i] = malloc(op->num_vert_h * sizeof(uchar));
    memset(op->graph_h[i],0,op->num_vert_h);
  }

  // initialize degrees to zero
  op->degrees_h = (int*) calloc(op->num_vert_H, sizeof(int));

  while( (fscanf(fp, "%s %s", str1,str2)) ==2){
    i = atoi(str1);
    j = atoi(str2);
    op->graph_h[i][j] = 1;
    op->degrees_h[i]++;
  }

}

void init_list(GRAPH *op, char * file_name)
{
  op->list_G2H = malloc(op->num_vert_g * sizeof(uchar *));
  for(i=0; i<op->num_vert_g; i++){
    op->list_G2H[i] = malloc(op->num_vert_h * sizeof(uchar));
    memset(op->list_G2H[i],0,op->num_vert_h);
  }


  int y,counter;
  for (int x=0; x < op->num_vert_g; x++) // assume everyone has a none-empty list 
  {
    
    fscanf(fp,"%d",&y); // the first one from G; 
    fscanf (fp,"%d",&counter); // how many in the list of y 
    op->list_G2H[y][0]=counter; 
    for (i=0; i < counter; i++)
    {   
      fscanf (fp,"%d",&b);   // b is the element to be in L1(y) 
      L1[y][i+1]=b;
    }  
  }

} 