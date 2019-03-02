/* Authors: Arash Rafiey Thiago Santos. --> https://github.com/thiagosantos1/   */

/***************************************************************************
*
* Main class to set all parameters to our LP
* As well to set all user parameters to load all files needed 
* to make our graph and its aditional informations
* 
* Copyright (C)
* 
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <graphs.h> 
#include <maltsev.h>

int main(int argc, char const *argv[])
{ 
  USER_PARAMS userpar;
  GRAPHS graphs;

  int create_new_H = 0;


  // Default configuration
  userpar.graph_g = "../etc/graph_G.txt";
  userpar.graph_h = "../etc/graph_H.txt";
  userpar.list_homom = "../etc/list_file.txt";


  // you can set manually create_new_H = 1
  if(create_new_H >0){
    create_list_G2H(&graphs, &userpar );
    init_graphs_lists(&graphs, &userpar, 0 );
  }
  else
    init_graphs_lists(&graphs, &userpar, 1 ); // reads graphs H and G and its list. Also, creates pairs list


  int out_maltsev = run_maltsev(&graphs);

  if (out_maltsev==0) 
    printf("there is no Maltsev \n");
  else if (out_maltsev > 1 ){  
    printf("there is no unique Maltsev solution \n");
    print_distinguisher(&graphs);
  }else if ( out_maltsev==1) 
    printf("there is a  Maltsev \n");

  return 0;

}


