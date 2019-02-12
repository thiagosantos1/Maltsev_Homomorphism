/* Authors: Arash Rafiey Thiago Santos. --> https://github.com/thiagosantos1/   */

/***************************************************************************
* This class is designed to have all the information about the class
* readgraphs.c, as well with more information about what each method does.
* 
* The Idea of this implementation is to solve linear equations, using 
* Graph and Maltsev theory. After appling the theory, the graph and
* its list left can be used to find a solution for the linear equation
* Using Homomorphism.
* 
* Copyright (C)
*
****************************************************************************/

/* Based on the file given from user, this method initialize the list homomphis 
   which is all possible assigments in H, for each vertex in G*/
void init_list(GRAPHS *op, char * file_name);

/* Initialize graph */
void init_graph_g(GRAPHS *op, char * file_name);

/* Initialize graph */
void init_graph_h(GRAPHS *op, char * file_name);

