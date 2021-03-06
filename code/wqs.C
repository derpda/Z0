#include "TFile.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TString.h"
#include "TLegend.h"
#include <iostream>
#include <myfunctions.h>
#include <string>

void wqs(){
	double factors[7]={0.118084,0.371194,0.517685,0.688433,0.656156,0.438832,0.473246};
	double factors_err[7]={0.022072,0.0225952,0.0228781,0.00812138,0.0208721,0.0301878,0.0257884 };
	//Luminositaeten

	//Plot styling settings
	gStyle->SetLabelSize(0.05, "x");
	gStyle->SetLabelSize(0.05, "y");
	gStyle->SetOptStat(0);
	gStyle->SetLabelFont(42, "x");
	gStyle->SetLabelFont(42, "y");
	gStyle->SetOptTitle(0);
	gStyle->SetOptFit(111);
	gStyle->SetOptStat(0);

	//create histogram objects
	TLegend leg[4];
	leg[0] = new TLegend(0.62, 0.62, 1.0, 1.0);
	leg[1] = new TLegend(0.62, 0.62, 1.0, 1.0);
	leg[2] = new TLegend(0.62, 0.62, 1.0, 1.0);
	leg[3] = new TLegend(0.62, 0.62, 1.0, 1.0);

	double L[7];
	L[0]=675.8590;
	L[1]=543.6270;
	L[2]=419.7760;
	L[3]=3122.204;
	L[4]=639.8380;
	L[5]=479.2400;
	L[6]=766.8380;
	double L_err[7]={5.721257,4.830643,3.974844,22.31760,5.577354,4.481870,6.497519};
	//Schwerpunktsenergien
	double S[7]={88.48021,89.47158,90.22720,91.23223,91.97109,92.97091,93.71841};
	//Strahlungskorrekturen
	double wq_cor_lep[7]={0.09,0.2,0.36,0.52,0.22,-0.01,-0.08};//leptonisch
	double wq_cor_had[7]={2.0,4.3,7.7,10.8,4.7,-0.2,-1.6};//hadronisch
	//Eventnamen
	TString names[4]={"ee","mm","TT","qq"};
	//Anzahl Ereignisse nach Schnitt,
	//sortiert nach Schwerpunktsenergie
	double N[4][7];
	//e-events
	N[0][0]=508;
	N[0][1]=505;
	N[0][2]=491;
	N[0][3]=4334;
	N[0][4]=658;
	N[0][5]=299;
	N[0][6]=406;
	//m-events
	N[1][0]=137;
	N[1][1]=242;
	N[1][2]=332;
	N[1][3]=3905;
	N[1][4]=691;
	N[1][5]=269;
	N[1][6]=329;
	//t-events
	N[2][0]=134;
	N[2][1]=187;
	N[2][2]=234;
	N[2][3]=3227;
	N[2][4]=528;
	N[2][5]=242;
	N[2][6]=269;
	//q-events
	N[3][0]=3514;
	N[3][1]=5303;
	N[3][2]=7496;
	N[3][3]=92293;
	N[3][4]=15214;
	N[3][5]=6609;
	N[3][6]=7380;
	//invertierte effizienzmatrizen
	double ineps[4][4]= {
		{ 2.55899, 7.46841e-005, -0.0129356, -4.03522e-005 },
		{ -0.000506167, 1.10909, -0.00812152, -3.63607e-006 },
		{ -0.00120419, -0.0124961, 1.19896, -0.0011491 },
		{ -0.000185608, 7.50761e-005, -0.00834814, 1.01589 }
	};

//fehler auf ineps
	double ineps_err[4][4]= {
	{ 0.0103718, 4.31217e-005, 0.000702587, 3.70456e-005 },
	{ 0.000123171, 0.00118703, 0.000365665, 1.13137e-005 },
	{ 0.000195936, 0.000416375, 0.00187033, 0.000118901 },
	{ 7.35519e-005, 1.27943e-005, 0.000352797, 0.000402688 }
	};
	cout << ineps_err[2][2]<<endl;

	//Graphen
	TGraphErrors *dat[4];
	//Graphen Namen
	//Fitfunktion(Leptonen)
	TF1* func = new TF1("func","389105.0583657588*12*3.14159265358979323846*[2]**2*x**2/([0]**2*((x**2-[0]**2)**2+(x**4*[1]**2)/[0]**2))",88.45,94.0);
	//tatsächliche Ereignisse, Multiplikation mit ineps
	double Nk[4][7];
	double Nk_var[4][7]; //Varianz(s^2) von Nk
	double wq[7];//7 Wq pro Zerfallsart
	double wq_err[7];
	gStyle->SetOptFit(1111);
	//Canvas seiten
	Double_t w = 1000;
	Double_t h = 618;
	TFitResultPtr r;
	//i-te Zerfallsart
	for(int i=0; i<4;i++){
		//für jede Schwerpunktsenergie j
		for(int j=0; j<7;j++){
			Nk[i][j]=0;
			Nk_var[i][j]=0;
			for(int l=0; l<4;l++){
				Nk[i][j]+=ineps[i][l]*N[l][j];
				Nk_var[i][j]+=ineps[i][l]**2*N[l][j]+(N[l][j]*ineps_err[i][l])**2;
				if(i==0 && l==0){
					Nk[i][j]*=factors[j];
					Nk_var[i][j]+=(ineps[i][l]*N[l][j]*factors_err[j])**2;
				}
			}
			if(i<3){wq[j]=Nk[i][j]/L[j]+wq_cor_lep[j];}
			else{wq[j]=Nk[i][j]/L[j]+wq_cor_had[j];}
			wq_err[j]=wq[j]*sqrt(Nk_var[i][j]/Nk[i][j]**2+(L_err[j]/L[j])**2);
		}
		dat[i]= new TGraphErrors(7,S,wq,0,wq_err);
		func -> SetParameters(91.187,2.4862,0.0838);
		func -> SetParLimits(1,0,10);
		dat[i]->SetMarkerSize(1.0);
		dat[i]->SetMarkerStyle(5);
		TCanvas * c1 = new TCanvas("c", "c", w, h);
		c->SetWindowSize(w + (w - c->GetWw()), h + (h - c->GetWh()));

		//func -> Draw("SAME");
		r=dat[i]->Fit("func","S","",88.48021,93.71841);
		r->Print("V");
		//set graph axis labels
		TF1 * f = dat[i]->GetFunction("func");
		Double_t chi2 = f->GetChisquare();
		Double_t mZ = f->GetParameter(0);
		Double_t e_mZ = f->GetParError(0);
		Double_t GZ = f->GetParameter(1);
		Double_t e_GZ = f->GetParError(1);
		Double_t Gf = f->GetParameter(2);
		Double_t e_Gf = f->GetParError(2);
		cout << "chi2 of cut " << i + 1 << ": " << chi2 << endl;
		cout << "mZ of cut " << i + 1 << ": " << mZ << "+-" << e_mZ << endl;
		cout << "GZ of cut " << i + 1 << ": " << GZ << "+-" << e_GZ << endl;
		cout << "Gf of cut " << i + 1 << ": " << Gf << "+-" << e_Gf << endl;
		graphstyle(dat[i], "Center-of-mass energy [GeV]", "Cross section #sigma [nb]");
		dat[i]->Draw("AP");
		TLegendEntry *le = leg[i]->AddEntry(f, "#\\frac{12\\pi}{M^2_Z}\\frac{s\\Gamma_f^2}{(s-M_Z^2)^2+s^2\\Gamma_Z^2/M_Z^2}","l");
		//le->SetTextSize(0.032);
		if (i == 0) {
			leg[i]->AddEntry((TObject*)0, "#\\chi^2/DoF=4.7","");
			leg[i]->AddEntry((TObject*)0, "#M_Z=91.10\\pm0.06\\quad GeV","");
			leg[i]->AddEntry((TObject*)0, "#\\Gamma_Z=2.28\\pm0.11\\quad GeV","");
			leg[i]->AddEntry((TObject*)0, "#\\Gamma_f=0.094\\pm0.004\\quad GeV","");
		}
		if (i == 1) {
			leg[i]->AddEntry((TObject*)0, "#\\chi^2/DoF=2.3", "");
			leg[i]->AddEntry((TObject*)0, "#M_Z=91.20\\pm0.03\\quad GeV", "");
			leg[i]->AddEntry((TObject*)0, "#\\Gamma_Z=2.52\\pm0.06\\quad GeV", "");
			leg[i]->AddEntry((TObject*)0, "#\\Gamma_f=0.0829\\pm0.0017\\quad GeV", "");
		}
		if (i == 2) {
			leg[i]->AddEntry((TObject*)0, "#\\chi^2/DoF=3.1", "");
			leg[i]->AddEntry((TObject*)0, "#M_Z=91.18\\pm0.04\\quad GeV", "");
			leg[i]->AddEntry((TObject*)0, "#\\Gamma_Z=2.48\\pm0.07\\quad GeV", "");
			leg[i]->AddEntry((TObject*)0, "#\\Gamma_f=0.0769\\pm0.0019\\quad GeV", "");
		}
		if (i == 3) {
			leg[i]->AddEntry((TObject*)0, "#\\chi^2/DoF=3.1", "");
			leg[i]->AddEntry((TObject*)0, "#M_Z=91.19\\pm0.09\\quad GeV", "");
			leg[i]->AddEntry((TObject*)0, "#\\Gamma_Z=2.526\\pm0.019\\quad GeV", "");
			leg[i]->AddEntry((TObject*)0, "#\\Gamma_f=0.3839\\pm0.0021\\quad GeV", "");
		}
		leg[i]->Draw();
		c->SaveAs("../results/data_results/wqs/WQ"+names[i]+".png");
		c->Close();
	}
}

