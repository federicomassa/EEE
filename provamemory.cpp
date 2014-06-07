#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void provamemory(){
  int j = 0;
  string line;
  ifstream f1("EEE_PISA01TestRun4Telescopes_20140507_014455.txt");
  ofstream f2("provamemory.txt");
  do{  
getline(f1,line);
 j++;
 cout << "LINEA: " << j << endl;
 for (int i = 0; i < line.size(); i++) {
   f2 << line.substr(i,1) << endl;
}
  }
  while (!f1.eof());
  f1.close();
  f2.close();
  
}
