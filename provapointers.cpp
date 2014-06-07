void provapointers(){

  for (int i = 0; i < 3; i++) {
  double* bobby = new double;
  double* teddy;
  teddy = bobby;
  
  bobby[0] = 3.6553;
  cout << bobby[0] << endl;
  cout << teddy[0] << endl;}
  cout << "FINE" << endl;
  delete bobby;
  cout << *teddy << endl;}

