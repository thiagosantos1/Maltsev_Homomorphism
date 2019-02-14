#include<stdio.h>
#include<stdlib.h>

int **G;
int **H; 

struct instance{ // the instance is used inside Symmetric_Diff function  
 
 int n;   // number of nodes 
 int *PG;  // Active vertices in G
 int **PL1; // Lists 
 int ****PL2;  // Pair Lists

 
	
} ; 


int** GraphConstruct( int *number, char *file_name) {
	
	// read the file and return the number of vertices and allocate memory for GraphPointer 
	
	FILE *fp;
	
	fp=fopen (file_name, "r");
	
    if (fp ==NULL ) {
    	printf("no file \n");
		return 0;
    }
    
	int n,m,a,b;
	
	
	fscanf(fp,"%d", &n); // read the number of vertices 
	fscanf(fp,"%d", &m);  // read the number of edges of G
	
	// allocate memory for the input graph G which is read from a file graph-G.txt 
	int i,j;

	int **GP= (int **) malloc( n * sizeof(int *)); 
	for (i=0;i < n; i++) {
		GP[i] = (int *) malloc( n * sizeof(int *)); 
	}
	// read number of nodes and number of edges 
	
	// set GP to zero the adjacency matrix is empty
	for (i=0;i < n;i++)
		for(j=0; j< n; j++)
			GP[i][j]=0; 
	
	// read the pair of adjacent vertices from the file 
	
	for (i=0; i< m; i++) {
		fscanf(fp,"%d", &a);
		fscanf(fp,"%d", &b);
		GP[a][b]=1;
	    
	}
	
	fclose(fp);
	*number =n; 
  	
	return 	GP;
	
	
}


// initial the L lists 

int ** Initial_L1_lists(int n_g, int n_h, int with_file )
{
	
	int x,y,a,b;
	int counter=0; 
	int i; 
	int **L1;
	
	L1= (int **) malloc( n_g * sizeof(int *)); 
	for (x=0;x < n_g; x++) 
		L1[x] = (int *) malloc( n_h * sizeof(int *)); 
	
	
	 if (with_file == 0) 
	
		for (x=0;x < n_g; x++) 
			for (a=0; a < n_h; a++)
	
				L1[x][a]=1;
	
	else {
		
		FILE *fp;
	
		fp=fopen ("../etc/list_file.txt", "r");
	
	    if (fp ==NULL ) {
	    	printf("no list file \n");
			return 0;
	    }
		
		for (x=0;x < n_g; x++) 
			for (a=0; a < n_h; a++)
				L1[x][a]=0;
		
		// read from the list_file... 
		// first elment is the node of G, then number of elements in the list and the the vertices of H is the list 
		
		for (x=0; x< n_g; x++) // assume everyone has a none-empty list 
		{
			
			fscanf(fp,"%d",&y); // the first one from G; 
			fscanf (fp,"%d",&counter); // how many in the list of y 
			L1[y][0]=counter; 
			for (i=0; i < counter; i++)
			{		
				fscanf (fp,"%d",&b);   // b is the element to be in L1(y) 
				L1[y][i+1]=b;
			}  
		}
	 
	}

	return L1;
		
}

int **** Initial_L2_lists(int **L1, int n_g, int n_h)
{
	
	int x,y,a,b;
	int temp_a,temp_b;
	int ****L2; 
	
	// allocate memory for pair lists 
	
	L2= (int ****) malloc( n_g * sizeof(int *)); 
	for (x=0; x< n_g; x++)
		L2[x]= (int ***) malloc( n_g * sizeof(int *)); 
	
	
	for (x=0; x< n_g; x++)
		for (y=0; y<n_g; y++)
			L2[x][y]=(int **) malloc( n_h * sizeof(int *)); 
	
	for (x=0; x< n_g; x++)
		for (y=0; y<n_g; y++)
			for (a=0; a < n_h; a++)
				L2[x][y][a]=(int *) malloc( n_h * sizeof(int *));
	
	
	// initially everything is zero 
	for (x=0;x < n_g; x++) 
		for (y=0; y < n_g; y++)
			for (a=0; a< n_h; a++) 
	   		 for (b=0; b < n_h; b++) 
				 L2[x][y][a][b]=0;
		    
		
	
	for (x=0;x < n_g; x++) 
		for (y=x+1; y < n_g; y++)
			for (a=1; a<= L1[x][0]; a++) 
	   		 for (b=1; b <= L1[y][0]; b++) 
	         
				if ( (L1[x][a]>-1) && (L1[y][b] > -1) ){
					temp_a=L1[x][a];
					temp_b=L1[y][b];
					
			  	    L2[x][y][temp_a][temp_b]=1;
		      	    L2[y][x][temp_b][temp_a]=1; 
		        }
		
	for (x=0;x < n_g; x++) 
		for (a=1; a<= L1[x][0]; a++) 
			if (L1[x][a] > -1)
				L2[x][x][L1[x][a]][L1[x][a]]=1;  
	
	for (x=0;x < n_g; x++) 
		for (y=0; y < n_g; y++)
			
			if (G[x][y]==1)
				
			 for (a=1; a<= L1[x][0]; a++) 
				 if (L1[x][a] > -1 )
				 {
					 for (b=1; b <= L1[y][0]; b++)  { 
	 					temp_a=L1[x][a];
	 					temp_b=L1[y][b];
						if ( (L1[y][b] >-1) && H[temp_a][temp_b]==1)
					 	{	 
						  L2[x][y][temp_a][temp_b]=1;
						  L2[y][x][temp_b][temp_a]=1;
						}
						else {
						  L2[x][y][temp_a][temp_b]=0; 
						  L2[y][x][temp_b][temp_a]=0;
						 }
				      }
				 }
	
	
	return L2; 
	
	
}

	
int PreProcessing (int *ActiveG, int **L1,  int ****L2, int n_g) 
{
	int update=1;
	
	/// arc consistency 
	int x,y,z,a,b,c,d;
	int flag=0;
	int i; 
	int i_a,i_b; 
	
	//index for a and b in the list of x and y 
	//printf("beginning of preprocessing the active vertices are \n");
	 // arc consistency 	
	 
	/*for (i=0; i< n_g; i++)
		if(ActiveG[i])
			printf("%d ",i);
	
	printf("\n");*/
	
	// scanf("%d",&i);
	// int remove_list[2][n_g * n_h+1];
	// for (i=0; i< n_g*n_h+1; i++)
	//	 remove_list[0][i]=remove_list[1][i]=0;
	 
	 int counter=0; 
	 while (update) {
		
	 		update=0; 
		
	 		for (x=0;x<n_g;x++)
	 		   for (a=1; a<= L1[x][0]; a++) 
	 			if( L1[x][a] > -1 ) 
	 			{ 
				  i_a=L1[x][a];
				  for( y=0; y < n_g ; y++)
	 				 if  ( G[x][y] && ActiveG[x] && ActiveG[y]) {
	 					 flag=0;
	 					 for (b=1; b<= L1[y][0] && !flag ; b++)
	 						if ( (L1[y][b]> -1)  && H[ i_a ][ L1[y][b] ] ) 	
	 							flag=1;
	 				     if (! flag) {
				     
	 					   update=1;
	 			 	       L1[x][a]=-1;	
						  
	 		 	         }		
	 				 }		
			 
				 }
				 
			 for (y=0; y<n_g; y++)
	 			for (b=1; b<= L1[y][0] ; b++)
	 				if( L1[y][b]> -1 )  
	 				{
	 					
					 i_b=L1[y][b];
					 for( x=0; x < n_g ; x++)
	 					 if  ( G[x][y] && ActiveG[x] && ActiveG[y] ) {
	 						 flag=0;
	 						 for (a=1; a <=L1[x][0] && !flag ; a++)
	 							if ( (L1[x][a]> -1) && H[ L1[x][a] ][ i_b ] ) 	
	 								flag=1;
	 					     if (! flag) {
				     
	 						   update=1;
	 				 	       L1[y][b]=-1; 
							  
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
		for ( x=0;x < n_g; x++)
			 for (y=0;y < n_g;y++)
					 for (a=1; a<= L1[x][0]; a++) 
		               for (b=1; b<= L1[y][0] ; b++) 
						  if ( (L1[x][a] >-1 ) && (L1[y][b] > -1 ) )  
						  {
							 i_a=L1[x][a]; i_b=L1[y][b];
							 if ( L2[x][y][i_a][i_b] && ActiveG[x] && ActiveG[y] )
							 
					   			
								for (z=0; z<n_g; z++) 
									if (ActiveG[z])
		                            {
									  flag=0;
							    	  
									  for (c=1; c<=L1[z][0] ; c++)
									  {
									    i_c=L1[z][c];
									   if( ( i_c >-1)  && L2[x][z][i_a][i_c] && L2[y][z][i_b][i_c] ) 
										   flag=1;
								      }
									  
									  if (!flag) {
									
										L2[x][y][i_a][i_b]=0;
										L2[y][x][i_b][i_a]=0;
									
									 	update=1; 
								     }
						           }			
					      }
	}  
	
	
		 	
	
 	 for (x=0; x < n_g; x++)  
		 if (ActiveG[x])
		   for (a=1; a<= L1[x][0]; a++)  
               if ( L1[x][a] > -1 ) 
				{   
				 i_a=L1[x][a];
					
				 for ( y=0; y < n_g ; y++)
					if (ActiveG[y])
				      {	 
                        flag=0; 
 						//
						for (b=1; b<= L1[y][0] && !flag ; b++)
 					    {
				    	    i_b=L1[y][b]; 
 							if ( (i_b >-1 ) && L2[x][y][i_a][i_b] )
 								flag=1;
 					    }  
					  
 				       if(!flag )
 					    L1[x][a]=-1; 
 			          }

				  }
	
		
	
				for( x=0; x< n_g; x++)
					for (y=0; y< n_g; y++)
						if (ActiveG[x] && ActiveG[y])
						{
							flag=0;
						    for (a=1; a<= L1[x][0]; a++)  
							  for (b=1; b<= L1[y][0]; b++)  
								if ( (L1[x][a]> -1) && (L1[y][b]> -1) && L2[x][y][ L1[x][a] ][ L1[y][b] ])
									flag=1;
						
			   		        if (! flag) 
			 			     return 0;
				       }
					   
	
	return 1;
	
}



int Free_lists(int *ActiveG, int  **L1, int ****L2, int n_g, int n_h) 
{
	
	int x,y,c,d;
	int i,j;
	
	free (ActiveG);
	
	for (x=0;x<n_g;x++)
			free (L1[x]);
	
	for (x=0; x< n_g; x++)
		for (y=0; y< n_g; y++)
			for (c=0;c <n_h; c++)
				free(L2[x][y][c]);
	for (x=0;x<n_g; x ++)
		for (y=0; y< n_g; y++)
			free(L2[x][y]);
	
	for (x=0;x<n_g;x++)
			free (L2[x]);
	
	return 0;	
} 

int Is_Rectangle (int x, int a, int b, int n_g, int ** L1, int ****L2 )
{
	int y,c;
	int flag =0; 
	if (a == b) 
		return 0;
	if ( L1[x][a] < 0 )
		return 0; 
	if ( L1[x][b] < 0 )
		return 0; 
	
	flag =0; 
    for (y=0; y< n_g; y++)
		for (c=1; c <= L1[y][0]; c++)
			if ( (L1[y][c] > -1) && L2[x][y][a][ L1[y][c] ] && L2[x][y][b][ L1[y][c] ] && x!= y)
				flag=1; 
	
	return flag; 
}

//int max_call=0;
int Print_Instance(int *ActiveG, int **L1,  int n_g, int x, int a, int current){
	
	int y,b;
	
	printf("new graph for x=%d , a=%d, current =%d \n",x,a,current);
	for (y=0; y< n_g; y++)
		if (ActiveG[y])
			printf("%d,",y);
	printf(" \n");
	
	printf(" the lists are \n");
	
	
	
	for (y=0; y< n_g ; y++) 
	if ( ActiveG[y]) {
	    printf("L(%d )=",y);
		for (b=1; b<= L1[y][0]; b++) 
			if (L1[y][b]>-1 )
				printf("%d , ",L1[y][b]);
		printf("\n");
	} 
	
	return 0;
	
}

int Pruning_the_lists ( int *ActiveG, int **L1, int ****L2, int n_g)
{
   int x,a,b;
   int flag=0;
   for ( x=0 ; x < n_g; x++) 
 	if (ActiveG[x]) {
 		
 		for (a=L1[x][0] ; a >= 1 ; a--)   //for (a=n_h-1; a > 0 ; a--)
 			if (L1[x][a] > -1 )
 				for (b = L1[x][0] ; b >= 1; b --)  //for (b=n_h-1; b > 0; b--)
 					if ( (L1[x][b] > -1 )   && a!=b) {
						
 						L1[x][b]=-1;		//it was L1[x][b]=0;
						
 						//printf ("b= %d is deleted from x=%d \n",b,x); 
						
 						flag=PreProcessing(ActiveG, L1,L2,n_g); 
 						if (! flag) 
 							return 0; 
 					}
										
 	}
	return 1;
}


int main()
{
 

   
    int n_g,n_h;
	int current=1;
	
	G=GraphConstruct(&n_g, "../etc/graph_G.txt");
	
	H=GraphConstruct(&n_h,"../etc/graph_H.txt");
	
	
	int **L1;
	int ****L2;
	
	L1=Initial_L1_lists(n_g,n_h,1);
	L2=Initial_L2_lists(L1,n_g,n_h);
	
 	int flag;
	int Active[n_g];
	int i; 
	for (i=0; i< n_g; i++)
		Active[i]=1;
		
	
	flag=PreProcessing( &Active[0],L1,L2,n_g);
    
	
	int x,y,a,b;

	 
	
	
	for (x=0; x< n_g ; x++) 
		for (y=0; y< n_g ; y++) 
	    { //printf("L(%d )=",x);
		   printf("L(%d,%d)=",x,y);
		   for (a=0; a< n_h; a++) 
			for (b=0; b< n_h; b++) 
				if ( L2[x][y][a][b])
					printf("{%d,%d},",a,b);
		printf("\n");
	   }  
	
	printf("the output is %d \n", flag); 
	
 return 0;
 
}