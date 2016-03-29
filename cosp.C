#include "TFile.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TString.h"
#include "TLegend.h"
#include <iostream>
#include <fstream>

void cosp(){
	
	TString names[7];
	names[0]="46.9";
	names[1]="46.5";
	names[2]="46.0";
	names[3]="45.6";
	names[4]="45.1";
	names[5]="44.7";
	names[6]="44.2";
	TString fname;
	TString hname;
	TFile* files[7];
	TH1F* hcos[7];
	TF1* func = new TF1("fitfunc","[0]*(1+x**2)+[1]*(1-x)**-2",-1,1);
	ofstream tfile;
	tfile.open("fs.txt");
	for(int i; i<7; i++){
		fname="histos"+names[i]+".root";
		cout << fname << endl;
		files[i]= new TFile(fname,"READ");
		hname="h_costhet_"+names[i];
		hcos[i] = (TH1F*)files[i]->Get(hname);
		cout << "Strahlenergie:"+names[i] << endl;
		TCanvas* c = new TCanvas("c", "c");
		hcos[i]->Fit("fitfunc","","",-0.9,0.9);
		hcos[i]->Draw();
		c->SaveAs("fit"+names[i]+".png");
		c->Close();
		float S = func->GetParameter(0);
		cout << "S=" << S << endl;
		float T = func->GetParameter(1);
		cout << "T=" << T << endl;
		float fs = 1.0/(1+T*9.47368/(S*2.286));
		cout << "fs=" << fs << endl;
		tfile << fs << ",";
		int Ns = hcos[i]->GetEntries();
		cout << "Ns=" << Ns << endl;
		
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
