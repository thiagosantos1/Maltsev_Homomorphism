## Maltsev Homomorphism

The Idea of this implementation is to solve linear equations, using Graph and Maltsev theory. 
After appling the theory, the graph and its list left can be used to find a solution for the linear equation Using Homomorphism.

# Functionality of each Class
  * main
    * As the name says, its the main class.
    * It's where we define names of files to be used and its path.
    * Also, it's where you define if you want to run for a saved graph or if you want to create a new instance of H.
  
  * graphs.h
    * Have all data structure needed for our solution. 
    * Includes all main methods, which is visiable for main/user.
  
  * readgraphs
    * This class is used to read all graphs G & H, as well to initialize the list of G and the pair list.
    * You also have available methods to print both Graphs as well the list and pairs.
    
  * graphsconstruct.h
    * This main goal of this class it to create a new instance of graph H, as well the new list for graph G
    * Thus, all methods necessary to create this new instance is saved in here.
    
  * maltsev.h
    * This is the main class. Once you have all graphs, list & pairs loaded, we can use this class and its methods to run our maltsev algorithm.
    * Here is where we do the pre-processing of the graph, reducing the list of each Vertex by doing pair consistency.

# How to use the code
  There's a Makefile in the folder src, which helps the process.
  
  ** How to compile, if needed
  inside of the folder src, you can run the command "make" in your terminal
  
  ** How to run the code
  Considering that Graphs G & H and the list is saved in the folder etc, you can simply run by typing "make run" into your terminal
  
