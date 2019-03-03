#include<stdio.h>
#include<stdlib.h>

int **G;
int **H; 

struct Instance{ // the instance is used inside Symmetric_Diff function  
 
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
	
		fp=fopen ("List_file.txt", "r");
	
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
int Print_Instance(int *ActiveG, int **L1,  int n_g){
	
	int y,b;
	
	printf("new graph for \n");
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

int ***** Initial_Distinguisher(int **L1, int ****L2, int n_g, int n_h  ) 
{
	
	
	int x,a,b,c,d;
	
	
	int *****PDis;
	
	PDis= (int *****) malloc( n_g * sizeof(int *)); 
	for (x=0; x< n_g; x++)
	   
		PDis[x]= (int ****) malloc( L1[x][0] * sizeof(int *)); 
	 
	
	
 	for (x=0;x < n_g; x++ )
	  for (a=0; a< L1[x][0]; a++)	  
		  PDis[x][a]= (int ***) malloc( L1[x][0] * sizeof(int *)); 	
	
	 for (x=0;x < n_g; x++ )
	  for (a=0; a< L1[x][0]; a++)	
		  for (b=0; b< L1[x][0]; b++)  
		   PDis[x][a][b]= (int **) malloc( L1[x][0] * sizeof(int *)); 	
	
	 for (x=0;x < n_g; x++ )
	  for (a=0; a< L1[x][0]; a++)	
		  for (b=0; b< L1[x][0]; b++)  
		    for (c=0; c< L1[x][0]; c++)
		      PDis[x][a][b][c]= (int *) malloc( (n_h+1) * sizeof(int *));  
		  
    for (x=0; x < n_g; x++)
	  for (a=0; a< L1[x][0]; a++)
	    for (b=0; b< L1[x][0]; b++)
	     for( c=0; c< L1[x][0]; c++ )
	       for( d=0; d<= n_h; d++ )
			PDis[x][a][b][c][d]=0; 
	     
	int temp, counter;
     for (x=0; x < n_g; x++)
	  for (a=0; a< L1[x][0]; a++)
	    for (b=0; b< L1[x][0]; b++)
	      
	     for( c=0; c< L1[x][0]; c++ )
	     {
	           counter=0;
		   for( d=1 ; d<= L1[x][0]; d++ )
	           
	            {
				
				  temp=L1[x][d];  
				//printf("temp= %d \n",temp);
				  if ( temp > -1)
				  {
				    counter++;
				    PDis[x][a][b][c][ counter ] =temp;    
				    
				    
				  }
	 
		 
		   }
	   
	       PDis[x][a][b][c][0]=counter;   
	    }
	 
	 return PDis;
	
}

int Print_Distinguisher (int *ActiveG, int **L1, int ****L2, int n_g, int *****Dis)
{
  int x,a,b,c,d;
  for ( x=0; x < n_g; x++)
                  for (a=0; a< L1[x][0]; a++)
		    for (b=0; b < L1[x][0]; b++)
			for (c=0; c < L1[x][0]; c++)
      	
			  if ( L1[x][a+1] > -1 && L1[x][b+1] > -1 && L1[x][c+1] > -1 )
			    {
			     printf("Dis[%d][%d][%d][%d] : ",x,L1[x][a+1],L1[x][b+1],L1[x][c+1]);
			     for (d=1; d<= Dis[x][a][b][c][0] ; d++) 
			      if ( Dis[x][a][b][c][d] > -1 )
				   printf("%d ",Dis[x][a][b][c][d]);
      
			    printf("\n");
			 }	
			 
		return 1;	 
}

int Arc_Consistency_Dis( int *ActiveG, int **L1, int ****L2, int n_g,  int *****Dis ){
	
	 
	 int x,y,a,b,c,d,e,i_a,i_b,i_c,i_e; 
	 int f,g,i_f,i_g,l,i_l,flag1,ind; 
	 
	 for (x=0; x < n_g; x++)
	   for (a=0; a < L1[x][0]; a++)
		 for (b=0; b < L1[x][0]; b++)
		   for (c=0; c < L1[x][0]; c++)
		   { 
		      i_a=L1[x][a+1]; i_b=L1[x][b+1]; i_c=L1[x][c+1]; 
		      
		      if (L1[x][a+1] > -1 && L1[x][b+1] > -1 && L1[x][c+1] > -1 )
		      
			 for (d=1; d <=Dis[x][a][b][c][0]; d++ )	
			 {
			   ind=  Dis[x][a][b][c][d];
			   if ( ind > -1 )      
			     for (y=0; y < n_g; y++)  
			         if ( y != x)
				    for (e =0; e < L1[y][0]; e++)
		  		           for (f =0; f < L1[y][0]; f++)
		  		                for (g =0; g < L1[y][0]; g++)
						{  
						  i_e=L1[y][e+1]; i_f=L1[y][f+1]; i_g=L1[y][g+1];
						  
						  if (L1[y][e+1] > -1 && L1[y][f+1] > -1 && L1[y][g+1] > -1 )
						  
						  
						   if (L2[x][y][i_a][i_e] && L2[x][y][i_b][i_f] && L2[x][y][i_c][i_g]) 
						   {
						    flag1=0;  
					           
						    for ( l=1; l <= Dis[y][e][f][g][0]; l++)	 
						    {
							 
						         
						        i_l=Dis[y][e][f][g][l];
							 if ( i_l > -1 && L2[x][y][ind][i_l])   
							  flag1=1; 
								   	   
						    } 
						
						    if (!flag1) 
						     Dis[x][a][b][c][d]=-1;
						 
						   }
						  
						
						  
						}
			          }	 
	 	   	  
                   } 
	  
	     
	
	
	return 1;
	
	
}


int Distinguisher ( int *ActiveG, int **L1, int ****L2, int n_g,  int *****Dis )
{
	
 int x,y,a,b,c,d,e,j;
 int i_a,i_b,i_c,i_d,i_e, i_j;
 
 
   
   for (x=0; x< n_g ; x++)
   for (a=0; a< L1[x][0]; a++)
   {
     
     
     i_a=L1[x][a+1];
     if (i_a > -1)
     { 
       Dis[x][a][a][a][0]=1;
       Dis[x][a][a][a][1]=i_a;
     }
   
   }
   
    for (x=0; x< n_g ; x++)
     for (a=0; a< L1[x][0]; a++)
      for (b=0; b< L1[x][0]; b++)
      {  
	i_a=L1[x][a+1];
	i_b=L1[x][b+1]; 
	if (i_a > -1 && i_b > -1) 
	{  
	 Dis[x][a][a][b][0]=1; 
	 Dis[x][b][a][a][0]=1;  
	 Dis[x][b][a][a][1]=i_b;
	 Dis[x][a][a][b][1]=i_b;
	}
      }
 
   

	
	// first round of elimination
	for (x=0; x < n_g; x++)
	     for (a=0; a < L1[x][0]; a++)
	       for (b=0; b < L1[x][0]; b++)
		for (c=0; c < L1[x][0]; c++)
		  if (L1[x][a+1] > -1 && L1[x][b+1] > -1 && L1[x][c+1] > -1 )
	           for (y=0; y < n_g; y++)  
		    for (e =0; e < L1[y][0]; e++)
		      if (L1[y][e+1] > -1) 
		      {
		      
			i_a=L1[x][a+1]; i_b=L1[x][b+1]; i_c=L1[x][c+1];
			i_e=L1[y][e+1];
			
			for ( d=1; d<=Dis[x][a][b][c][0]; d++)
			 {
			   i_d=Dis[x][a][b][c][d]; 
			   if (i_d > -1 ) 
			    if (L2[x][y][i_a][i_e] && L2[x][y][i_b][i_e] && L2[x][y][i_c][i_e] && !L2[x][y][i_d][i_e])
			      Dis[x][a][b][c][d]=-1;
			 }
			
		      }
	
	// second round of elimination 
	
	for (x=0; x < n_g; x++)
	     for (a=0; a < L1[x][0]; a++)
	       for (b=0; b < L1[x][0]; b++)
		 for (c=0; c < L1[x][0]; c++)
		  if (L1[x][a+1] > -1 && L1[x][b+1] > -1 && L1[x][c+1] > -1 )
	           for (y=0; y < n_g; y++)  
		    for (d =0; d < L1[y][0]; d++)
		      for (e =0; e < L1[y][0]; e++)
		      if (L1[y][e+1] > -1 && L1[y][d+1] > -1) 
		      {
		      
			i_a=L1[x][a+1]; i_b=L1[x][b+1]; i_c=L1[x][c+1];
			i_d=L1[y][d+1]; i_e=L1[y][e+1];
			
			for ( j=1; j <= Dis[x][a][b][c][0]; j++)
			{
			  i_j=Dis[x][a][b][c][j];
			  if (i_j > -1) 
			  {  
			    if (L2[x][y][i_a][i_d] && L2[x][y][i_b][i_d] && L2[x][y][i_c][i_e] && !L2[x][y][i_j][i_e])
			      Dis[x][a][b][c][j]=-1;
			     
			    if (L2[x][y][i_a][i_d] && L2[x][y][i_b][i_e] && L2[x][y][i_c][i_e] && !L2[x][y][i_j][i_d])
			      Dis[x][a][b][c][j]=-1;
			  }     
			    
			}
		   }  
			  
		   j=Arc_Consistency_Dis( &ActiveG[0], L1, L2, n_g,  Dis );
  return 1;

}		  	
	
	//Print_Distinguisher(&ActiveG[0],L1,L2,n_g,Dis);
	//scanf("%d",&a);

int Make_Choice_Distinguisher (int *ActiveG, int **L1, int ****L2, int n_g,  int *****Dis )
{	
	
 int x,y,a,b,c,d,e,j;
 int i_a,i_b,i_c,i_d;
 //int make_choice; 
 for (x=0; x < n_g; x++) 
 for (a=0; a < L1[x][0]; a++)
  for (b=0; b < L1[x][0]; b++)
   for (c=0; c < L1[x][0]; c++)
	{ 
	   i_a=L1[x][a+1];  i_b=L1[x][b+1]; i_c=L1[x][c+1];
	   
	   if (L1[x][a+1] > -1 && L1[x][b+1] > -1 && L1[x][c+1] > -1 )
 			{
			//flag1=0;
			for (d=1; d<= Dis[x][a][b][c][0]; d++)
				if(Dis[x][a][b][c][d] > -1) 
			     {
					 //flag1++;
					 if (i_a== Dis[x][a][b][c][d]) {
						// make_choice=i_a;
						 Dis[x][a][b][c][0]=1; 
						 Dis[x][a][b][c][1]=i_a;
						  j=Arc_Consistency_Dis( &ActiveG[0], L1, L2, n_g,  Dis );
						 
					 }
					 
					 if (i_c== Dis[x][a][b][c][d]) {
						// make_choice=i_a;
						 Dis[x][a][b][c][0]=1; 
						 Dis[x][a][b][c][1]=i_c;
						  j=Arc_Consistency_Dis( &ActiveG[0], L1, L2, n_g,  Dis );
						 
					 } 
				     
				  
					 	 
				 }
			
			
 		  	}
		
	}
	
		return 1;
}
	
int check_Maltsev(int *ActiveG, int **L1, int ****L2, int n_g,  int *****Dis)
{
		
  int x,y,a,b,c,d,e,f;
  int i_a,i_b,i_c,i_d,i_e,i_f;
  int p,q;
  int flag1,flag2;
  
  for (x=0; x < n_g; x++) 
     for (a=0; a < L1[x][0]; a++)
	  for (b=0; b < L1[x][0]; b++)
	   for (c=0; c < L1[x][0]; c++)
		   if (L1[x][a+1] > -1 && L1[x][b+1] > -1 && L1[x][c+1] > -1 )
  			{
				flag1=0;
				for (p=1; p<= Dis[x][a][b][c][0]; p++)
					if(Dis[x][a][b][c][p] > -1) 
						flag1++;
				
				if( ! flag1) 	
				  return 0; 
				if ( flag1 > 1) 
				  return flag1;
				
  		  	}
			
  			
  
  for (x=0; x < n_g; x++) 
   for (y=0; y < n_g; y++)
	if (G[x][y] && ActiveG[x] && ActiveG[y])
     for (a=0; a < L1[x][0]; a++)
	  for (b=0; b < L1[x][0]; b++)
	   for (c=0; c < L1[x][0]; c++)
	     if (L1[x][a+1] > -1 && L1[x][b+1] > -1 && L1[x][c+1] > -1 )
 		 
		   for (d=0; d< L1[y][0]; d++)
			   for (e=0; e< L1[y][0]; e++)
				   for (f=0; f< L1[y][0]; f++)
					  if (L1[y][d+1] > -1 && L1[y][e+1] > -1 && L1[y][f+1] > -1 )
					   {
						  i_a=L1[x][a+1]; i_b=L1[x][b+1]; i_c=L1[x][c+1];
						  i_d=L1[y][d+1]; i_e=L1[y][e+1]; i_f=L1[y][f+1];
						  
						 if (L2[x][y][i_a][i_d] && L2[x][y][i_b][i_e] && L2[x][y][i_c][i_f] ) 
						  
						   for (p=1; p<= Dis[x][a][b][c][0]; p++)
						   {
							  //flag1=0; 
							  if(Dis[x][a][b][c][p] > -1) 
							  {
								  //flag1=1;
								  for (q=1; q <= Dis[y][d][e][f][0]; q++)
								  {
									 // flag2=0; 
									  if(Dis[y][d][e][f][q] > -1 ) 
									  {
										  //flag2=1;
										  if ( ! H[Dis[x][a][b][c][p]][Dis[y][d][e][f][q]])
											  return 0; 
									  }
		 							 //if (! flag2)
		 							//	 return 0  
								  }
							  }	
							 //if (! flag1)
								// return 0; 	  
						  }	  
						
					 } 
		  	 	
		
	for ( x=0; x < n_g; x++)
                  for (a=0; a< L1[x][0]; a++)
		    for (b=0; b < L1[x][0]; b++)
			for (c=0; c < L1[x][0]; c++)
      	
			  if ( L1[x][a+1] > -1 && L1[x][b+1] > -1 && L1[x][c+1] > -1 )
			    {
			     printf("Dis[%d][%d][%d][%d] : ",x,L1[x][a+1],L1[x][b+1],L1[x][c+1]);
			     for (d=1; d<= Dis[x][a][b][c][0] ; d++) 
			      if ( Dis[x][a][b][c][d] > -1 )
				   printf("%d ",Dis[x][a][b][c][d]);
      
			    printf("\n");
			 }	
  
		
		return 1;
		
}	

int main()
{
 

   
    int n_g,n_h;
	int current=1;
	
	G=GraphConstruct(&n_g, "graph-G.txt");
	
	H=GraphConstruct(&n_h,"graph-H.txt");
	
	
	int **L1;
	int ****L2;
	int *****Dis; 
	
	L1=Initial_L1_lists(n_g,n_h,1);
	L2=Initial_L2_lists(L1,n_g,n_h);
	
 	int flag;
	int Active[n_g];
	int i; 
	for (i=0; i< n_g; i++)
		Active[i]=1;
		
	int x,y,a,b,c,d;
	PreProcessing( &Active[0],L1,L2,n_g);
    
	//Print_Instance ( &Active[0], L1, n_g);
	
	//scanf("%d",&i);
	
 /*	for (x=0; x< n_g ; x++) 
	 for (y=0; y< n_g ; y++) 
	    { //printf("L(%d )=",x);
		   printf("L(%d,%d)=",x,y);
		   for (a=0; a< n_h; a++) 
			for (b=0; b< n_h; b++) 
				if ( L2[x][y][a][b])
					printf("{%d,%d},",a,b);
		printf("\n");
	   }  
	
	scanf("%d",&a); */
	
	
      	 
	 
		Dis= Initial_Distinguisher (L1,L2,n_g,n_h);
	        
		
		
		Distinguisher(&Active[0],L1,L2,n_g,Dis);
		
		//Print_Distinguisher(&Active[0],L1,L2,n_g,n_h,Dis);
		Make_Choice_Distinguisher(&Active[0],L1,L2,n_g,Dis);
		
		flag=check_Maltsev(&Active[0],L1,L2,n_g,Dis);

	    if (flag==0) 
			printf("there is no Maltsev \n");
		if (flag > 1 )
		{  
			printf("there is no unique Maltsev solution \n");
		    Print_Distinguisher(&Active[0],L1,L2,n_g,Dis);
	    }
	 	if ( flag==1) 
		  printf("there is a  Maltsev \n");
	
 return 0;
 
}