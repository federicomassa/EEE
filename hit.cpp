#include <iostream>

using namespace std;

//Coordinate z delle 3 camere
double zch1 = 145;
double zch2 = 85;
double zch3 = 23;

class point {
public:
  // static int n;
  point(){x = 0; y = 0; z = 0;/*n++;*/};
  //  ~point(){n--;};
  double x,y,z;
  void SetValues(double xc, double yc, double zc) {x = xc; y = yc; z = zc;};
  void SetValue(int i, double v) {
    switch(i){
    case(0):
      x = v;
      break;
    case(1):
      y = v;
      break;
    case(2):
      z = v;
      break;
    default:
      cout << "ERROR: invalid coordinate number in SetValue function" << endl;}};
  int GetChNumber(){
    if (z == zch1) return (1);
    else if (z == zch2) return (2);
    else if (z == zch3) return (3);
    else {
      cout << "ERROR: invalid z coordinate in GetChNumber function: z = " << z <<  endl;
      return (0);}

  };
};
