#include "TFile.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TString.h"
#include "TLegend.h"
#include <iostream>
#include <fstream>

void cosp(){
	float fs_a[7];
	float fs_err_a[7];
	TString names[7];
	names[6]="46.9";
	names[5]="46.5";
	names[4]="46.0";
	names[3]="45.6";
	names[2]="45.1";
	names[1]="44.7";
	names[0]="44.2";
	TString fname;
	TString hname;
	TFile* files[7];
	TH1F* hcos[7];
	TF1* func = new TF1("fitfunc","[0]*(1+x**2)+[1]*(1-x)**-2",-1,1);
	float cT=9.47368;
	float cS=2.286;
	Double_t w = 1000;
	Double_t h = 618;
	for(int i=0; i<7; i++){
		fname="..\\results\\data_results\\histos"+names[i]+".root";
		cout << fname << endl;
		files[i]= new TFile(fname,"READ");
		hname="h_costhet_"+names[i];
		hcos[i] = (TH1F*)files[i]->Get(hname);
		hcos[i]-> Rebin(5);
		cout << "Strahlenergie:"+names[i] << endl;
		TCanvas *c1 = new TCanvas("c", "c", w, h);
		c->SetWindowSize(w + (w - c->GetWw()), h + (h - c->GetWh()));
		TFitResultPtr r=hcos[i]->Fit("fitfunc","S","",-0.9,0.9);
		r->Print("V");
		TMatrixDSym cov = r->GetCovarianceMatrix();
		hcos[i]->Draw("HIST");
		func -> Draw("SAME");
		c->SaveAs("C:\\Users\\benjamin\\Documents\\FPII\\Z0\\results\\data_results\\cosp_fits\\fit"+names[i]+".png");
		c->Close();
		float S = func->GetParameter(0);
		float T = func->GetParameter(1);
		float fs = 1.0/(1+T*cT/(S*cS));
		fs_a[i]=fs;
		float fs_err = cT**2*cS**2/(cT*T+cS*S)**4
		*(T**2*cov(0,0)+2*T*S*cov(0,1)+S**2*cov(1,1));
		fs_err=sqrt(fs_err);
		fs_err_a[i]=fs_err;
		cout << "fs=" << fs << endl;
		int Ns = hcos[i]->GetEntries();
		cout << "Ns=" << Ns << endl;	
	}
	ofstream tfile;
	tfile.open("..\\results\\data_results\\fs.txt");
	tfile << "fs: ";
	for(int j=0;j<7;j++){
		cout<< fs_a[j] <<"("<<j<<") ";
		tfile << fs_a[j] << ",";
	}
	tfile << endl << "fs_err: ";
	for(int i=0;i<7;i++){
		tfile << fs_err_a[i] << ",";
	}
	tfile.close();
	/*
	TFile* file = new TFile("histos45.6.root","READ");
  TH1F* hcos_thet = (TH1F*)file->Get("h_costhet_45.6");
	//fit function
	TF1* func = new TF1("fitfunc","[0]*(1+x**2)+[1]*(1-x)**-2",-1,1);
	hcos_thet->Fit("fitfunc","","",-0.9,0.9);
	float S = func->GetParameter(0);
	cout << "S=" << S << endl;
	float T = func->GetParameter(1);
	cout << "T=" << T << endl;
	float fs = 1.0/(1+T*9.47368/(S*2.286));
	cout << "fs=" << fs << endl;
	int Ns = hcos_thet->GetEntries();
	cout << "Ns=" << Ns << endl;
	*/
}
