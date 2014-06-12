#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <TH1F.h>
#include <TFile.h>
#include <TRandom3.h>

//lati lunghi = 48.5 cm, lati corti = 40 cm, spessore = 1.2 cm, distanze relative 10,2 cm
double xmax = 82; //cm
double ymax = 158; //cm
double D12 = 60;// cm
double D23 = 62; //cm
int imax = 200000;

using namespace std;


void accettanza(){
	TH1F* htheta = new TH1F("dis_acctheta","Distribuzione Theta accettati", 50, 0, 90);
	TH1F* hphi = new TH1F("dis_accphi", "Distribuzione Phi accettati", 90, 0, 360);
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
  double theta, W1,W2,W3, phi,x1,x2,x3,y1,y2,y3,z1,z2,z3;
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

	*acctheta << theta << endl;
	*accphi << phi << endl;
	htheta->Fill(theta*180/3.14159);
	hphi->Fill(phi*180/3.14159);
	j+=1;}
    }
  }
  htheta->Scale(36186.0/double(j));
  hphi->Scale(36186.0/double(j));
  
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
