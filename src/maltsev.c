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

  init_distinguisher(op);
  //print_distinguisher(op);
  run_distinguisher(op);
  make_choice_distinguisher(op);

  out = check_maltsev(op);

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


void init_distinguisher(GRAPHS *op)
{
  int x,a,b,c,d;

  op->distinguisher = (int *****) malloc( op->num_vert_G * sizeof(int *)); 

  for (x=0; x< op->num_vert_G; x++){
    op->distinguisher[x]= (int ****) malloc( op->list_G2H[x][0] * sizeof(int *)); 

    for (a=0; a< op->list_G2H[x][0]; a++){   
      op->distinguisher[x][a]= (int ***) malloc( op->list_G2H[x][0] * sizeof(int *)); 

      for (b=0; b< op->list_G2H[x][0]; b++){  
        op->distinguisher[x][a][b]= (int **) malloc( op->list_G2H[x][0] * sizeof(int *)); 

        for (c=0; c< op->list_G2H[x][0]; c++){
          op->distinguisher[x][a][b][c]= (int *) malloc( (op->num_vert_H + 1) * sizeof(int *)); 
          memset(op->distinguisher[x][a][b][c],0,op->num_vert_H);
        }
      }
    }
  }

  int temp, counter;
  for (x=0; x < op->num_vert_G; x++){
    for (a=0; a< op->list_G2H[x][0]; a++){
      for (b=0; b< op->list_G2H[x][0]; b++){
        for( c=0; c< op->list_G2H[x][0]; c++ ){
          counter=0;
          for( d=1 ; d<= op->list_G2H[x][0]; d++ ){
            temp=op->list_G2H[x][d];  
            if ( temp > -1){
              counter++;
              op->distinguisher[x][a][b][c][counter] = temp;    
            }
          
          }
          op->distinguisher[x][a][b][c][0] = counter;   
        }
      }
    }
  }
   
}

void print_distinguisher(GRAPHS *op)
{
  int x,a,b,c,d;
  for (x=0; x < op->num_vert_G; x++){
    for (a=0; a< op->list_G2H[x][0]; a++){
      for (b=0; b< op->list_G2H[x][0]; b++){
        for( c=0; c< op->list_G2H[x][0]; c++ ){
          if ( op->list_G2H[x][a+1] > -1 && op->list_G2H[x][b+1] > -1 && op->list_G2H[x][c+1] > -1 ){
            printf("Dis[%d][%d][%d][%d] : ",x,op->list_G2H[x][a+1],op->list_G2H[x][b+1],op->list_G2H[x][c+1]);
            for (d=1; d<= op->distinguisher[x][a][b][c][0] ; d++){ 
              if ( op->distinguisher[x][a][b][c][d] > -1 )
                printf("%d ",op->distinguisher[x][a][b][c][d]);
            }
            printf("\n");
          }
        }
      }
    }
  }  
}


void run_distinguisher(GRAPHS *op)
{
  int x,y,a,b,c,d,e,j;
  int i_a,i_b,i_c,i_d,i_e, i_j;

  // Explain more
  for (x=0; x< op->num_vert_G; x++){
    for (a=0; a< op->list_G2H[x][0]; a++){
      i_a=op->list_G2H[x][a+1];
      if (i_a > -1){ 
        op->distinguisher[x][a][a][a][0]=1;
        op->distinguisher[x][a][a][a][1]=i_a;
      }
    }
  }

  for (x=0; x< op->num_vert_G ; x++){
    for (a=0; a< op->list_G2H[x][0]; a++){
      for (b=0; b< op->list_G2H[x][0]; b++){
        i_a=op->list_G2H[x][a+1];
        i_b=op->list_G2H[x][b+1]; 
        if (i_a > -1 && i_b > -1) {  
          op->distinguisher[x][a][a][b][0]=1; 
          op->distinguisher[x][b][a][a][0]=1;  
          op->distinguisher[x][b][a][a][1]=i_b;
          op->distinguisher[x][a][a][b][1]=i_b;
        }
      }
    }
  }

  // first round of elimination
  for (x=0; x < op->num_vert_G; x++){
    for (a=0; a < op->list_G2H[x][0]; a++){
      for (b=0; b < op->list_G2H[x][0]; b++){
        for (c=0; c < op->list_G2H[x][0]; c++){
          if (op->list_G2H[x][a+1] > -1 && op->list_G2H[x][b+1] > -1 && op->list_G2H[x][c+1] > -1 ){
            for (y=0; y < op->num_vert_G; y++){  
              for (e =0; e < op->list_G2H[y][0]; e++){
                if (op->list_G2H[y][e+1] > -1){
                  i_a=op->list_G2H[x][a+1]; i_b=op->list_G2H[x][b+1]; i_c=op->list_G2H[x][c+1];
                  i_e=op->list_G2H[y][e+1];
                  for ( d=1; d<=op->distinguisher[x][a][b][c][0]; d++){
                    i_d=op->distinguisher[x][a][b][c][d]; 
                    if (i_d > -1 ){ 
                      if (op->pair_list_G2H[x][y][i_a][i_e] && op->pair_list_G2H[x][y][i_b][i_e] && op->pair_list_G2H[x][y][i_c][i_e] && !op->pair_list_G2H[x][y][i_d][i_e])
                        op->distinguisher[x][a][b][c][d]=-1;
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

  // second round of elimination 
  for (x=0; x < op->num_vert_G; x++){
    for (a=0; a < op->list_G2H[x][0]; a++){
      for (b=0; b < op->list_G2H[x][0]; b++){
        for (c=0; c < op->list_G2H[x][0]; c++){
          if (op->list_G2H[x][a+1] > -1 && op->list_G2H[x][b+1] > -1 && op->list_G2H[x][c+1] > -1 ){
            for (y=0; y < op->num_vert_G; y++){  
              for (d =0; d < op->list_G2H[y][0]; d++){
                for (e =0; e < op->list_G2H[y][0]; e++){
                  if (op->list_G2H[y][e+1] > -1 && op->list_G2H[y][d+1] > -1){
                    i_a=op->list_G2H[x][a+1]; i_b=op->list_G2H[x][b+1]; i_c=op->list_G2H[x][c+1];
                    i_d=op->list_G2H[y][d+1]; i_e=op->list_G2H[y][e+1];
                    for ( j=1; j <= op->distinguisher[x][a][b][c][0]; j++){           
                      i_j=op->distinguisher[x][a][b][c][j];
                      if (i_j > -1){  
                        if (op->pair_list_G2H[x][y][i_a][i_d] && op->pair_list_G2H[x][y][i_b][i_d] && op->pair_list_G2H[x][y][i_c][i_e] && !op->pair_list_G2H[x][y][i_j][i_e])
                          op->distinguisher[x][a][b][c][j]=-1;
                         
                        if (op->pair_list_G2H[x][y][i_a][i_d] && op->pair_list_G2H[x][y][i_b][i_e] && op->pair_list_G2H[x][y][i_c][i_e] && !op->pair_list_G2H[x][y][i_j][i_d])
                          op->distinguisher[x][a][b][c][j]=-1;
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
  

  // Third rounds of eliminations (Arc consistency between distinguisher of (x,a,b,c) and (y,a',b',c') xy is not necessarily an edge)    
  
  int ind,f,g,i_f,i_g,l,i_l,flag1; 
  for (x=0; x < op->num_vert_G; x++){
    for (a=0; a < op->list_G2H[x][0]; a++){
      for (b=0; b < op->list_G2H[x][0]; b++){
        for (c=0; c < op->list_G2H[x][0]; c++){
          i_a=op->list_G2H[x][a+1]; i_b=op->list_G2H[x][b+1]; i_c=op->list_G2H[x][c+1]; 
          if (op->list_G2H[x][a+1] > -1 && op->list_G2H[x][b+1] > -1 && op->list_G2H[x][c+1] > -1 ){
            for (d=1; d <=op->distinguisher[x][a][b][c][0]; d++ ){
              ind=  op->distinguisher[x][a][b][c][d];
              if ( ind > -1 ){      
                for (y=0; y < op->num_vert_G; y++){  
                  if ( y != x){
                    for (e =0; e < op->list_G2H[y][0]; e++){
                      for (f =0; f < op->list_G2H[y][0]; f++){
                        for (g =0; g < op->list_G2H[y][0]; g++){  
                          i_e=op->list_G2H[y][e+1]; i_f=op->list_G2H[y][f+1]; i_g=op->list_G2H[y][g+1];
                          if (op->list_G2H[y][e+1] > -1 && op->list_G2H[y][f+1] > -1 && op->list_G2H[y][g+1] > -1 ){
                            if (op->pair_list_G2H[x][y][i_a][i_e] && op->pair_list_G2H[x][y][i_b][i_f] && op->pair_list_G2H[x][y][i_c][i_g]){
                              flag1=0;  
                              for ( l=1; l <= op->distinguisher[y][e][f][g][0]; l++){   
                                i_l=op->distinguisher[y][e][f][g][l];
                                if ( i_l > -1 && op->pair_list_G2H[x][y][ind][i_l])   
                                  flag1=1; 
                              }

                              if (!flag1) 
                                op->distinguisher[x][a][b][c][d]=-1;
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

}

void make_choice_distinguisher(GRAPHS *op)
{
  int x,a,b,c,d;
  int i_a,i_b,i_c;

  for (x=0; x < op->num_vert_G; x++){ 
    for (a=0; a < op->list_G2H[x][0]; a++){
      for (b=0; b < op->list_G2H[x][0]; b++){
        for (c=0; c < op->list_G2H[x][0]; c++){
          i_a=op->list_G2H[x][a+1];  
          i_b=op->list_G2H[x][b+1]; 
          i_c=op->list_G2H[x][c+1];
          if (op->list_G2H[x][a+1] > -1 && op->list_G2H[x][b+1] > -1 && op->list_G2H[x][c+1] > -1 ){
            for (d=1; d<= op->distinguisher[x][a][b][c][0]; d++){
              if(op->distinguisher[x][a][b][c][d] > -1){
                if (i_a== op->distinguisher[x][a][b][c][d]) {
                 op->distinguisher[x][a][b][c][0]=1; 
                 op->distinguisher[x][a][b][c][1]=i_a;
             
                }
           
                if (i_c== op->distinguisher[x][a][b][c][d]) {
                  op->distinguisher[x][a][b][c][0]=1; 
                  op->distinguisher[x][a][b][c][1]=i_c;
                }
              }
            }
          } 
        }
      }
    }
  }

}

int check_maltsev(GRAPHS *op)
{
  int x,y,a,b,c,d,e,f;
  int i_a,i_b,i_c,i_d,i_e,i_f;
  int p,q;
  int flag1;

  for (x=0; x < op->num_vert_G; x++){ 
    for (a=0; a < op->list_G2H[x][0]; a++){
      for (b=0; b < op->list_G2H[x][0]; b++){
        for (c=0; c < op->list_G2H[x][0]; c++){
          if (op->list_G2H[x][a+1] > -1 && op->list_G2H[x][b+1] > -1 && op->list_G2H[x][c+1] > -1 ){
            flag1=0;
            for (p=1; p<= op->distinguisher[x][a][b][c][0]; p++){
              if(op->distinguisher[x][a][b][c][p] > -1) 
                flag1++;
            } 
            if( ! flag1)  
              return 0; 
            if ( flag1 > 1) 
              return flag1;
        
          }
        }
      }
    }
  }


  for (x=0; x < op->num_vert_G; x++){ 
    for (y=0; y < op->num_vert_G; y++){
      if (op->graph_g[x][y] && op->activeG[x] && op->activeG[y]){
        for (a=0; a < op->list_G2H[x][0]; a++){
          for (b=0; b < op->list_G2H[x][0]; b++){
            for (c=0; c < op->list_G2H[x][0]; c++){
              if (op->list_G2H[x][a+1] > -1 && op->list_G2H[x][b+1] > -1 && op->list_G2H[x][c+1] > -1 ){
                for (d=0; d< op->list_G2H[y][0]; d++){
                  for (e=0; e< op->list_G2H[y][0]; e++){
                    for (f=0; f< op->list_G2H[y][0]; f++){
                      if (op->list_G2H[y][d+1] > -1 && op->list_G2H[y][e+1] > -1 && op->list_G2H[y][f+1] > -1 ){
                        i_a=op->list_G2H[x][a+1]; i_b=op->list_G2H[x][b+1]; i_c=op->list_G2H[x][c+1];
                        i_d=op->list_G2H[y][d+1]; i_e=op->list_G2H[y][e+1]; i_f=op->list_G2H[y][f+1];
                        if (op->pair_list_G2H[x][y][i_a][i_d] && op->pair_list_G2H[x][y][i_b][i_e] && op->pair_list_G2H[x][y][i_c][i_f] ){
                          for (p=1; p<= op->distinguisher[x][a][b][c][0]; p++){
                            if(op->distinguisher[x][a][b][c][p] > -1){
                              for (q=1; q <= op->distinguisher[y][d][e][f][0]; q++){
                                if(op->distinguisher[y][d][e][f][q] > -1 ){
                                  if ( ! op->graph_h[op->distinguisher[x][a][b][c][p]][op->distinguisher[y][d][e][f][q]])
                                    return 0; 
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
    }
  }

  return 1;

}