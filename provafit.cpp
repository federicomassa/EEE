void provafit(){
  double x[5] = {1.01,1.99,3.01,3.99,5.01};
  double y[5] = {8.99,8.01,7.01,5.99,5.01};
  double xerr[5] = {0.01,0.01,0.01,0.01,0.01};
  double yerr[5] = {0.01,0.01,0.01,0.01,0.01};
  TGraphErrors graph(5,x,y,xerr,yerr);
  TF1 fitfunc("Fitting","[0]+[1]*x",0,6);
  fitfunc.SetParameters(10,-1);
  TFitResultPtr frp = graph.Fit(&fitfunc,"0NS");
}
