#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

void provacomp(){
  string line = "";
  fstream run("disperazione.txt");
  getline(run,line);
  cout << line << endl;
  getline(run,line);
  cout << line << endl;
  run.close();
}
