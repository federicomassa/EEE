#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>

using namespace std;

void clear(){
  const char fname[] = "EEE_PISA01TestRun4Telescopes_20140507_014455.txt";
  string line;
  ifstream f1(fname);
  ofstream f2("cleared.txt");
  while (!f1.eof()) {
    getline(f1,line);
    if (line != "\r") f2 << line << endl;
  };

  remove(fname);
  rename("cleared.txt",fname);
  f1.close();
  f2.close();
}
      
