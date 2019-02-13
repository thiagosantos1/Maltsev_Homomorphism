/* Authors: Arash Rafiey Thiago Santos. --> https://github.com/thiagosantos1/   */

/***************************************************************************
* This class is designed to have all the information about the class
* graphsconstruct.h, as well with more information about what each method does.
*
* The Idea of this implementation is to solve linear equations, using 
* Graph and Maltsev theory. After appling the theory, the graph and
* its list left can be used to find a solution for the linear equation
* Using Homomorphism.
* 
* Copyright (C)
*
****************************************************************************/


int odd_Even_bits(int x);

int which_neighbor(GRAPHS *op, int x, int y);

int which_bit(int l, int i );

void create_list_G2H(GRAPHS *op, USER_PARAMS * ip);

void save_list_G2H(GRAPHS *op, USER_PARAMS * ip);

