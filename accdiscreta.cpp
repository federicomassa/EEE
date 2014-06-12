#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <TH1F.h>
#include <TFile.h>
#include <TRandom3.h>
#include <TH2F.h>
#include "fit.cpp"

//lati lunghi = 48.5 cm, lati corti = 40 cm, spessore = 1.2 cm, distanze relative 10,2 cm (scintillatori per flusso raggi cosmici)
double xmax = 82; //cm
double ymax = 158; //cm
double D12 = 60;// cm
double D23 = 62; //cm
int imax = 200000;
double strd = 82.0/24.0; //unità arbitrarie, distanza tra due strip considerate filiformi ---- CORRETTA -----
double stheta;
double sphi;
using namespace std;


double xstrip(double xx){ // returns the x-coordinate of the strip (actually its region of influence) the generated point falls in
  int strnum;
  double xs;
  if (xx >= 0) {strnum=int(xx/strd); xs = double(strnum)*strd+strd/2;}
  else {strnum=int((xx)/strd); xs = double(strnum)*strd-strd/2;}
  return xs;
}

void accdiscreta(){
  double chi = 0;
  point p1,p2,p3;
triplet n1;
	TH1F* htheta = new TH1F("dis_acctheta","Distribuzione Theta accettati", 50, 0, 90);
	// TH1F* hrestheta = new TH1F("dis_restheta","Distribuzione Theta-Theta_quantizzato accettati", 50, 0, 90);
// TH1F* hrestheta = new TH1F("dis_restheta","Distribuzione Theta-Theta_quantizzato accettati", 50, 0, 90);
	TH1F* hphi = new TH1F("dis_accphi", "Distribuzione Phi accettati", 90, 0, 360);
	TH1F* distchi = new TH1F("dis_chi", "Distribuzione Chi2", 50, 0, 10);
	TH2F* phitheta = new TH2F("phitheta","Phi-Theta Correlation;phi(°);theta(°)",90,0,360,50,0,90);
	TFile rfile("accettanza.root","RECREATE");
	TRandom3 rndgen;
  ofstream* dtheta = new ofstream("theta.dat");
  ofstream* acctheta = new ofstream("acctheta.dat");
  ofstream* accphi = new ofstream("accphi.dat");
  //efficienze
  //  float eps1 [3][3] = {{0.612,0.662,0.762},{0.425,0.470,0.612},{0.574,0.618,0.722}};
  float eps1 = 1;
  float eps2 = 1;
  float eps3 = 1;
  double theta, W1,W2,W3, phi,x1,x2,x3,y1,y2,y3,z1,z2 = 0,z3,xs1,xs2,xs3;
  int j = 0;

  for (int i = 1; i <= imax;) {
    theta = rndgen.Uniform(2*atan(1.)); 
    W1 = rndgen.Uniform(1);
    if (sin(theta)*pow(cos(theta),2) > W1) {
      *dtheta << theta << endl;
      i+=1;
      if (i%5000 == 0) cout << double(i)/double(imax)*100 << "%" << endl;
      phi = rndgen.Uniform(8*atan(1.));
      x2 = (rndgen.Uniform(xmax)-xmax/2.);
      y2 = (rndgen.Uniform(ymax)-ymax/2.);
      x1 = tan(theta)*cos(phi)*D12 + x2;
      y1 = tan(theta)*sin(phi)*D12 + y2;
      z1 = D12;
      x3 = -tan(theta)*cos(phi)*D23+x2;
      y3 = -tan(theta)*sin(phi)*D23 + y2;
      z3 = -D23;
      W1 = rndgen.Uniform(1);
      W2 = rndgen.Uniform(1);
      W3 = rndgen.Uniform(1);
      
      if ((pow(x1,2) <= pow(xmax,2)/4) && (pow(y1,2) <= pow(ymax,2)/4) && (pow(x3,2) <= pow(xmax,2)/4) && (pow(y3,2) <= pow(ymax,2)/4) && W1 < eps1 && W2 < eps2 && W3 < eps3){

	xs1=xstrip(x1);
	xs2=xstrip(x2);
	xs3=xstrip(x3);
	


	p1.SetValues(xs1,y1,z1);
	p2.SetValues(xs2,y2,z2);
	p3.SetValues(xs3,y3,z3);

	
	n1.SetPoints(p1,p2,p3);
	
	n1.Fit();

	theta=n1.GetTheta();

	phi=n1.GetPhi();
	chi = n1.XYGetChisquare();
	chi += n1.XZGetChisquare();
	chi += n1.YZGetChisquare();
	chi = chi/3.0;

	if (chi == 0) {
	  cout << "p1 x: " << p1.x << " p1 y: " << p1.y << " p1 z: " << p1.z << endl;
	  cout << "p2 x: " << p2.x << " p2 y: " << p2.y << " p2 z: " << p2.z << endl;
	  cout << "p3 x: " << p3.x << " p3 y: " << p3.y << " p3 z: " << p3.z << endl;
	  cout << '\n';
	  cin.get();
	}
	  
	  

	*acctheta << theta << endl;
	*accphi << phi << endl;
	htheta->Fill(theta*180/3.14159);
	hphi->Fill(phi*180/3.14159);
	phitheta->Fill(phi*180/3.14159,theta*180/3.14159);
	  distchi->Fill(chi);
	j+=1;}
    }
  }
  distchi->Write();

  htheta->Scale(30751.0/double(j));
  hphi->Scale(30751.0/double(j));

  phitheta->Write();
  htheta->Write();
  hphi->Write();
  rfile.Close();
  dtheta->close();
  acctheta->close();
  accphi->close();
  cout << "Generati: " << imax << endl;
  cout << "Accettati: " << j << endl;
  cout << "Accettanza: " << double(j)/double(imax) << endl;
  }
