/* Authors: Arash Rafiey Thiago Santos. --> https://github.com/thiagosantos1/   */

/***************************************************************************
* This class is designed to have all the information about the class
* pre-processing, as well with more information about what each method does.
* The idea is to run arc and pair consistency between graphs G & H.
*
* The Idea of this implementation is to solve linear equations, using 
* Graph and Maltsev theory. After appling the theory, the graph and
* its list left can be used to find a solution for the linear equation
* Using Homomorphism.
* 
* Copyright (C)
*
****************************************************************************/


int pre_process(GRAPHS *op);
void arc_consistency(GRAPHS *op);
int pair_consistency(GRAPHS *op);

