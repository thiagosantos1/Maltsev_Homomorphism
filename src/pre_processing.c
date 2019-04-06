 /* Authors: Arash Rafiey Thiago Santos. --> https://github.com/thiagosantos1/
   Copyright (C)
*/


#include <graphs.h>  
#include <pre_processing.h> 

int pre_process(GRAPHS *op)
{
  arc_consistency(op);
  print_pairs(op);
  printf("\n\n\n\n");
  if(is_empty_list(op) >0){
    print_pairs(op);
    printf("\nThere are some empty list. No Maltsev\n\n");
    exit(0);
  }
  int out = pair_consistency(op);
  return out;
}

void arc_consistency(GRAPHS *op)
{
  int update=1;
  
  /// arc consistency 
  int x,y,a,b;
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
}

int pair_consistency(GRAPHS *op)
{
  // pair consistency 
  int update=1;
  
  int x,y,z,a,b,c;
  int flag=0,i_a,i_b,i_c; 

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