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
  for (unsigned int i = 0; i < str.size(); i++) {
    if(!isalnum(str[i]) && str[i] != '.' && str[i] != '-') //se trova un carattere non alfanumerico o . allora è la fine di un blocco
      s = s+1;
  }
  return (s-9)/3;
}; // 9 blocchi prima dell'inizio delle 3 coordinate



void cluster(){
  ifstream run("EEE_PISA01TestRun4Telescopes_20140507_014455.txt"); //INPUT FILE
  //  int point::n = 0;
  TFile rfile("cluster2.root","RECREATE");
  double midx1, midx2, midx3, midy1, midy2, midy3;
  double XYint, XZint, YZint, XYpen, XZpen, YZpen;
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

  TH1F* cluster1X = new TH1F("cluster1x","Hits-Fit X Distance Distribution | Ch 1;X Distance [cm];#",120,-120,120);
  TH1F* cluster2X = new TH1F("cluster2x","Hits-Fit X Distance Distribution | Ch 2;X Distance [cm];#",120,-120,120);
  TH1F* cluster3X = new TH1F("cluster3x","Hits-Fit X Distance Distribution | Ch 3;X Distance [cm];#",120,-120,120);
  TH1F* cluster1Y = new TH1F("cluster1y","Hits-Fit Y Distance Distribution | Ch 1;Y Distance [cm];#",400,-200,200);
  TH1F* cluster2Y = new TH1F("cluster2y","Hits-Fit Y Distance Distribution | Ch 2;Y Distance [cm];#",400,-200,200);
  TH1F* cluster3Y = new TH1F("cluster3y","Hits-Fit Y Distance Distribution | Ch 3;Y Distance [cm];#",400,-200,200);

  //   const char inputname[] = "disperazione.txt";
  //  const char inputname[] = "desp.txt";

  // ofstream prova("prova.txt"); // OUTPUT per debug
  do{
    getline(run,line);
  }
  while (line.substr(11,5) != "EVENT");
  // for (int n = 0; n <= 108;n++) {getline(run,line);}

  //  for (int k = 0; k <= 1000; k++){

   do  {// cout << "INIZIO DEL DO" << endl;
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

	  
  //   cout << "DOPO FOR" << endl;
 //Controllo la bontà dell'evento: ha almeno un hit per camera?
  if (ch1 < 1 || ch2 < 1 || ch3 < 1){/*cout << "EVENTO NON BUONO" << endl;*/ 
    j = 0; 
    delete [] hit;
    // cout << point::n << endl;
    getline(run,line);
    entry = ""; 

    continue;} //Evento non buono: prossimo evento

  else { //evento con almeno un hit per camera
   
   for (int a = 0; a < (ch1); a++) {
     for (int b = 0; b < (ch2); b++) {
       for (int c = 0; c < (ch3); c++) {
   	 	 n1.SetPoints(hit[a],hit[b+ch1],hit[c+ch1+ch2]); //considero tutte le combinazioni di triplette
   		  //		  cout << "PRIMA FIT" << endl;
   		  n1.XYFit();
   		  n1.YZFit();
		  n1.XZFit();
   		  //  cout << "DOPO FIT" << endl;
   		  tempchi = (n1.XYGetChisquare() + n1.XZGetChisquare() +  n1.YZGetChisquare())/3;
   		  //		  cout << "DOPO CHI" << endl;
   		  if (tempchi < chi && tempchi != 0) {
chi = tempchi; 
 XYint = n1.XYGetParameter(0);
 YZint = n1.YZGetParameter(0);
 XZint = n1.XZGetParameter(0);
 
 XYpen= n1.XYGetParameter(1);
 YZpen= n1.YZGetParameter(1);
 XZpen = n1.XZGetParameter(1);
 
		  }


       }}}

   //Riempiamo gli istogrammi di theta e phi se il fit è andato bene
       if(chi > 0 && chi < 10){

	 midx1 = (zch1-XZint)/XZpen;
	 midx2 = (zch2-XZint)/XZpen;
	 midx3 = (zch3-XZint)/XZpen;

	 midy1 = (zch1-YZint)/YZpen;
	 midy2 = (zch2-YZint)/YZpen;
	 midy3 = (zch3-YZint)/YZpen;

	 for(int i1 = 0; i1 < ch1; i1++) {
	   cluster1X->Fill(hit[i1].x-midx1);
	   cluster1Y->Fill(hit[i1].y-midy1);
	 }
	 for(int i2 = 0; i2 < ch2; i2++) {
	   cluster2X->Fill(hit[i2+ch1].x-midx2);
	   cluster2Y->Fill(hit[i2+ch1].y-midy2);
	 }
	 for(int i3 = 0; i3 < ch3; i3++) {
	   cluster3X->Fill(hit[i3+ch1+ch2].x-midx3);
	   cluster3Y->Fill(hit[i3+ch1+ch2].y-midy3);
	 }

       }

   //  else {
   //  jj += 1;
   //  if (jj == 1) {chi2 << chi << endl; n1.XYDraw(); n1.YZDraw();}
  }
  j = 0;
  delete[] hit;
  // cout << point::n << endl;
  getline(run,line);
  entry = "";
  chi = 1000;
  tempchi = 0;
   
   // cout << "FINE DEL DO" << endl;
  }



      	  while (!run.eof());
cluster1X->Write();
cluster2X->Write();
cluster3X->Write();
cluster1Y->Write();
cluster2Y->Write();
cluster3Y->Write();
rfile.Close();
run.close();
}
  
