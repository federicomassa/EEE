#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <TH1F.h>
#include <TFile.h>
#include <TRandom3.h>

//lati lunghi = 48.5 cm, lati corti = 40 cm, spessore = 1.2 cm, distanze relative 10,2 cm
double L1L= 100; //cm
double L1S = 200; //cm
double L2L= 100; //cm
double L2S= 200; // cm
double L3L= 100; //cm
double L3S = 200; //cm
double h = 2.5; //cm
double D12 = 60;// cm
double D23 = 60; //cm
int imax = 60000;

using namespace std;

void cell(int n, double x, double y, int& xn, int& yn){
  double LS, LL;
  switch (n) {
  case 1:
    LS = L1S;
    LL = L1L;
    break;
  case 2:
    LS = L2S;
    LL = L2L;
    break;
  case 3:
    LS = L3S;
    LL = L3L;
    break;
  default:
    cout << "Numero piano non valido" << endl;
  }
  xn = ceil(3*(x+LL/2.)/LL);
  yn = ceil(3*(y+LS/2.)/LS);
  if (xn > 3 || xn < 1) cout << "Fuori scala x" << endl;
  else if (yn > 3 || yn < 1) cout << "Fuori scala y" << endl;
  
}

void accettanza(){
	TH1F* htheta = new TH1F("dis_acctheta","Distribuzione Theta accettati", 50, 0, 2*atan(1.));
	TH1F* hphi = new TH1F("dis_accphi", "Distribuzione Phi accettati", 100, 0, 8*atan(1.));
	TFile rfile("accettanza.root","RECREATE");
	TRandom3 rndgen;
  ofstream* dtheta = new ofstream("theta.dat");
  ofstream* acctheta = new ofstream("acctheta.dat");
  ofstream* accphi = new ofstream("accphi.dat");
  //efficienze
  //  float eps1 [3][3] = {{0.612,0.662,0.762},{0.425,0.470,0.612},{0.574,0.618,0.722}};
  float eps1 [3][3] = {{1,1,1},{1,1,1},{1,1,1}};
  float eps2 [3][3] = {{1,1,1},{1,1,1},{1,1,1}};
  float eps3 [3][3] = {{1,1,1},{1,1,1},{1,1,1}};
  double theta, W,Y,Z, phi,x1,x2,x3,y1,y2,y3,z1,z2,z3;
  int j = 0;
  int nx1, nx2, nx3, ny1, ny2, ny3;

  for (int i = 1; i <= imax;) {
    theta = rndgen.Uniform(2*atan(1.)); 
    W = rndgen.Uniform(1);
    if (sin(theta)*pow(cos(theta),2) > W) {
      *dtheta << theta << endl;
      i+=1;
      phi = rndgen.Uniform(8*atan(1.));
      x2 = (rndgen.Uniform(L1L)-L1L/2.);
      y2 = (rndgen.Uniform(L1S)-L1S/2.);
      x1 = tan(theta)*cos(phi)*D12 + x2;
      y1 = tan(theta)*sin(phi)*D12 + y2;
      z1 = D12;
      x3 = -tan(theta)*cos(phi)*D23+x2;
      y3 = -tan(theta)*sin(phi)*D23 + y2;
      z3 = -D23;
      if ((pow(x1,2) <= pow(L1L,2)/4) && (pow(y1,2) <= pow(L1S,2)/4) && (pow(x3,2) <= pow(L3L,2)/4) && (pow(y3,2) <= pow(L3S,2)/4)){
	cell (1,x1,y1,nx1,ny1);
	cell (2,x2,y2,nx2,ny2);
	cell (3,x3,y3,nx3,ny3);
	W = rndgen.Uniform(1);
	Y = rndgen.Uniform(1);
	Z = rndgen.Uniform(1);
	//	if (W < eps1[nx1-1][ny1-1] && Y < eps2[nx2-1][ny2-1] && Z < eps3[nx3-1][ny3-1]) 
	*acctheta << theta << endl;
	*accphi << phi << endl;
	htheta->Fill(theta);
	hphi->Fill(phi);
	j+=1;}
    }
  }
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
