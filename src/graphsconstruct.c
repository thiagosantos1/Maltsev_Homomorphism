/* Authors: Arash Rafiey Thiago Santos. --> https://github.com/thiagosantos1/
   Copyright (C)
*/

#include <graphs.h> 
#include <readgraphs.h> 
#include <graphsconstruct.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 

int odd_Even_bits ( int x)
{
  int count=0;
  
  while ( x> 0){
    if (x %2 )
      count++;
    x=x/2;     
  } 

  if(count %2) 
    return 0;

  return 1;
}

int which_neighbor(GRAPHS *op, int x, int y)
{
  int a,i;
  
  i=0; 
  
  for (a=0; a< op->num_vert_G; a++) {
    if (op->graph_g[x][a])
      i++;

    if( a== y)
      return i-1; 
  }
    
  printf("error in Which neighbor\n");
  return 0; 
}

int which_bit(int l, int i )
{
  int t=0;
  
  while( t < i){
    l=l/2;
    t++;
  }

  if(l % 2 ) 
    return 1;
  
  return 0;
}

void create_list_G2H(GRAPHS *op, USER_PARAMS * ip)
{
  // initialize graph G
  init_graph_g(op,ip->graph_g);

  int i,x,y,a;
  int counter=0; 

  int range[op->num_vert_G];

  op->num_vert_H = 2; 

  for (x=1; x< op->num_vert_G; x++){
    range[x]=op->num_vert_H ;
    op->num_vert_H += (int) pow(2,op->degrees_g[x]-1);
  }  


  op->list_G2H = malloc(op->num_vert_G * sizeof(int *));
  for(i=0; i<op->num_vert_G; i++){
    op->list_G2H[i] = malloc(op->num_vert_H * sizeof(int));
    memset(op->list_G2H[i],0,op->num_vert_H * sizeof(int) );
  }


  op->list_G2H[0][0]=1;
  op->list_G2H[0][1]=1;
  
  counter=2;
  
  for(x=1; x< op->num_vert_G; x++){
    y= (int) pow(2,op->degrees_g[x]-1);
    for (a=0; a< y; a++)
      op->list_G2H[x][counter+a]=1;
    counter+=y;  
  }

  int j,l,k,max_range_x,max_range_y;

  int H[2][op->num_vert_H*op->num_vert_H];
  int edge_counter=0; 


  // adding edges between L[0] && L[1]
  max_range_y=(int) pow(2,op->degrees_g[1]);
 
  for ( k=0; k< max_range_y; k++){ // for 00 in L1[0]
    if (odd_Even_bits(k) )    {
      if ( k % 2 ==0  ) {   
        H[0][edge_counter]=0;
        H[1][edge_counter]=range[1]+k/2;

        H[0][edge_counter+1]=range[1]+k/2; 
        H[1][edge_counter+1]=0;
        edge_counter+=2;
      }             
      else {
        H[0][edge_counter]=1;
        H[1][edge_counter]=range[1]+k/2;

        H[0][edge_counter+1]=range[1]+k/2; 
        H[1][edge_counter+1]=1;
        edge_counter+=2;
      }

    }
  }

  for ( x=1; x < op->num_vert_G; x++){
    for (y=x+1; y < op->num_vert_G; y++){
      if (op->graph_g[x][y]){
        // x is the i-th neighbor of y
        i= which_neighbor(op,x,y);
        j= which_neighbor(op,y,x);
        max_range_x = (int) pow(2,op->degrees_g[x]);
        max_range_y= (int) pow(2,op->degrees_g[y]);
        for (l=0; l< max_range_x; l++){
          if ( odd_Even_bits(l)){
            for ( k=0; k< max_range_y; k++){
              if ( odd_Even_bits(k)) {
              
                if ( which_bit(l,i) == which_bit(k,j) ) {   
                  H[0][edge_counter]=range[x]+l/2;
                  H[1][edge_counter]=range[y]+k/2;
           
                  H[0][edge_counter+1]=range[y]+k/2; 
                  H[1][edge_counter+1]=range[x]+l/2;
                  edge_counter+=2;
                }  
              } 
            } 
          } 
        }       
      }
    }
  }

  save_list_G2H(op,ip);

  // save graph H

  FILE * fp;
    
  fp=fopen(ip->graph_h,"w"); 

  if (fp ==NULL ) {
    printf("no list file \n");
    return;
  } 
  
  fprintf (fp,"%d ", op->num_vert_H);
  fprintf (fp,"%d", edge_counter);
  fprintf (fp,"\n");
  
  for (i=0; i < edge_counter; i++)
  {
    fprintf(fp, "%d %d",H[0][i],H[1][i]);
    fprintf(fp,"\n");
  }
  
  fclose(fp);

}


void save_list_G2H(GRAPHS *op, USER_PARAMS * ip)
{
  FILE * fp_list;
  int x,y,a;
   
  fp_list=fopen(ip->list_homom,"w"); 

  if (fp_list ==NULL ) {
    printf("no list file \n");
    return;
  } 

  fprintf(fp_list,"%d ",0);
  fprintf(fp_list,"%d ",2);
  fprintf(fp_list,"%d ",0);
  fprintf(fp_list,"%d\n",1);

  for ( x=1; x< op->num_vert_G; x++){
    fprintf(fp_list,"%d ",x);
    y=(int) pow (2,op->degrees_g[x]-1);
    fprintf(fp_list,"%d ",y);
    
    for (a=0; a < op->num_vert_H; a++){
      if (op->list_G2H[x][a])
       fprintf(fp_list,"%d ",a);
    }
    
    fprintf(fp_list,"\n");
  } 
  fclose (fp_list); 

}


/*

list of X has 2 to the degre of x 

The edges in H starts randmoly




*/








