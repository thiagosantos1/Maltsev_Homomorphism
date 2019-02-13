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


  // Default configuration
  userpar.graph_g = "../etc/graph_G.txt";
  userpar.graph_h = "../etc/graph_H.txt";
  userpar.list_homom = "../etc/list_file.txt";

  read_graphs(&graphs, &userpar );

  return 0;

}