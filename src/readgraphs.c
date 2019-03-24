/* Authors: Arash Rafiey Thiago Santos. --> https://github.com/thiagosantos1/
   Copyright (C)
*/

#include <graphs.h>  
#include <readgraphs.h>


// if already have all graphs and list saved
void init_graphs_lists(GRAPHS *op, USER_PARAMS * ip, int read_g)
{ 
  // if we are create new H and list, we already have read graph G
  if(read_g >0)
    init_graph_g(op,ip->graph_g);

  init_graph_h(op,ip->graph_h);
  init_list(op,ip->list_homom); 
  init_pair_list(op);

  // print_graphH(op); 
  // print_graphG(op);
  // print_list(op);
  // print_init_pairs(op);

}

void init_graph_g(GRAPHS *op, char * file_name)
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

  op->graph_g = malloc(op->num_vert_G * sizeof(int *));
  for(i=0; i<op->num_vert_G; i++){
    op->graph_g[i] = malloc(op->num_vert_G * sizeof(int));
    memset(op->graph_g[i],0,op->num_vert_G * sizeof(int) );
  }

  // initialize degrees to zero
  op->degrees_g = (int *)calloc(op->num_vert_G, sizeof(int)); 


  for (int x=0; x< op->num_E_G; x++) {
    fscanf(fp, "%s %s", str1,str2);
    i = atoi(str1);
    j = atoi(str2);
    op->graph_g[i][j] = 1;
    op->degrees_g[i]++;
  }

  // Set all nodes to active/availave. 
  op->activeG = malloc(op->num_vert_G * sizeof(int));
  for(i=0; i< op->num_vert_G; i++)
    op->activeG[i] = 1;



}

void init_graph_h(GRAPHS *op, char * file_name)
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

  op->graph_h = malloc(op->num_vert_H * sizeof(int *));
  for(i=0; i<op->num_vert_H; i++){
    op->graph_h[i] = malloc(op->num_vert_H * sizeof(int));
    memset(op->graph_h[i],0,op->num_vert_H * sizeof(int) );
  }

  // initialize degrees to zero
  op->degrees_h = (int*) calloc(op->num_vert_H, sizeof(int));

  for (int x=0; x< op->num_E_H; x++) {
    fscanf(fp, "%s %s", str1,str2);
    i = atoi(str1);
    j = atoi(str2);
    op->graph_h[i][j] = 1;
    op->degrees_h[i]++;
  }

}

void init_list(GRAPHS *op, char * file_name)
{
  FILE * fp = fopen(file_name, "r");
  if(fp == NULL){
    fprintf(stderr, "\nError opening file %s\n",file_name );
    exit(0);
  }

  int i,x,y,b,counter;
  op->list_G2H = malloc(op->num_vert_G * sizeof(int *));
  for(i=0; i<op->num_vert_G; i++){
    op->list_G2H[i] = malloc(op->num_vert_H * sizeof(int));
    memset(op->list_G2H[i],0,op->num_vert_H * sizeof(int) );
  }

  for (x=0; x < op->num_vert_G; x++){ // assume everyone has a none-empty list 
    
    fscanf(fp,"%d",&y); // the first one from G; 
    fscanf (fp,"%d",&counter); // how many in the list of y 
    op->list_G2H[y][0]=counter; 
    for (i=0; i < counter; i++){   
      fscanf (fp,"%d",&b);   // b is the element to be in L1(y) 
      op->list_G2H[y][i+1]=b;
    }  
  }

} 

void init_pair_list(GRAPHS *op)
{
  int x,y,a,b;
  int temp_a,temp_b;

  // allocate memory for pair lists 
  op->pair_list_G2H = (int ****) malloc(op->num_vert_G * sizeof(int *)); 
  for (x=0; x< op->num_vert_G; x++)
    op->pair_list_G2H[x]= (int ***) malloc(op->num_vert_G * sizeof(int *)); 
  
  
  for (x=0; x< op->num_vert_G; x++)
    for (y=0; y<op->num_vert_G; y++)
      op->pair_list_G2H[x][y]=(int **) malloc(op->num_vert_H * sizeof(int *)); 
  
  for (x=0; x< op->num_vert_G; x++)
    for (y=0; y<op->num_vert_G; y++)
      for (a=0; a < op->num_vert_H; a++)
        op->pair_list_G2H[x][y][a]=(int *) malloc(op->num_vert_H * sizeof(int *));


  // initially everything is zero 
  for (x=0;x < op->num_vert_G; x++) 
    for (y=0; y < op->num_vert_G; y++)
      for (a=0; a< op->num_vert_H; a++) 
        for (b=0; b < op->num_vert_H; b++) 
          op->pair_list_G2H[x][y][a][b]=0;


  for (x=0;x < op->num_vert_G; x++) {
    for (y=x+1; y < op->num_vert_G; y++){
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

  for (x=0;x < op->num_vert_G; x++) 
    for (a=1; a<= op->list_G2H[x][0]; a++) 
      if (op->list_G2H[x][a] > -1)
        op->pair_list_G2H[x][x][op->list_G2H[x][a]][op->list_G2H[x][a]]=1; 


  for (x=0;x < op->num_vert_G; x++){
    for (y=0; y < op->num_vert_G; y++){

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

void print_init_pairs(GRAPHS *op)
{
  int x,y,a,b;
  printf("Pairs\n");
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
  printf("\n\n"); 
}

void print_graphG(GRAPHS *op)
{
  printf("Graph G\n");
  printf("Num V: %d\tNum of E:%d\n", op->num_vert_G, op->num_E_G);
  int x,y;
  for (x=0; x< op->num_vert_G ; x++){ 
    for (y=0; y< op->num_vert_G ; y++){ 
      if(op->graph_g[x][y] >0)
        printf("%d %d\n", x,y );
    }
  }
  printf("\n\n");
}

void print_graphH(GRAPHS *op)
{
  printf("Graph H\n");
  printf("Num V: %d\tNum of E:%d\n", op->num_vert_H, op->num_E_H);
  int x,y;
  for (x=0; x< op->num_vert_H ; x++){ 
    for (y=0; y< op->num_vert_H ; y++){ 
      if(op->graph_h[x][y] >0)
        printf("%d %d\n", x,y );
    }
  }
  printf("\n\n");
}

void print_list(GRAPHS *op)
{

  int x,i;
  printf("List\n");
  for (x=0; x < op->num_vert_G; x++){
    printf("List for V: %d\n{ ",x );
    for (i=0; i < op->list_G2H[x][0]; i++){   
      printf("%d, ",op->list_G2H[x][i+1]);
    }
    printf("}\n");  
  }
  printf("\n\n");
}






