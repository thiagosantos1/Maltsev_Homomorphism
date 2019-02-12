#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*struct Instance{
 
 //int ** G;
 
 int node;
 
 int **Change_list; 
// int m;
 
 //int G[n][m];
 
 //int L1[n_g][n_h];
 
// int L2[n_g][n_g][n_h][n_h]; 
 
 
	
} ;*/ 



int** GraphConstruct( int *number, char *file_name) {
	
	// read the file and return the number of vertices and allocate memory for GraphPointer 
	
	FILE *fp;
	
	fp=fopen (file_name, "r");
	
    if (fp ==NULL ) {
    	printf("no file \n");
		return 0;
    }
    
	int n,m;
	
	
	fscanf(fp,"%d", &n);
	fscanf(fp,"%d", &m);
	
	//printf("%d \n",m);
	
	//int G1[n][n];
	
	
	
	
	//GraphPointer= malloc(n);
	
	//int i,a,b; 
	// allocate memory
	int i,j;

	int **GP= (int **) malloc( n * sizeof(int *)); 
	for (i=0;i < n; i++) {
		GP[i] = (int *) malloc( n * sizeof(int *)); 
	} 
	// read number of nodes and number of edges 

	
	
	// read the pair of adjacent vertices; 
	for (i=0;i < n;i++)
		for(j=0; j< n; j++)
			GP[i][j]=0; 
	
	// read the pair of adjacent vertices; 
	int a,b;
	
	for (i=0; i< n; i++)
		for (j=0; j< n; j++)
			GP[i][j]=0;
	
	for (i=0; i< m; i++) {
		fscanf(fp,"%d", &a);
		fscanf(fp,"%d", &b);
		GP[a][b]=1;
	    
	}
	
	
	fclose(fp);
	*number =n; 
	
	// printing G 
/*	printf("the graph is \n"); 
	for (i=0; i < n ; i++)
	{
		for (j=0; j< n; j++)
		  printf("%d ",GP[i][j]);
	    printf("\n");
	} */
			  	
	return 	GP;
	
	
	
//	return n;
}


int Odd_Even_bits ( int x)
{
	
  int count=0;
  
  while ( x> 0)
  {
   if (x %2 )
	   count++;
   x=x/2;	  	

  }	
  if (count %2) 
	  return 0;
  else 
	  return 1;
  
	
}

int Which_neighbor (int ** G, int n_g, int x, int y)
{
	int a,i;
	
	i=0; 
	
	for (a=0; a< n_g; a++) {
		if (G[x][a])
			i++;
		if ( a== y)
			return i-1; 
	}
		
	printf("error in Which neighbor\n");
	return 0; 	
}

int which_bit (int l, int i )
{
	int t=0;
	
	while ( t < i)
	{
		l=l/2;
	    t++;
	}
	//for ( t=0; t < i ; t++)
	//{
	if (l % 2 ) 
			return 1;
	else 
			return 0;
		
	
	
}
int ** Initial_L1_lists(int **G, int n_g )
{
	
	int x,y,a,b;
	int counter=0; 
	
	
	int degree[n_g];
	//int Eq_parity[n_g];
	
	int range[n_g];
	
	for (x=0; x< n_g; x++)
		degree[x]=0;
	
	//for (x=1; x< n_g; x++)
	//	Eq_parity[x]=0;
	// Eq_parity[0]=1; 
	
	for (x=0; x< n_g; x++)
		for (y=0; y< n_g; y++)
			if (G[x][y])
				degree[x]++;
	
	int n_h=2; 
	
	for (x=1; x< n_g; x++)
	{
		range[x]=n_h;
		n_h=n_h+(int) pow(2,degree[x]-1);
	}    
	
	printf("%d ", n_h);
	
	
	int **L1;
	
	L1= (int **) malloc( n_g * sizeof(int *)); 
	for (x=0;x < n_g; x++) 
		L1[x] = (int *) malloc( n_h * sizeof(int *)); 
	
	
	
	for( x=0; x< n_g; x++)
		for (a=0;a < n_h; a++)
			L1[x][a]=0;
	
	
	L1[0][0]=1;
	L1[0][1]=1;
	
	counter=2;
	
	for ( x=1; x< n_g; x++)
	 {
		 y= (int) pow(2,degree[x]-1);
		 for (a=0; a< y; a++)
			 L1[x][counter+a]=1;
		 counter+=y;
		  
	 }	
	 
    
	 
	 int i,j,l,k,max_range_x,max_range_y;
	 
	 int H[2][n_h*n_h];
	 int edge_counter=0; 
	 
	 // adding edges between L[0] && L[1]
	 
	 max_range_y=(int) pow(2,degree[1]);
	 //int vertex=0;
	 for ( k=0; k< max_range_y; k++) // for 00 in L1[0]
	  if ( Odd_Even_bits(k) ) 	 {
		
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
	  //  vertex++;
		
	   }
	   int vertex_x=0;
	   int vertex_y=0;
	  for ( x=1; x < n_g; x++)
		 for (y=x+1; y < n_g; y++)
	 		if (G[x][y]){
				// x is the i-th neighbor of y
				i= Which_neighbor(G,n_g,x,y);
				j= Which_neighbor(G,n_g,y,x);
				max_range_x = (int) pow(2,degree[x]);
				max_range_y= (int) pow(2,degree[y]);
				vertex_x=0; 
				for (l=0; l< max_range_x; l++) 
					if ( Odd_Even_bits(l)) 
					{
						 //vertex_y=0;
						 for ( k=0; k< max_range_y; k++)
						 if ( Odd_Even_bits(k)) {
							  
			    		  if ( which_bit(l,i) == which_bit(k,j) ) { 	
							  H[0][edge_counter]=range[x]+l/2;
							  H[1][edge_counter]=range[y]+k/2;
				 
							  H[0][edge_counter+1]=range[y]+k/2; 
							  H[1][edge_counter+1]=range[x]+l/2;
							  edge_counter+=2;
			    		  }	
						 // vertex_y++ ;
						  
						 }				 	 	
					//vertex_x++;
				   }				
					 
			}
		 
	
	/*printf ("the lists are \n");
 	for (y=0; y< n_g ; y++) {
		
 	    printf("L(%d )= {",y);
 		for (a=0; a< n_h; a++) 
 			if (L1[y][a]==1 )
 				printf("%d , ",a);
 		printf("} \n");
 	} 
	
	printf("enter a number \n");
	scanf("%d", &x); */
	
	
	FILE * fp_list;
    fp_list=fopen("List_file.txt","w"); 
	
    if (fp_list ==NULL ) {
    	printf("no list file \n");
		return 0;
    } 
	
	fprintf(fp_list,"%d ",0);
	fprintf(fp_list,"%d ",2);
	fprintf(fp_list,"%d ",0);
	fprintf(fp_list,"%d\n",1);
		
	for ( x=1; x< n_g; x++)
    {
		fprintf(fp_list,"%d ",x);
		y=(int) pow (2,degree[x]-1);
		fprintf(fp_list,"%d ",y);
		
		for (a=0; a < n_h; a++)
			if (L1[x][a])
			 fprintf(fp_list,"%d ",a);
		
		fprintf(fp_list,"\n");
     } 
    fclose (fp_list);	
	
	
	/*printf ("the edges of H  are \n");
    for (i=0; i < edge_counter; i++)
		printf ("%d %d \n",H[0][i],H[1][i]);
	printf("\n");*/
		 
    FILE * fp;
   
    fp=fopen("graph-H.txt","w"); 
	
    if (fp ==NULL ) {
    	printf("no list file \n");
		return 0;
    } 
	
	fprintf (fp,"%d ", n_h);
	fprintf (fp,"%d", edge_counter);
	fprintf (fp,"\n");
	
	for (i=0; i < edge_counter; i++)
	{
		fprintf(fp, "%d %d",H[0][i],H[1][i]);
	    fprintf(fp,"\n");
	}
	
	fclose(fp);
	
	
	 /*  if (with_file == 0) 
	
		for (x=0;x < n_g; x++) 
			for (a=0; a < n_h; a++)
	
				L1[x][a]=1;
	
	else {
		
		FILE *fp;
	
		fp=fopen ("List_file.txt", "r");
	
	    if (fp ==NULL ) {
	    	printf("no list file \n");
			return 0;
	    }
		
		for (x=0;x < n_g; x++) 
			for (a=0; a < n_h; a++)
				L1[x][a]=0;
		
		for (x=0; x< n_g; x++) // assume everyone has a none-empty list 
		{
			
			fscanf(fp,"%d",&y); // the first one from G; 
			fscanf (fp,"%d",&counter); // how many in the list of y 
			for (i=0; i < counter; i++)
			{		
				fscanf (fp,"%d",&b);   // b is the element to be in L1(y) 
				L1[y][b]=1;
			}  
		}
	 
	}*/
	

	
	return L1;
	
	
}

int main()
{
 
 
	int **G; 
	int **H;
   
    int n_g,n_h;
	int current=1;
	
	G=GraphConstruct(&n_g, "graph-G.txt");
	
	//H=GraphConstruct(&n_h,"graph-H.txt");
	
//	printf("%d\n", n);
	
	int **L1;
	//int ****L2;
	
	
 /*	int i,j; 
		for (i=0;i < n_g;i++) {
	
		for(j=0; j< n_g; j++)
			printf("%c ",G[i][j]); 
		printf("\n");
	} 
	
	//printf("\n");
	
		for (i=0;i < n_h;i++) {
	
		for(j=0; j< n_h; j++)
			printf("%c ",H[i][j]); 
		printf("\n");
	}  */
	

	L1=Initial_L1_lists(G,n_g);
	//L2=Initial_L2_lists(G,H,L1,n_g,n_h);
	
	
	
	
	//flag=PreProcessing (G,H, L1, L2, n_g,n_h,current);

	//flag=Check_lists(G,L1,L2,n_g,n_h,current);
	
	
	
 return 0;
 
}