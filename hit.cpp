#include <iostream>

using namespace std;

//Coordinate z delle 3 camere
double zch1 = 53.2;
double zch2 = 0.0;
double zch3 = -52.8;

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
    if (z == zch1 || z == 145.0) return (1);
    else if (z == zch2 || z == 85.0) return (2);
    else if (z == zch3 || z == 23.0) return (3);
    else {
      cout << "ERROR: invalid z coordinate in GetChNumber function: z = " << z <<  endl;
      return (0);}

  };
};

double XYGetIntercept(point a, point b) { //se verticale restituisce -5000
  if (a.x != b.x)
    return (a.y - (b.y-a.y)/(b.x-a.x)*a.x);
  else
    return (-5000);
      }

double XYGetSlope(point a, point b) { //se verticale restituisce -5000
  if (a.x != b.x)
    return (b.y-a.y)/(b.x-a.x);
  else
    return (-5000);
      }

double XZGetIntercept(point a, point b) { //se verticale restituisce -5000
  if (a.x != b.x)
    return (a.z - (b.z-a.z)/(b.x-a.x)*a.x);
  else
    return (-5000);
      }

double XZGetSlope(point a, point b) { //se verticale restituisce -5000
  if (a.x != b.x)
    return (b.z-a.z)/(b.x-a.x);
  else
    return (-5000);
      }

double YZGetIntercept(point a, point b) { //se verticale restituisce -5000
  if (a.y != b.y)
    return (a.z - (b.z-a.z)/(b.y-a.y)*a.y);
  else
    return (-5000);
      }

double YZGetSlope(point a, point b) { //se verticale restituisce -5000
  if (a.y != b.y)
    return (b.z-a.z)/(b.y-a.y);
  else
    return (-5000);
      }
