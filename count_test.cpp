//Script per verificare che selezionare solamente gli eventi con un hit per camera non cambia l'efficienza

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



void count_test(){
  int evnum = 0;
  int eff3 = 0, eff2 = 0;
  int s_eff3 = 0, s_eff2 = 0;
  ifstream run("../Data/EEE_Prova_topbottom8900__20140530_174533.txt"); //INPUT FILE  

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
  //   for (int n = 0; n <= 4999;n++) {getline(run,line);}

     for (k = 0; k <= 10000; k++){

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
	  
 //Controllo la bontà dell'evento: ha almeno un hit per camera (top_bottom)?
  if (ch1 < 1 || ch3 < 1){/*cout << "EVENTO NON BUONO" << endl;*/ 
    j = 0; 
    delete [] hit;
    getline(run,line);
    entry = ""; 

    continue;} //Evento non buono: prossimo evento

  else { //evento con almeno un hit per camera (top_bottom)
 
    eff2 += 1;
    if (ch1*ch3 == 1 && ch2 == 0 ) s_eff2 += 1;
    if (ch2 == 0) { j = 0; 
    delete [] hit;
    getline(run,line);
    entry = ""; 

    continue;}
    else if (ch1*ch2*ch3 == 1) {s_eff3+= 1; eff3 += 1;}
    else eff3 += 1;
    j = 0;
    delete[] hit;
    // cout << point::n << endl;
    getline(run,line);
    entry = "";
    //Bin per chamber histograms

   
  }	  }//FINE DEL DO



     //	  	  while (!run.eof());

     run.close();
    
     cout << "EFFICIENZA: " << double(eff3)/double(eff2) << endl;
     cout << "EFFICIENZA SINGOLA: " << double(s_eff3)/double(s_eff2+s_eff3) << endl;

     cout << "TOTALI MULTIPLI: " << eff2 << endl;
     cout << "TOTALI SINGOLI: " << (s_eff2+s_eff3) << endl;
}
  
