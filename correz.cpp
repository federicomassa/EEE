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



void correz(){
  int stripnum = -1;
  double corrx = 0, corry1 = 0, corry2 = 0, corry3 = 0;
  int chnum = 0;
  char dest[80] = "../Data/";
  char ofile[80];
  strcpy(ofile,dest);
  char infile[80] = "EEE_PISA01TestRun4Telescopes_20140507_014455.txt";
  strcat(dest,infile);
  ifstream run(dest); //INPUT FILE 
  char crr[80] = "CORR_";
  strcat(ofile,crr);
  strcat(ofile,infile);
  ofstream crrfile(ofile);
  int check = 0;
  int bc1, bc2, bc3;
  double up1[24],up2[24],up3[24],low1[24],low2[24],low3[24];
  int evnum = 0;
 
  TFile rfile("correzione.root","RECREATE");
  TH2F* disxy1 = new TH2F("disxy1","XY Occupancy: Ch 1", 200,-100,100,400,-400,400);
  TH2F* disxy2 = new TH2F("disxy2","XY Occupancy: Ch 2", 200,-100,100,400,-400,400);
  TH2F* disxy3 = new TH2F("disxy3","XY Occupancy: Ch 3", 200,-100,100,400,-400,400);
  triplet n1;
  int j = 0;
  double number = 0, numbery = 0, numberx = 0;
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

     for (k = 0; k <= 50000; k++){

  // do  {k+= 1;// cout << "INIZIO DEL DO" << endl;
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
		    cout << "HIT VALUE IMPOSTATO" << endl;
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
  cout << "FINE CICLO LINEA" << endl;
	  
  //   cout << "DOPO FOR" << endl;
 //Controllo la bontà dell'evento: ha almeno un hit per camera (top_bottom)?
  if (ch1 < 1 ||ch2 < 1 || ch3 < 1){/*cout << "EVENTO NON BUONO" << endl;*/ 
    cout << "EVENTO NON BUONO" << endl;
    j = 0; 
    delete [] hit;
    
    getline(run,line);
    entry = ""; 

    continue;} //Evento non buono: prossimo evento

  else { //evento con almeno un hit per camera
 
   for (int a = 0; a < (ch1); a++) {
     for (int b = 0; b < (ch2); b++) {
       for (int c = 0; c < (ch3); c++) {
   	 	 n1.SetPoints(hit[a],hit[b+ch1],hit[c+ch1+ch2]); //considero tutte le combinazioni di triplette
		 disxy1->Fill(n1.GetCoordinate(0,0),n1.GetCoordinate(1,0));
		 disxy2->Fill(n1.GetCoordinate(0,1),n1.GetCoordinate(1,1));
		 disxy3->Fill(n1.GetCoordinate(0,2),n1.GetCoordinate(1,2));
       }}}

  } //fine else evento buono




  j = 0;
  delete[] hit;
  getline(run,line);
  entry = "";
  //Bin per chamber histograms
   

     }



     //	  	  while (!run.eof());
   for (int a = 0; a < 24; a++) {
     for (int b = 1; b <= 200; b++) {
       bc1 = disxy1->GetBinContent(41+5*a,b+200);
       if (bc1 == 0) check += 1; 
       if (bc1 != 0 && check < 2) check = 0;
       if (check == 2) {up1[a] = double (b-2)*2; check = 0; break;}
     }
     for (int b = 1; b <= 200; b++) {
       bc1 = disxy1->GetBinContent(41+5*a,201-b);
       if (bc1 == 0) check += 1;
       if (bc1 != 0 && check < 2) check = 0;
       if (check == 2) {low1[a] = double(2-b)*2; check = 0;break;}
     }
       
   }
     
  for (int a = 0; a < 24; a++) {
     for (int b = 1; b <= 200; b++) {
       bc2 = disxy2->GetBinContent(41+5*a,b+200);
       if (bc2 == 0) check += 1; 
       if (bc2!= 0 && check < 2) check = 0;
       if (check == 2) {up2[a] = double(b-2)*2; check = 0; break;}
     }
     for (int b = 1; b <= 200; b++) {
       bc2 = disxy2->GetBinContent(41+5*a,201-b);
       if (bc2 == 0) check += 1;
       if (bc2 != 0 && check < 2) check = 0;
       if (check == 2) {low2[a] = double(2-b)*2; check = 0; break;}
     }
       
   }


  for (int a = 0; a < 24; a++) {
     for (int b = 1; b <= 200; b++) {
       bc3 = disxy3->GetBinContent(41+5*a,b+200);
       if (bc3 == 0) check += 1; 
       if (bc3 != 0 && check < 2) check = 0;
       if (check == 2) {up3[a] = double(b-2)*2; check = 0; break;}
     }
     for (int b = 1; b <= 200; b++) {
       bc3 = disxy3->GetBinContent(41+5*a,201-b);
       if (bc3 == 0) check += 1;
       if (bc3 != 0 && check < 2) check = 0;
       if (check == 2) {low3[a] = double(2-b)*2; check = 0;break;}
     }
       
  }


 //   for (int bb = 0; bb < 24; bb++) {
 // cout << up1[bb]-low1[bb] << endl;
 // cout << "UP: " << up1[bb] << endl;
 // cout << "LOW: " << low1[bb] << endl;
 // cout << '\n';
 // cout << up2[bb]-low2[bb] << endl;
 // cout << "UP: " << up2[bb] << endl;
 // cout << "LOW: " << low2[bb] << endl;
 // cout << '\n';
 // cout << up3[bb]-low3[bb] << endl;
 // cout << "UP: " << up3[bb] << endl;
 // cout << "LOW: " << low3[bb] << endl;
 // cout << '\n' << '\n';
 //  }

   corrx = 82.0/(60.0+55.0);
   for (int i = 0; i < 24; i++) {corry1 += up1[i]-low1[i]; corry2 += up2[i]-low2[i]; corry3 += up3[i] - low3[i];}
   corry1 = corry1/22;  //strip mancanti: media
   corry2 = corry2/23;
   corry3 = corry3/21;

   corry1 = 158.0/corry1;  //y rescaling
   corry2 = 158.0/corry2;
   corry3 = 158.0/corry3;

   cout << '\n' << "FINITO. ORA RICOMINCIA E CORREGGE" << '\n' << endl;

   ////////////////////////////////////RICOMINCIA DA CAPO E CORREGGI ///////////////////////////////////////

   j = 0;
   run.seekg(0,ios::beg);
   getline(run,line);

   do {
   // for (k = 0; k <= 50000; k++){

  // do  {k+= 1;// cout << "INIZIO DEL DO" << endl;
	 //  if (m%5000 == 0) cout << m << " eventi analizzati..." << endl;
     if (line.find("EVENT") > 15 ) {crrfile << line << endl; getline(run,line);j = 0; continue;}//Durante il run compaiono righe non di evento, se non lo trova find restituisce un numero molto grande
    linecount = GetHitCount(line);
    if (linecount == 0){getline(run,line); j = 0; continue;}

      point* hit = new point[linecount];//alloca la memoria per tutti i punti dell'evento
       //   cout << point::n << endl;
  // cout << GetHitCount(line) << endl;
  for (unsigned int i = 0; i < line.size()+1; i++) {
    if(!isalnum(line[i]) && line[i] != '.' && line[i] != '-') {
      j += 1;
      if (j < 9) crrfile << entry << ' ';
      if (entry != "" && j == 4) {stringstream(entry) >> evnum; cout << "EVENTO NUMERO: " << evnum << endl;} 
      if (entry != "" && j >= 9) {
	stringstream(entry) >> number;
	//  cout << number << endl;
	hit[int(floor((double(j)-9)/3))].SetValue(j%3,number);
	//	    cout << "OK" << endl;
	if (j%3 == 2) {
    	chnum = hit[int(floor((double(j)-9)/3))].GetChNumber();
	if (chnum == 1) crrfile << (numbery - (up1[stripnum]+low1[stripnum])/2)*corry1 << ' ';
	if (chnum == 2) crrfile << (numbery - (up2[stripnum]+low2[stripnum])/2)*corry2 << ' ';
	if (chnum == 3) crrfile << (numbery - (up3[stripnum]+low3[stripnum])/2)*corry3 << ' ';
	crrfile << number << ' ';
	} 
	if (j%3 == 0) {numberx = number; stripnum = int((numberx+60.0)/5.0); crrfile << numberx*corrx << ' ';}
	if (j%3 == 1) numbery = number;
      }
      entry = "";}
      else entry = entry + line.substr(i,1);
  } //fine ciclo linea
     j = 0;
  delete[] hit;
  getline(run,line);
  entry = "";
  crrfile << endl;}
   while (!run.eof());
    
   disxy1->Write();
   disxy2->Write();
   disxy3->Write();
     rfile.Close();
     run.close();
     crrfile.close();
}
  
