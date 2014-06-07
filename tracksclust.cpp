//LA FINE DI OGNI RIGA DEVE CONTENERE UNO SPAZIO BIANCO
#include <TF1.h>
#include <TGraphErrors.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph2D.h>
#include <TFile.h>
#include <iostream>
#include "fit.cpp"  //contiene le classi point e triplet, e le variabili zch1, zch2, zch3
#include <fstream>

#include <string>
#include <sstream>

using namespace std;


int GetHitCount(string str){ //Calcola il numero di hits per evento
  int s = 0;
  for (unsigned int i = 0; i < str.size(); i++) {
    if(!isalnum(str[i]) && str[i] != '.' && str[i] != '-') //se trova un carattere non alfanumerico o . allora è la fine di un blocco
      s = s+1;
  }
  return (s-9)/3;
}; // 9 blocchi prima dell'inizio delle 3 coordinate



void tracksclust(){  

  //variabili di cluster
  double xrad1 = 10;
  double xrad2 = 10;
  double xrad3 = 10;
  double yrad1 = 3;
  double yrad2 = 3;
  double yrad3 = 3;

  double midx1= 0, midx2= 0, midx3= 0, midy1= 0, midy2 = 0, midy3 = 0;
  double jx1 = 0, jx2 = 0, jx3 = 0, jy1 = 0, jy2 = 0, jy3 = 0;
  
  int besta = 1000, bestb = 1000, bestc = 1000; //coordinate dei punti migliori

  double XYint, XZint, YZint, XYpen, YZpen, XZpen; //variabili del fit migliore

  double xav1 = 0, xav2 = 0, xav3 = 0, yav1 = 0, yav2 = 0, yav3 = 0;

  TH1F* clustdistheta = new TH1F("cdist","Clustered Theta distribution", 50, 0,2*atan(1.));
  TH1F* clustdisphi = new TH1F("cdisp","Clustered Phi distribution", 100, 0, 8*atan(1.));


  ifstream run("EEE_PISA01TestRun4Telescopes_20140507_014455.txt"); //INPUT FILE
  //  int point::n = 0;
  TFile rfile("Distclust.root","RECREATE");
  TH1F* dischi = new TH1F("dischi","Chi2 distribution; chi2; #", 100,0,1000);
  TH1F* hpc1 = new TH1F("hpc1", "Hit per chamber / Chamber 1; #Hits;# ", 20,0,20);
  TH1F* hpc2 = new TH1F("hpc2", "Hit per chamber / Chamber 2;#Hits;#", 20,0,20);
  TH1F* hpc3 = new TH1F("hpc3", "Hit per chamber /Chamber 3;#Hits;#", 20,0,20);
  TH1F* distheta = new TH1F("dist","Theta distribution", 50, 0,2*atan(1.));
  TH1F* disphi = new TH1F("disp","Phi distribution", 100, 0, 8*atan(1.));
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
  double chi = 1000, tempchi = 0, theta = 1000, temptheta = 0, phi = 1000, tempphi = 0;
  int j = 0;
  double number = 0;
  string line;
  string entry = "";
  int ch1 = 0;
  int ch2 = 0;
  int ch3 = 0;
  int linecount = 0;

  //   const char inputname[] = "disperazione.txt";
  //  const char inputname[] = "desp.txt";

  // ofstream prova("prova.txt"); // OUTPUT per debug
  do{
    getline(run,line);
  }
  while (line.substr(11,5) != "EVENT");
  // for (int n = 0; n <= 108;n++) {getline(run,line);}

        for (int k = 0; k <= 5000; k++){

  // do  {// cout << "INIZIO DEL DO" << endl;
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
      if (entry != "" && j == 4) {stringstream(entry) >> number; cout << "EVENTO NUMERO: " << number << endl;} 
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
  
  if (k == 608) evdisplay->Write();

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

 for (int h = ch3; h > 0; h--){
    for(int p = ch3-1; p < h && p >= 0; p--){
	  disdist3->Fill(pow(pow(hit[h+ch1+ch2].x-hit[p+ch1+ch2].x,2)+pow(hit[h+ch1+ch2].y-hit[p+ch1+ch2].y,2),0.5));
    disdistx3->Fill(hit[h+ch1+ch2].x-hit[p+ch1+ch2].x);
    disdisty3->Fill(hit[h+ch1+ch2].y-hit[p+ch1+ch2].y);
    }}
	  
  //   cout << "DOPO FOR" << endl;
 //Controllo la bontà dell'evento: ha almeno un hit per camera?
  if (ch1 < 1 || ch2 < 1 || ch3 < 1){/*cout << "EVENTO NON BUONO" << endl;*/ 
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

  else { //evento con almeno un hit per camera
   
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
		  n1.XZFit();
   		  //  cout << "DOPO FIT" << endl;
   		  tempchi = (n1.XYGetChisquare() + n1.XZGetChisquare() + n1.YZGetChisquare())/3;
   		  //		  cout << "DOPO CHI" << endl;
   		  temptheta = n1.GetTheta();
   		  tempphi = n1.GetPhi();
   		  if (tempchi < chi && tempchi != 0) {chi = tempchi; theta = temptheta; phi = tempphi; besta = a; bestb = b; bestc = c;}
       }}}
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

      //CLUSTER
      {
	 XYint = n1.XYGetParameter(0);
	 YZint = n1.YZGetParameter(0);
	 XZint = n1.XZGetParameter(0);
 
	 XYpen= n1.XYGetParameter(1);
	 YZpen= n1.YZGetParameter(1);
	 XZpen = n1.XZGetParameter(1);

	 midx1 = (zch1-XZint)/XZpen;
	 midx2 = (zch2-XZint)/XZpen;
	 midx3 = (zch3-XZint)/XZpen;

	 midy1 = (zch1-YZint)/YZpen;
	 midy2 = (zch2-YZint)/YZpen;
	 midy3 = (zch3-YZint)/YZpen;

	 for(int m1 = 0; m1 < ch1; m1++) {
	   if(absval(hit[m1].x - midx1) <= xrad1) {xav1 = (xav1*jx1+ hit[m1].x)/(jx1+1);jx1+= 1;}
	   else {xav1 = hit[besta].x;}
	   
	    if(absval(hit[m1].y - midy1) <= yrad1) {yav1 = (yav1*jy1+ hit[m1].y)/(jy1+1);jy1+= 1;}
	   else {yav1 = hit[besta].y;}
	 }

	 for(int m2 = 0; m2 < ch2; m2++) {
	   if(absval(hit[m2+ch1].x - midx2) <= xrad2) {xav2 = (xav2*jx2+ hit[m2+ch1].x)/(jx2+1);jx2+= 1;}
	   else {xav2 = hit[bestb+ch1].x;}
	   
	    if(absval(hit[m2+ch1].y - midy2) <= yrad2) {yav2 = (yav2*jy2+ hit[m2+ch1].y)/(jy2+1);jy2+= 1;}
	   else {yav2 = hit[bestb+ch1].y;}
	 }

	 for(int m3 = 0; m3 < ch3; m3++) {
	   if(absval(hit[m3+ch1+ch2].x - midx3) <= xrad3) {xav3 = (xav3*jx3+ hit[m3+ch1+ch2].x)/(jx3+1);jx3+= 1;}
	   else {xav3 = hit[bestc+ch1+ch2].x;}
	   
	    if(absval(hit[m3+ch1+ch2].y - midy3) <= yrad3) {yav3 = (yav3*jy3+ hit[m3+ch1+ch2].y)/(jy3+1);jy3+= 1;}
	   else {yav3 = hit[bestc+ch1+ch2].y;}
	 }

	 if (chi > 0 && chi < 10) {
  distheta->Fill(theta);
  disphi->Fill(phi); }

  hit[0].SetValues(xav1,yav1,zch1);
  hit[1].SetValues(xav2,yav2,zch2);
  hit[2].SetValues(xav3,yav3,zch3);
  n1.SetPoints(hit[0],hit[1],hit[2]);
  n1.Fit();
  chi = (n1.XYGetChisquare()+n1.YZGetChisquare()+n1.XZGetChisquare())/3;
  
      } //END CLUSTER
  
   //Riempiamo gli istogrammi di theta e phi se il fit è andato bene
       if(chi > 0 && chi < 10){
     // cout << "Fit con chi2: " << chi << endl;
     // cout << "Theta: " << n1.GetTheta() << endl;
     // cout << "Phi: " << n1.GetPhi() << endl;
	 //   if (theta <= 0.025){cout << "TROVATO THETA = 0 in evento" << k << " con theta: " << theta << " e phi: " << phi << " xyparamter: " << n1.XYGetParameter(1) << " yzparameter: " << n1.YZGetParameter(1) << endl; cin.get();}
       
	 clustdistheta->Fill(n1.GetTheta());
	 clustdisphi->Fill(n1.GetPhi());



       }
   //  else {
   //  jj += 1;
   //  if (jj == 1) {chi2 << chi << endl; n1.XYDraw(); n1.YZDraw();}
  

 
  } // END taglio chi

  //Reset
  j = 0;
  jx1 = 0;
  jx2 = 0;
  jx3 = 0;
  jy1 = 0;
  jy2 = 0;
  jy3 = 0;

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
   hpc3->Fill(ch3);
   disz->SetBinContent(4,disz->GetBinContent(4)+ch1);
   disz->SetBinContent(8,disz->GetBinContent(8)+ch2);
   disz->SetBinContent(12,disz->GetBinContent(12)+ch3);
   
   // cout << "FINE DEL DO" << endl;
	}



	  //	  while (!run.eof());
     // TCanvas* thetacanv = new TCanvas();
     // thetacanv->SetGrid();
     // thetacanv->cd();
     // distheta->Draw();
  
      //TCanvas* phicanv = new TCanvas();
       //phicanv->SetGrid();
      //phicanv->cd();
  //   disphi->Draw();
	clustdistheta->Write();
	clustdisphi->Write();
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
}
  
