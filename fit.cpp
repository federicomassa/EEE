#include "hit.cpp"
#include <TF1.h>
#include <TGraphErrors.h>
#include <TCanvas.h>

using namespace std;

double absval(double d) {
  if (d >= 0) return (d);
  else return (-d);}

double sqrt(double a) {return pow(a,0.5);}
double sqr(double a) {return pow(a,2);}

class triplet{
public:

  //calcolo "manuale" del chi2 per i fit che danno particolari problemi (picco in theta = 0..)
  double XYGetChisquare_m() {
    XYFit();
    
    double eqerr[3];
    double result = 0;
    // somma in quadratura: attenzione: viene usata la pendenza tra due punti per il calcolo di eqerr
    for (unsigned n = 0; n < 3; n++) eqerr[n] = sqrt(sqr(yerr[n]) + sqr(XYGetSlope()*xerr[n]));
    for (unsigned n = 0; n < 3; n++) result += sqr((yv[n]- (XYGetParameter(0) + XYGetParameter(1)*xv[n]))/eqerr[n]);
    return result; 

  }

  double XZGetChisquare_m() {
    XZFit();
    double eqerr[3];
    double result = 0;
    // somma in quadratura: attenzione: viene usato XYGetParameter(1) per il calcolo di eqerr, possibile pericolo
    for (unsigned n = 0; n < 3; n++) eqerr[n] = sqrt(sqr(zerr[n]) + sqr(XZGetSlope()*xerr[n]));
    for (unsigned n = 0; n < 3; n++) result += sqr((zv[n]- (XZGetParameter(0) + XZGetParameter(1)*xv[n]))/eqerr[n]);
    return result;
  }

  double YZGetChisquare_m() {
    YZFit();
    double eqerr[3];
    double result = 0;
    // somma in quadratura
    for (unsigned n = 0; n < 3; n++) eqerr[n] = sqrt(sqr(zerr[n]) + sqr(YZGetSlope()*yerr[n]));
    for (unsigned n = 0; n < 3; n++) result += sqr((zv[n]- (YZGetParameter(0) + YZGetParameter(1)*yv[n]))/eqerr[n]);
    return result;
  }


  //Calcolo intercetta e pendenza sui primi due punti per migliore inizializzazione dei parametri del fit
 double XYGetIntercept() {
    return (yv[0]-(yv[1]-yv[0])/(xv[1]-xv[0])*xv[0]) ;}
 double XZGetIntercept() {
     return (zv[0]-(zv[1]-zv[0])/(xv[1]-xv[0])*xv[0]) ;}
 double YZGetIntercept() {
   return (zv[0]-(zv[1]-zv[0])/(yv[1]-yv[0])*yv[0]) ;}

 double XYGetSlope() {
    return (yv[1]-yv[0])/(xv[1]-xv[0]) ;}
 double XZGetSlope() {
    return (zv[1]-zv[0])/(xv[1]-xv[0]) ;}
 double YZGetSlope() {
    return (zv[1]-zv[0])/(yv[1]-yv[0]) ;}

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
  bool vert;
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
    if (!vert){
    phi = atan(xyfitfunc->GetParameter(1));
    if (yzfitfunc->GetParameter(1) > 0 && phi < 0) phi = phi + 4*atan(1.);
    if (yzfitfunc->GetParameter(1) < 0 && phi > 0) phi = phi + 4*atan(1.);
    if (yzfitfunc->GetParameter(1) < 0 && phi < 0) phi = phi + 8*atan(1.);
//Uso l'altra sezione per risolvere l'indecisione di 180° su phi
    return phi;}
    else {
      if (YZGetParameter(1) > 0) return 2*atan(1);
      else {return 6*atan(1);} 
    
    }
  }
  
 
  
  double GetTheta(){
    phi = GetPhi();
    theta = absval(atan(1/(yzfitfunc->GetParameter(1)*(sin(phi)))));
    return theta;};

void SetPoints(point x1, point x2, point x3){
    xv[0] = x1.x; xv[1] = x2.x; xv[2] = x3.x;
    yv[0] = x1.y ; yv[1] = x2.y; yv[2] = x3.y;
    zv[0] = x1.z; zv[1] = x2.z; zv[2] = x3.z;
    //controllo di verticalità
    if (xv[0] == xv[1] && xv[0] == xv[2]) vert = true; else vert = false;
    for (int i = 0; i < 3;i++) {xerr[i] = 1.44; yerr[i] = 2;zerr[i] = 0.5;}};  //incertezze di default
  
   triplet(point x1, point x2, point x3){
     xv[0] = x1.x; xv[1] = x2.x; xv[2] = x3.x;
     yv[0] = x1.y ; yv[1] = x2.y; yv[2] = x3.y;
     zv[0] = x1.z; zv[1] = x2.z; zv[2] = x3.z;
     for (int i = 0; i < 3;i++) {xerr[i] = 1.44; yerr[i] = 2;zerr[i] = 0.5;}};  //incertezze di default

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
	xyfitfunc->SetParameters(XYGetIntercept(), XYGetSlope());
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
        xzgraph = new TGraphErrors(3,xv,zv,xerr,zerr);
  	xzfitfunc->SetParameters(XZGetIntercept(), XZGetSlope());
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
    yzfitfunc->SetParameters(YZGetIntercept(), YZGetSlope());
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
  // point* p1 = new point; p1->SetValues(5,-1.44,145); 
  // point* p2 = new point; p2->SetValues(-5,-5.37,85);
  // point* p3 = new point; p3->SetValues(-20,0,23);

  point* p1 = new point; p1->SetValues(-30.00,138.24,145.00); 
  point* p2 = new point; p2->SetValues(-30.00,92.55,85.00);
  point* p3 = new point; p3->SetValues(-30,66.24,23.00);
  triplet n1; n1.SetPoints(*p1,*p2,*p3);
  n1.XYFit();
  n1.YZFit();
  
  // TGraphErrors* g1 = new TGraphErrors(3,n1.yv,n1.zv,n1.yerr,n1.zerr);
  // cout << "intercetta: " << n1.YZGetIntercept();
  // cout << "pendenza: " << n1.YZGetSlope();
  // TF1* f1 = new TF1("lin","[0]+[1]*x",-100,100);
  // f1->SetParameters(n1.YZGetIntercept(),n1.YZGetSlope());
  // g1->Fit(f1,"0");
  // g1->Draw("APE");
  // f1->DrawCopy("same");
  // cout << "XY 0: " << n1.XYGetParameter(0) << endl;
  // cout << "XY 1: " << n1.XYGetParameter(1) << endl;
  // cout << "THETA: " << n1.GetTheta() << endl;
  // cout << "PHI: " << n1.GetPhi() << endl;
  // cout << "XZ: " << n1.XZGetChisquare() << endl;
  // cout << "YZ: " << n1.YZGetChisquare() << endl;
  // cout << "XY: " << n1.XYGetChisquare() << endl;

  cout << '\n' << "MANUALE" << '\n' << endl;

  cout << "yz: " << n1.YZGetChisquare() << endl;
  // cout << "xz: " << n1.XZGetChisquare_m() << endl;
  cout << "xy: " << n1.XYGetChisquare_m() << endl;
  cout << "theta: " << n1.GetTheta() << endl;
  cout << "phi: " << n1.GetPhi() << endl;
  // double chi = n1.XYGetChisquare();
  // cout << "chi square: " << chi << endl;
  //  cout << "Theta: " << n1.GetTheta() << endl;
  // cout << "Phi: " << n1.GetPhi() << endl;
};
  

