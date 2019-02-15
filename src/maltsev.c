/* Authors: Arash Rafiey Thiago Santos. --> https://github.com/thiagosantos1/
   Copyright (C)
*/


#include <graphs.h>  
#include <maltsev.h>

int run_maltsev(GRAPHS *op)
{
  
  int out=pre_processing(op);
  print_pairs(op);

  return out;

}

int pre_processing(GRAPHS *op)
{
  int update=1;
  
  /// arc consistency 
  int x,y,z,a,b,c;
  int flag=0,i_a,i_b; 

  while (update) {
    
    update=0; 
  
    for (x=0;x<op->num_vert_G;x++){
      for (a=1; a<= op->list_G2H[x][0]; a++){
        if( op->list_G2H[x][a] > -1 ){ 
          i_a=op->list_G2H[x][a];
          for( y=0; y < op->num_vert_G ; y++){
            if  ( op->graph_g[x][y] && op->activeG[x] && op->activeG[y]) {
              flag=0;
              for (b=1; b<= op->list_G2H[y][0] && !flag ; b++){
                if ( (op->list_G2H[y][b]> -1)  && op->graph_h[ i_a ][ op->list_G2H[y][b] ] )  
                  flag=1;
              }
              if (! flag) {
                update=1;
                op->list_G2H[x][a]=-1; 
              }    
            } 
          }   
       
        }
      }
    }
         
    for (y=0; y<op->num_vert_G; y++){
      for (b=1; b<= op->list_G2H[y][0] ; b++){
        if( op->list_G2H[y][b]> -1 ){
          i_b=op->list_G2H[y][b];
          for( x=0; x < op->num_vert_G ; x++){
            if ( op->graph_g[x][y] && op->activeG[x] && op->activeG[y] ){
              flag=0;
              for (a=1; a <=op->list_G2H[x][0] && !flag ; a++){
                if ( (op->list_G2H[x][a]> -1) && op->graph_h[ op->list_G2H[x][a] ][ i_b ] )   
                  flag=1;
              }

              if (! flag) {
                update=1;
                op->list_G2H[y][b]=-1; 
              }   
            } 
          }   
        }
      }
    }    
  } 

  // pair consistency 
  update=1;
  int  i_c;
  while (update)
  {
    update=0;
    for(x=0; x < op->num_vert_G; x++){
      for(y=0; y < op->num_vert_G; y++){
        for (a=1; a<= op->list_G2H[x][0]; a++){ 
          for (b=1; b<= op->list_G2H[y][0] ; b++){ 
            if ( (op->list_G2H[x][a] >-1 ) && (op->list_G2H[y][b] > -1 ) ){
              i_a=op->list_G2H[x][a]; i_b=op->list_G2H[y][b];
              if ( op->pair_list_G2H[x][y][i_a][i_b] && op->activeG[x] && op->activeG[y] ){
                for (z=0; z<op->num_vert_G; z++){ 
                  if (op->activeG[z]){
                    flag=0;
                      
                    for (c=1; c<=op->list_G2H[z][0] ; c++)
                    {
                      i_c=op->list_G2H[z][c];
                     if( ( i_c >-1)  && op->pair_list_G2H[x][z][i_a][i_c] && op->pair_list_G2H[y][z][i_b][i_c] ) 
                       flag=1;
                      }
                    
                    if (!flag) {
                  
                    op->pair_list_G2H[x][y][i_a][i_b]=0;
                    op->pair_list_G2H[y][x][i_b][i_a]=0;
                  
                    update=1; 
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

  for (x=0; x < op->num_vert_G; x++){ 
    if (op->activeG[x]){
      for (a=1; a<= op->list_G2H[x][0]; a++){ 
        if ( op->list_G2H[x][a] > -1 ){   
          i_a=op->list_G2H[x][a]; 
          for ( y=0; y < op->num_vert_G ; y++){
            if (op->activeG[y]){  
              flag=0; 

              for (b=1; b<= op->list_G2H[y][0] && !flag ; b++){
                i_b=op->list_G2H[y][b]; 
                if ( (i_b >-1 ) && op->pair_list_G2H[x][y][i_a][i_b] )
                  flag=1;
              }  
              if(!flag )
                op->list_G2H[x][a]=-1; 
            }

          }
        } 
      }
    }
  }


  for( x=0; x< op->num_vert_G; x++){
    for (y=0; y< op->num_vert_G; y++){
      if (op->activeG[x] && op->activeG[y]){
        flag=0;
        for (a=1; a<= op->list_G2H[x][0]; a++){  
          for (b=1; b<= op->list_G2H[y][0]; b++)  
            if ( (op->list_G2H[x][a]> -1) && (op->list_G2H[y][b]> -1) && op->pair_list_G2H[x][y][ op->list_G2H[x][a] ][ op->list_G2H[y][b] ])
              flag=1;
        }
        if (! flag) 
          return 0;
      }
    }
  }
             
  return 1; 

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
              flag=pre_processing(op); 
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

