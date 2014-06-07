#include "hit.cpp"
#include <TF1.h>
#include <TGraphErrors.h>
#include <TCanvas.h>

using namespace std;

double absval(double d) {
  if (d >= 0) return (d);
  if (d < 0) return (-d);}

class triplet{
public:
  double xv[3];
  double yv[3];
  double zv[3];
  double xerr[3];
  double yerr[3];
  double zerr[3];
  TF1 *xyfitfunc;
  TGraphErrors *xygraph;
  TF1 *xzfitfunc;
  TGraphErrors *xzgraph;
  TF1 *yzfitfunc;
  TGraphErrors *yzgraph;
  double theta,phi;
  //public:
  triplet(){
    xyfitfunc = 0;
    xzfitfunc = 0;
    yzfitfunc = 0;
    xygraph = 0;
    xzgraph = 0;
    yzgraph = 0;
    for (int i = 0; i < 3; i++) xv[i] = 0;
    for (int i = 0; i < 3; i++) yv[i] = 0;
    for (int i = 0; i < 3; i++) zv[i] = 0;
    theta = 0;
    phi = 0;
    for (int i = 0; i < 3; i++) xerr[i] = 0;
    for (int i = 0; i < 3; i++) yerr[i] = 0;
    for (int i = 0; i < 3; i++) zerr[i] = 0;
  };
  double XYGetChisquare(){return (xyfitfunc->GetChisquare());};
  double XYGetParameter(int i){return (xyfitfunc->GetParameter(i));};
  double XZGetChisquare(){return (xzfitfunc->GetChisquare());};
  double XZGetParameter(int i){return (xzfitfunc->GetParameter(i));};
  double YZGetChisquare(){return (yzfitfunc->GetChisquare());};
  double YZGetParameter(int i){return (yzfitfunc->GetParameter(i));};
  double GetPhi(){
    phi = atan(xyfitfunc->GetParameter(1));
    if (yzfitfunc->GetParameter(1) > 0 && phi < 0) phi = phi + 4*atan(1.);
    if (yzfitfunc->GetParameter(1) < 0 && phi > 0) phi = phi + 4*atan(1.);
    if (yzfitfunc->GetParameter(1) < 0 && phi < 0) phi = phi + 8*atan(1.);
//Uso l'altra sezione per risolvere l'indecisione di 180° su phi
    return phi; };
  
  double GetTheta(){
    phi = GetPhi();
    //   if((xzfitfunc->GetParameter(1)*fabs(cos(phi))) != 0)
    theta = absval(atan(1/(yzfitfunc->GetParameter(1)*(sin(phi)))));
      return theta;};

void SetPoints(point x1, point x2, point x3){
  //  point::n = point::n +3;
    xv[0] = x1.x; xv[1] = x2.x; xv[2] = x3.x;
    yv[0] = x1.y ; yv[1] = x2.y; yv[2] = x3.y;
    zv[0] = x1.z; zv[1] = x2.z; zv[2] = x3.z;
    for (int i = 0; i < 3;i++) {xerr[i] = 1.44; yerr[i] = 2;zerr[i] = 0.5;}};  //incertezze di default
  
   triplet(point x1, point x2, point x3){
     xv[0] = x1.x; xv[1] = x2.x; xv[2] = x3.x;
     yv[0] = x1.y ; yv[1] = x2.y; yv[2] = x3.y;
     zv[0] = x1.z; zv[1] = x2.z; zv[2] = x3.z;
     for (int i = 0; i < 3;i++) {xerr[i] = 1.44; yerr[i] = 0.1;zerr[i] = 0.5;}};  //incertezze di default

     ~triplet() {
    delete xyfitfunc;
     delete xzfitfunc;
     delete yzfitfunc;
     delete xygraph;
     delete xzgraph;
     delete yzgraph;};
    

  //  void SetErr(double ex[3],double ey[3], double ez[3]){xerr = ex; yerr = ey;zerr = ez;};

  void XYFit(){   
    if (xerr[0] == 0 && xerr[1] == 0 && xerr[2] == 0 && yerr[0] == 0 && yerr[1] == 0 && yerr[2] == 0) {cout << "ERRORE: Incertezze non specificate" << endl;}
    else {
      //  TGraphErrors *graph1 = new TGraphErrors(3,x,y,xerr,yerr);
    //  TF1* fitfunc1 = new TF1("fittingfunction","[0]+[1]*x",-100,100);
    //   xyfitfunc = fitfunc1;
    // xygraph = graph1;
      delete xyfitfunc;  //Dealloca prima di riallocarne uno nuovo
      delete xygraph;
xyfitfunc = new TF1("xyfittingfunction", "[0]+[1]*x",-100,100);
        xygraph = new TGraphErrors(3,xv,yv,xerr,yerr);
    xyfitfunc->SetParameters(80,0.7);

    xygraph->Fit(xyfitfunc,"0QS");
    }
  };
  
  // void XYDraw(){
  //   xygraph->SetLineColor(kBlue);
  //    xyfitfunc->SetLineColor(kRed);
  // TCanvas* XYlinearfit = new TCanvas();
  // XYlinearfit->SetTitle("XY Linear Fit");
  // XYlinearfit->SetGrid();
  // xygraph->GetXaxis()->SetTitle("x [cm]");
  // xygraph->GetYaxis()->SetTitle("y [cm]");
  // xygraph->GetXaxis()->CenterTitle();
  // xygraph->GetYaxis()->CenterTitle();
  // xygraph->DrawClone("APE");
  // xyfitfunc->DrawClone("SAME");
  // };

  void XZFit(){   
    if (xerr[0] == 0 && xerr[1] == 0 && xerr[2] == 0 && zerr[0] == 0 && zerr[1] == 0 && zerr[2] == 0) {cout << "ERRORE: Incertezze non specificate" << endl;}
    else {
      //  TGraphErrors *graph1 = new TGraphErrors(3,x,y,xerr,yerr);
    //  TF1* fitfunc1 = new TF1("fittingfunction","[0]+[1]*x",-100,100);
    //   xyfitfunc = fitfunc1;
    // xygraph = graph1;
      delete xzfitfunc;  //Dealloca prima di riallocarne uno nuovo
      delete xzgraph;
xzfitfunc = new TF1("xzfittingfunction", "[0]+[1]*x",-100,100);
        xzgraph = new TGraphErrors(3,xv,yv,xerr,zerr);
    xzfitfunc->SetParameters(30,-2);
    xzgraph->Fit(xzfitfunc,"0QS");
    }
  };
  
  // void XZDraw(){
  // xzgraph->SetLineColor(kBlue);
  // xzfitfunc->SetLineColor(kRed);
  // TCanvas* XZlinearfit = new TCanvas();
  // XZlinearfit->SetTitle("XZ Linear Fit");
  // XZlinearfit->SetGrid();
  // xzgraph->GetXaxis()->SetTitle("x [cm]");
  // xzgraph->GetYaxis()->SetTitle("z [cm]");
  // xzgraph->GetXaxis()->CenterTitle();
  // xzgraph->GetYaxis()->CenterTitle();
  // xzgraph->DrawClone("APE");
  // xzfitfunc->DrawClone("SAME");
  // };

void YZFit(){   
    if (yerr[0] == 0 && yerr[1] == 0 && yerr[2] == 0 && zerr[0] == 0 && zerr[1] == 0 && zerr[2] == 0) {cout << "ERRORE: Incertezze non specificate" << endl;}
    else {
      delete yzfitfunc;
      delete yzgraph;
    yzgraph = new TGraphErrors(3,yv,zv,yerr,zerr);
    yzfitfunc = new TF1("yzfittingfunction","[0]+[1]*x",-1000,1000);
    yzfitfunc->SetParameters(300,-6);
    yzgraph->Fit(yzfitfunc,"0QS");
    }
  };
  
  // void YZDraw(){
  //   yzgraph->SetLineColor(kBlue);
  //    yzfitfunc->SetLineColor(kRed);
  // TCanvas* YZlinearfit = new TCanvas();
  // YZlinearfit->SetTitle("YZ Linear Fit");
  // YZlinearfit->SetGrid();
  // yzgraph->GetXaxis()->SetTitle("y [cm]");
  // yzgraph->GetYaxis()->SetTitle("z [cm]");
  // yzgraph->GetXaxis()->CenterTitle();
  // yzgraph->GetYaxis()->CenterTitle();
  // yzgraph->DrawClone("APE");
  // yzfitfunc->DrawClone("SAME");
  // };

  void Fit(){
    XZFit();
    XYFit();
    YZFit();};

  double GetCoordinate(int coord, int pn) {
    switch (coord){
    case(0):
      return (xv[pn]);
      break;
    case(1):
      return (yv[pn]);
      break;
    case(2):
      return (zv[pn]);
      break;
    default:
      cout << "INVALID COORDINATE NUMBER: " << coord << endl;
      return (-10000);
    }};
    
    

};
    

void fit(){
  point* p1 = new point; p1->SetValues(-50,47.52,145); 
  point* p2 = new point; p2->SetValues(-20,70.95,85);
  point* p3 = new point; p3->SetValues(10,84.96,23);

  triplet n1; n1.SetPoints(*p1,*p2,*p3);
  n1.Fit();
  TF1* f1 = new TF1("XZ;X;Z","[0]*x+[1]",-60,60);  
  TF1* f2 = new TF1("YZ;Y;Z","[0]*x+[1]",40,100);
  TF1* f3 = new TF1("XY;X;Y","[0]*x+[1]",-60,60);
  f1->SetParameters(n1.XZGetParameter(0),n1.XZGetParameter(1));
  f2->SetParameters(n1.YZGetParameter(0),n1.YZGetParameter(1));
  f3->SetParameters(n1.XYGetParameter(0),n1.XYGetParameter(1));
  TCanvas *c1 = new TCanvas("c1");
  f1->DrawCopy();
  TCanvas *c2 = new TCanvas("c2");
  f2->DrawCopy();
  TCanvas *c3 = new TCanvas("c3");
  f3->DrawCopy();
  cout << "THETA: " << n1.GetTheta() << endl;
  cout << "PHI: " << n1.GetPhi() << endl;
  cout << "XZ: " << n1.XZGetChisquare() << endl;
  cout << "YZ: " << n1.YZGetChisquare() << endl;
  cout << "XY: " << n1.XYGetChisquare() << endl;

  // double chi = n1.XYGetChisquare();
  // cout << "chi square: " << chi << endl;
  //  cout << "Theta: " << n1.GetTheta() << endl;
  // cout << "Phi: " << n1.GetPhi() << endl;
};
  

