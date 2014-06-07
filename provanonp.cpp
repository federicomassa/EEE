void provanonp(){
  TF1 hist;
  hist.TF1("ciao","sin(x)/x",-10,10);
  hist.DrawClone();
}
