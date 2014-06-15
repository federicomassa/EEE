//LA FINE DI OGNI RIGA DEVE CONTENERE UNO SPAZIO BIANCO
#include <TF1.h>
#include <TGraphErrors.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph2D.h>
#include <TFile.h>
#include <iostream>
#include "fit.cpp"  //contiene le classi point e triplet
#include <fstream>

#include <string>
#include <sstream>

using namespace std;

int GetHitCount(string str){ //Calcola il numero di hits per evento
  int s = 0;
  for (unsigned int i = 0; i < str.size()+1; i++) {
    if(!isalnum(str[i]) && str[i] != '.' && str[i] != '-') //se trova un carattere non alfanumerico o . allora è la fine di un blocco
      s = s+1;
  }
  return (s-9)/3;
}; // 9 blocchi prima dell'inizio delle 3 coordinate



void top_mid_tracks(){
  double x3, y3, z3;
  int count2 = 0;
  int evnum = 0;
  int eff3 = 0, eff2 = 0;
  bool bestvert;
  double *besty = new double[3];
  double parameter = 0;
   ifstream run("../Data/CORR_EEE_Prova_topmid9000__20140530_181026.txt"); //INPUT FILE  
  TFile rfile("Disttopmid_9000.root","RECREATE");
  TH2F* zeroch3 = new TH2F("zeroch3","Hit distribution when ch3 = 0", 30,0,15,30,0,15);
  TH1F* dischi = new TH1F("dischi","Chi2 distribution; chi2; #", 100,0,1000);
  TH1F* hpc1 = new TH1F("hpc1", "Hit per chamber / Chamber 1; #Hits;# ", 20,0,20);
  TH1F* hpc2 = new TH1F("hpc2", "Hit per chamber / Chamber 2;#Hits;#", 20,0,20);
  TH1F* hpc3 = new TH1F("hpc3", "Hit per chamber /Chamber 3;#Hits;#", 20,0,20);
  TH1F* distheta = new TH1F("dist","Theta distribution", 50, 0,90);
  TH1F* disphi = new TH1F("disp","Phi distribution", 90, 0, 360);
  TH2F* disxy1 = new TH2F("disxy1","XY Occupancy: Ch 1", 200,-100,100,100,-400,400);
  TH1F* disx1 = new TH1F("disx1", "X Occupancy: Ch 1", 200,-100,100);
  TH1F* disy1 = new TH1F("disy1", "Y Occupancy: Ch 1", 100,-400,400);
  TH2F* disxy2 = new TH2F("disxy2","XY Occupancy: Ch 2", 200,-100,100,100,-400,400);
  TH1F* disx2 = new TH1F("disx2", "X Occupancy: Ch 2", 200,-100,100);
  TH1F* disy2 = new TH1F("disy2", "Y Occupancy: Ch 2", 100,-400,400);
  TH2F* disxy3 = new TH2F("disxy3","XY Occupancy: Ch 3", 200,-100,100,100,-400,400);
  TH1F* disx3 = new TH1F("disx3", "X Occupancy: Ch 3", 200,-100,100);
  TH1F* disy3 = new TH1F("disy3", "Y Occupancy: Ch 3", 100,-400,400);
  TH1F* disz = new TH1F("disz", "Z Occupancy; Ch Number; #", 15,0,4);
  TH1F* disdist1 = new TH1F("disdist1","Distance distribution, Ch 1;Distance;#",200,0,600);
  TH1F* disdist2 = new TH1F("disdist2","Distance distribution, Ch 2;Distance;#",200,0,600);
  TH1F* disdist3 = new TH1F("disdist3","Distance distribution, Ch 3;Distance;#",200,0,600);
  TH1F* disdistx1 = new TH1F("disdistx1","X Distance distribution, Ch1;X Distance;#",200,-100,100);
  TH1F* disdistx2 = new TH1F("disdistx2","X Distance distribution, Ch2;X Distance;#",200,-100,100);
  TH1F* disdistx3 = new TH1F("disdistx3","X Distance distribution, Ch3;X Distance;#",200,-100,100);
  TH1F* disdisty1 = new TH1F("disdisty1","Y Distance distribution, Ch1;Y Distance;#",800,-400,400);
  TH1F* disdisty2 = new TH1F("disdisty2","Y Distance distribution, Ch2;Y Distance;#",800,-400,400);
  TH1F* disdisty3 = new TH1F("disdisty3","Y Distance distribution, Ch3;Y Distance;#",800,-400,400);
  triplet n1;
  double chi = 1000, tempchi = 0,xytempchi = 0, xztempchi = 0, yztempchi = 0, theta = 1000, phi = 1000;
  int j = 0;
  double number = 0;
  string line;
  string entry = "";
  int ch1 = 0;
  int ch2 = 0;
  int ch3 = 0;
  int linecount = 0;
  int k = -1;
  do{
    getline(run,line);
  }
  while (line.substr(11,5) != "EVENT");
  // for (int n = 0; n <= 108;n++) {getline(run,line);}

  //   for (k = 0; k <= 10000; k++){

   do  {k+= 1;// cout << "INIZIO DEL DO" << endl;
	 //  if (m%5000 == 0) cout << m << " eventi analizzati..." << endl;
     if (line.find("EVENT") > 15 ) {getline(run,line);continue;}//Durante il run compaiono righe non di evento, se non lo trova find restituisce un numero molto grande
    ch1 = 0;
    ch2 = 0;
    ch3 = 0;

    linecount = GetHitCount(line);

    if (linecount == 0){getline(run,line); continue;}

       point* hit = new point[linecount];//alloca la memoria per tutti i punti dell'evento
       //   cout << point::n << endl;
  // cout << GetHitCount(line) << endl;
  for (unsigned int i = 0; i < line.size()+1; i++) {
    if(!isalnum(line[i]) && line[i] != '.' && line[i] != '-') {
      j = j+1;
      if (entry != "" && j == 4) {stringstream(entry) >> evnum; cout << "EVENTO NUMERO: " << evnum << endl;} 
      if (entry != "" && j >= 9) {
	stringstream(entry) >> number;
	//    	cout << number << endl;
	hit[int(floor((double(j)-9)/3))].SetValue(j%3,number);
	//	    cout << "OK" << endl;
	if (j%3 == 2) {
	  switch (hit[int(floor((double(j)-9)/3))].GetChNumber()) {
	  case(1):
	    ch1 += 1;
	    break;
	  case(2):
	    ch2 += 1;
	    break;
	  case(3):
	    ch3 += 1;
	    break;
	  };}
      } //divide l'evento nei vari punti
      entry = "";}
      else entry = entry + line.substr(i,1);
  } //fine ciclo linea


  double* x = new double[linecount];
  double* y = new double[linecount];
  double* z = new double[linecount];


   for (int q = 0; q < linecount; q++){
     x[q] = hit[q].x;
     y[q] = hit[q].y;
     z[q] = hit[q].z;}
   TGraph2D* evdisplay = new TGraph2D(linecount,x,y,z);
    evdisplay->SetTitle("Event Display;X;Y;Z");

  

    // qui inserire event display
    //         if (evnum == 148) evdisplay->Write(); 

  //Plot distanze tra due hit, per le camere 2 e 3 aggiunto un offset di ch1 o (ch1+ch2) perché gli 
  // hit sono in ordine di camera nel file
  for (int h = ch1; h > 0; h--){
    for(int p = ch1-1; p < h && p >= 0; p--){
	  disdist1->Fill(pow(pow(hit[h].x-hit[p].x,2)+pow(hit[h].y-hit[p].y,2),0.5));
    disdistx1->Fill(hit[h].x-hit[p].x);
    disdisty1->Fill(hit[h].y-hit[p].y);
    }}

 for (int h = ch2; h > 0; h--){
    for(int p = ch2-1; p < h && p >= 0; p--){
	  disdist2->Fill(pow(pow(hit[h+ch1].x-hit[p+ch1].x,2)+pow(hit[h+ch1].y-hit[p+ch1].y,2),0.5));
	      disdistx2->Fill(hit[h+ch1].x-hit[p+ch1].x);
    disdisty2->Fill(hit[h+ch1].y-hit[p+ch1].y);
    }}

 // for (int h = ch3; h > 0; h--){
 //    for(int p = ch3-1; p < h && p >= 0; p--){
 // 	  disdist3->Fill(pow(pow(hit[h+ch1+ch2].x-hit[p+ch1+ch2].x,2)+pow(hit[h+ch1+ch2].y-hit[p+ch1+ch2].y,2),0.5));
 //    disdistx3->Fill(hit[h+ch1+ch2].x-hit[p+ch1+ch2].x);
 //    disdisty3->Fill(hit[h+ch1+ch2].y-hit[p+ch1+ch2].y);
 //    }}
	  
  //   cout << "DOPO FOR" << endl;
 //Controllo la bontà dell'evento: ha almeno un hit per camera (top_mid)?
  if (ch1 == 0 || ch2 == 0){/*cout << "EVENTO NON BUONO" << endl;*/ 
    j = 0; 
    delete [] hit;
    // cout << point::n << endl;
    delete[] x;
    delete[] y;
    delete[] z;
     delete evdisplay;
    getline(run,line);
    entry = ""; 

    continue;} //Evento non buono: prossimo evento
  
  else {//evento con almeno un hit per camera (top_mid)
  

    if (ch3==0) { //funziona solo con file corretti
      zeroch3->Fill(ch1,ch2);
      for (int a = 0; a < ch1; a++) {
	for (int b = 0; b < ch2; b++) {

	  z3 = zch3;
	  x3 = (z3-XZGetIntercept(hit[a],hit[b+ch1]))/XZGetSlope(hit[a],hit[b+ch1]);
	  y3 = (z3-YZGetIntercept(hit[a],hit[b+ch1]))/YZGetSlope(hit[a],hit[b+ch1]);
	
	  if ((sqr(x3) < sqr(82.0-82.0/24.0)) && (sqr(y3) < sqr(158.0))) count2 += 1;
	  
	}}
    }
    if (count2 >= 1) eff2 += 1;
    
    count2 = 0;


    if(ch3 >= 1){
      eff3 += 1;
       //    for (int kk = 0; kk < 3; kk++) {
    //	cout << hit[kk].x << endl;
    //	cout << hit[kk].y << endl;
    //	cout << hit[kk].z << endl;}
   for (int a = 0; a < (ch1); a++) {
     for (int b = 0; b < (ch2); b++) {
       for (int c = 0; c < (ch3); c++) {
   	 	 n1.SetPoints(hit[a],hit[b+ch1],hit[c+ch1+ch2]); //considero tutte le combinazioni di triplette
   		  //		  cout << "PRIMA FIT" << endl;
   		  n1.XYFit();
   		  n1.YZFit();
   		  //  cout << "DOPO FIT" << endl;
		  if (!n1.vert)
		  tempchi = (n1.XYGetChisquare_m()/*+n1.XZGetChisquare_m()*/ +n1.YZGetChisquare_m())/2; //tolto xz per sicurezza: potrebbe essere verticale
		  else tempchi = n1.YZGetChisquare_m()/2;
   		  //		  cout << "DOPO CHI" << endl;
   		  if (tempchi < chi && tempchi != 0) {chi = tempchi; xytempchi = n1.XYGetChisquare_m(); yztempchi = n1.YZGetChisquare_m(); /* xztempchi = n1.XZGetChisquare_m();*/ theta = n1.GetTheta(); phi = n1.GetPhi();/*parameter = n1.YZGetParameter(1);*/besty = n1.yv;bestvert = n1.vert;}
       }}}
   // if (parameter > 100000) {cout << "THETA: " << theta << endl; cout << "PHI: " << phi << endl; cout << "y SOSPETTE: " << besty[0] << " " << besty[1] << " " << besty[2] << endl; cout << "k: " << k << endl;}
   disxy1->Fill(n1.GetCoordinate(0,0),n1.GetCoordinate(1,0));
   disxy2->Fill(n1.GetCoordinate(0,1),n1.GetCoordinate(1,1));
   disxy3->Fill(n1.GetCoordinate(0,2),n1.GetCoordinate(1,2));
   
   disx1->Fill(n1.GetCoordinate(0,0));
   disx2->Fill(n1.GetCoordinate(0,1));
   disx3->Fill(n1.GetCoordinate(0,2));

   disy1->Fill(n1.GetCoordinate(1,0));
   disy2->Fill(n1.GetCoordinate(1,1));
   disy3->Fill(n1.GetCoordinate(1,2));
   
      dischi->Fill(chi);
  
   //Riempiamo gli istogrammi di theta e phi se il fit è andato bene. Se è verticale considero solamente una sezione
      //     if (phi > 1.5 && phi < 1.64 && yztempchi > 30 && bestvert) {cout << "y sospette: " << besty[0] << '\t' << besty[1] << '\t' << besty[2] << endl; cout << "evnum: " << evnum << endl; cin.get();}
      if( ((xytempchi>0 || bestvert) && yztempchi/**xztempchi*/ > 0) && (xytempchi < 30 || bestvert) && (yztempchi < 30)/* && (xztempchi < 10)*/ ){
     // cout << "Fit con chi2: " << chi << endl;
     // cout << "Theta: " << n1.GetTheta() << endl;
     // cout << "Phi: " << n1.GetPhi() << endl;
	//   if (theta <= 0.025){cout << "TROVATO THETA = 0 in evento" << evnum << " con theta: " << theta << " e phi: " << phi << " xyparamter: " << n1.XYGetParameter(1) << " yzparameter: " << n1.YZGetParameter(1) << endl; cin.get();}   
	//	if (theta < 1E-4) {cout << "y SOSPETTE: " << besty[0] << " " << besty[1] << " " << besty[2] << endl; cout << "theta acc: " << theta << endl; cout << "evento da contr: " << evnum << endl; cin.get();}
	//	if (theta > 1.22) {cout << "HUGE THETA at evnum: " << evnum << endl;}
	distheta->Fill(theta*180/3.14159);
	disphi->Fill(phi*180/3.14159);
      }
   //  else {
   //  jj += 1;
   //  if (jj == 1) {chi2 << chi << endl; n1.XYDraw(); n1.YZDraw();}

  j = 0;
    delete[] x;
    delete[] y;
    delete[] z;
     delete evdisplay;
  delete[] hit;
  getline(run,line);
  entry = "";
  chi = 1000;
  tempchi = 0;
  continue;
    }

    //vedere se la traccia entra nella terza camera o meno!
  j = 0;
    delete[] x;
    delete[] y;
    delete[] z;
     delete evdisplay;
  delete[] hit;
  // cout << point::n << endl;
  getline(run,line);
  entry = "";
  chi = 1000;
  tempchi = 0;
  //Bin per chamber histograms
   hpc1->Fill(ch1);
   hpc2->Fill(ch2);
   //  hpc3->Fill(ch3);
   disz->SetBinContent(4,disz->GetBinContent(4)+ch1);
   disz->SetBinContent(8,disz->GetBinContent(8)+ch2);
   disz->SetBinContent(12,disz->GetBinContent(12)+ch3);
   
  }
   
   } // cout << "FINE DEL DO" << endl;



     	  	  while (!run.eof());
     // TCanvas* thetacanv = new TCanvas();
     // thetacanv->SetGrid();
     // thetacanv->cd();
     // distheta->Draw();
  
      //TCanvas* phicanv = new TCanvas();
       //phicanv->SetGrid();
      //phicanv->cd();
  //   disphi->Draw();
   zeroch3->Write();
   disdist1->Write();
   disdist2->Write();
   disdist3->Write();
   disdistx1->Write();
   disdistx2->Write();
   disdistx3->Write();
   disdisty1->Write();
   disdisty2->Write();
   disdisty3->Write();
   disxy1->Write();
   disxy2->Write();
   disxy3->Write();
   disx1->Write();
   disx2->Write();
   disx3->Write();
   disy1->Write();
   disy2->Write();
   disy3->Write();
   disz->Write();
    hpc1->Write();
    hpc2->Write();
    hpc3->Write();
    dischi->Write();
     distheta->Write();
     disphi->Write();
     rfile.Close();
     run.close();
    
     cout << "EFFICIENZA: " << double(eff3)/double(eff2+eff3) << endl;
}
  
