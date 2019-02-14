/* Authors: Arash Rafiey Thiago Santos. --> https://github.com/thiagosantos1/   */

/***************************************************************************
* This class is designed to have all the information about the class
* maltsev.h, as well with more information about what each method does.
* This is the main class, with all methods for our maltsev algorithm
*
* The Idea of this implementation is to solve linear equations, using 
* Graph and Maltsev theory. After appling the theory, the graph and
* its list left can be used to find a solution for the linear equation
* Using Homomorphism.
* 
* Copyright (C)
*
****************************************************************************/

// main method. This is where we call all other methods, to solve our problem
// This is the only available method to main
int run_maltsev(GRAPHS *op);

int pre_processing(GRAPHS *op);

int is_rectangle(GRAPHS *op, int x, int a, int b);

void print_instance(GRAPHS *op, int x, int a, int current);

int Pruning_the_lists(GRAPHS *op);

void print_pairs(GRAPHS *op);