using namespace std;

class prova{
 public:
  static int n;
  prova(){n+=1;}
  ~prova(){n-=1;}
  void tuamamma(){cout << "Tua mamma" << endl;};
};

void provastatic(){
  int prova::n = 0;
  prova* p1 = new prova;
  prova p2, p3;
  prova p4[6];
  prova* p5 = new prova[4];
  cout << prova::n << endl;
  delete p1;
  cout << prova::n << endl;
  delete [] p5;
  cout << prova::n << endl;
  p5 = new prova[22];
  cout << prova::n << endl;
  delete[] p5;
  cout << prova::n << endl;
}

