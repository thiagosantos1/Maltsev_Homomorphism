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

#include <graphs.h> 

int main(int argc, char const *argv[])
{ 
  USER_PARAMS userpar;
  GRAPHS graphs;

  int create_new_H = 0;


  // Default configuration
  userpar.graph_g = "../etc/graph_G.txt";
  userpar.graph_h = "../etc/graph_H.txt";
  userpar.list_homom = "../etc/list_file.txt";


  // you can set manually create_new_H = 1 or pass anything as argv to be as a flag indicating create new H.
  if(create_new_H >0 || argc > 2){
    create_list_G2H(&graphs, &userpar );
    init_graphs_lists(&graphs, &userpar, 0 );
  }
  else
    init_graphs_lists(&graphs, &userpar, 1 ); // reads graphs H and G and its list. Also, creates pairs list


  int out_put = run_maltsev(&graphs);

  printf("the output is %d \n", out_put); 

  return 0;

}


