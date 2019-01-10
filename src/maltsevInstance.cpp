// Name        : NewGraphRead.cpp

// Author      :

// Version     :

// Copyright   : Your copyright notice

// Description : Hello World in C++, Ansi-style

//============================================================================

#include <iostream>

#include <vector>

using namespace std;

class MaltsevInstance
{

  public:
    int n_g; // number of vertices in G

    vector<vector<int> > G;

    vector<vector<int> > L1;

    int**** L2;

    int* lable;

    // each vertex x of G has a label which shows it was a vertex label(x) is some other graph

    MaltsevInstance(char* file_name); // read the graph from file

    int Initial_L1_lists(char* file_name); // initialize the lists L1 from a file;

    int Initial_L_2_lists();

    int PreProcessing(); // it uses G,L1,L2

    // MaltsevInstance();   // create based on an input instance
};

int main()
{

  // example of a 2D vector
  vector<vector<int> > G;

  int i, j;

  for(i = 0; i < 3; i++){

    vector<int> temp1;

    for (j = 0; j < 3; j++)
      temp1.push_back(j);

    G.push_back(temp1);
  }

  for (i = 0; i < G.size(); i++){

    for (j = 0; j < G[i].size(); j++)
      cout << G[i][j] << " ";

    cout << endl;
  }

  return 0;

  // MaltsevInstance graph1;
}



