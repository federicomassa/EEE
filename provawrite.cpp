void provawrite(){
  TF1 h("my_histogram","x+1",-5,5);
  TFile out_file("my_rootfile.root","RECREATE");
  h.Write();
  out_file.Close();
}
