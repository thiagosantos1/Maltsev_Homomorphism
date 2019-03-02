/* Authors: Arash Rafiey Thiago Santos. --> https://github.com/thiagosantos1/
   Copyright (C)
*/


#include <graphs.h>  
#include <maltsev.h> 
#include <pre_processing.h> 


int run_maltsev(GRAPHS *op)
{
  
  int out=pre_process(op);
  //print_pairs(op);

  return out;

}

int is_rectangle(GRAPHS *op, int x, int a, int b)
{

  int y,c;
  int flag =0; 
  if (a == b) 
    return 0;
  if ( op->list_G2H[x][a] < 0 )
    return 0; 
  if ( op->list_G2H[x][b] < 0 )
    return 0; 
  
  flag =0; 
    for (y=0; y< op->num_vert_G; y++)
      for (c=1; c <= op->list_G2H[y][0]; c++)
        if ( (op->list_G2H[y][c] > -1) && op->pair_list_G2H[x][y][a][ op->list_G2H[y][c] ] && op->pair_list_G2H[x][y][b][ op->list_G2H[y][c] ] && x!= y)
          flag=1; 
  
  return flag; 
}

void print_instance(GRAPHS *op, int x, int a, int current)
{
  int y,b;
  
  printf("new graph for x=%d , a=%d, current =%d \n",x,a,current);
  for (y=0; y< op->num_vert_G; y++)
    if (op->activeG[y])
      printf("%d,",y);

  printf(" \n");
  
  printf(" the lists are \n");
  
  for (y=0; y< op->num_vert_G ; y++){
    if ( op->activeG[y]) {
      printf("L(%d )=",y);
      for (b=1; b<= op->list_G2H[y][0]; b++) 
        if (op->list_G2H[y][b]>-1 )
          printf("%d , ",op->list_G2H[y][b]);

      printf("\n");
    } 
  }

}

int Pruning_the_lists(GRAPHS *op)
{

  int x,a,b;
  int flag=0;
  for ( x=0 ; x < op->num_vert_G; x++){
    if (op->activeG[x]) {
      for (a=op->list_G2H[x][0] ; a >= 1 ; a--){
        if (op->list_G2H[x][a] > -1 ){
          for (b = op->list_G2H[x][0] ; b >= 1; b --){ 
            if ( (op->list_G2H[x][b] > -1 )   && a!=b) {
              op->list_G2H[x][b]=-1;  //it was L1[x][b]=0;
              flag=pre_process(op); 
              if (! flag) 
                return 0; 
            }
          }
        }
      }
                      
    }
  }

  return 1;
}

void print_pairs(GRAPHS *op)
{
  int x,y,a,b;

  for (x=0; x< op->num_vert_G ; x++){ 
    for (y=0; y< op->num_vert_G ; y++){ 
      //printf("L(%d )=",x);
      printf("L(%d,%d)=",x,y);
      for (a=0; a< op->num_vert_H; a++) 
        for (b=0; b< op->num_vert_H; b++) 
          if ( op->pair_list_G2H[x][y][a][b] >0)
            printf("{%d,%d},",a,b);
      printf("\n");
    }
  }  
}

