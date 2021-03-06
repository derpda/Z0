#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TString.h"
#include <iostream>

void makeCut(){
	TFile* files[5];
	files[0] = new TFile("../daten/mc/ee.root");
	files[1] = new TFile("../daten/mc/mm.root");
	files[2] = new TFile("../daten/mc/tt.root");
	files[3] = new TFile("../daten/mc/qq.root");
	files[4] = new TFile("../daten/daten/daten_1.root");

	//Make cuts and get the number of events



	files[1]->cd(); //mm ntuple
	//with cuts
	TString mmcuts="Ncharged==2 && Pcharged >=75 && E_ecal<=50";
	TTree *hmm = (TTree*)files[1]->Get("h3");
	float nEvents_mm_all = hmm->Draw("E_ecal>>heEcal_mm_all(200,0,200)",mmcuts);
	TH1F* heEcal_mm_all = (TH1F*) gDirectory->Get("heEcal_mm_all");
	cout << ("n_mm_all=") << nEvents_mm_all << endl;


	files[2]->cd(); //tt ntuple
	//with cuts
	TString ttcuts="Ncharged<=6 && Pcharged>=5 && Pcharged<=50 && E_ecal>=4 && E_ecal<=70";
	TTree *htt = (TTree*)files[2]->Get("h3");
	float nEvents_tt_all = htt->Draw("E_ecal>>heEcal_tt_all(200,0,200)",ttcuts);
	TH1F* heEcal_tt_all = (TH1F*) gDirectory->Get("heEcal_tt_all");
	cout << ("n_tt_all=") << nEvents_tt_all << endl;


	files[3]->cd(); //qq couple
	//with cuts
	TString qqcuts="Ncharged >=10";
	TTree *hqq = (TTree*)files[3]->Get("h3");
	float nEvents_qq_all = hqq->Draw("E_hcal>>heEcal_qq_all(200,0,200)",qqcuts);
	TH1F* heEcal_qq_all = (TH1F*) gDirectory->Get("heEcal_qq_all");
	cout << ("n_qq_all=") << nEvents_tt_all << endl;

//introduce cuts
	//TString eecuts="Ncharged<=5 && E_ecal>=75 && cos_theta>=-0.9 && cos_theta<=0.9";
	TTree *hee = (TTree*)files[0]->Get("h3");
	float nEvents_ee_all = hee->Draw("E_hcal>>heEcal_ee_all(40,0,20)","");
	TH1F* heEcal_ee_all = (TH1F*) gDirectory->Get("heEcal_ee_all");
	//cout << ("n_ee_all=") << nEvents_ee_all << endl;

	files[0]->cd(); //ee ntuple
	//introduce cuts
	TString eecuts="Ncharged<=5 && E_ecal>=75 && -0.9<cos_thet<=0.9";
	TTree *hee = (TTree*)files[0]->Get("h3");
	float nEvents_ee_all = hee->Draw("cos_thet>>heEcal_ee_all(50,-1,1)");
	cout << ("n_ee_all=") << nEvents_ee_all << endl;
}

