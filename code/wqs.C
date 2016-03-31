#include "TFile.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TString.h"
#include "TLegend.h"
#include <iostream>

void wqs(){
	double factors[7]={0.122601,0.37624,0.517685,0.688728,0.659251,0.445779,0.476775};
	double factors_err[7]={0.0221567,0.0224847,0.0228781,0.00811223,0.0208336,0.0301098,0.0256896};
	//Luminositaeten

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
	N[0][0]=510;
	N[0][1]=511;
	N[0][2]=491;
	N[0][3]=4354;
	N[0][4]=662;
	N[0][5]=300;
	N[0][6]=410;
	//m-events
	N[1][0]=137;
	N[1][1]=242;
	N[1][2]=332;
	N[1][3]=3905;
	N[1][4]=691;
	N[1][5]=269;
	N[1][6]=329;
	//t-events
	N[2][0]=137;
	N[2][1]=188;
	N[2][2]=237;
	N[2][3]=3251;
	N[2][4]=535;
	N[2][5]=245;
	N[2][6]=269;
	//q-events
	N[3][0]=3528;
	N[3][1]=5322;
	N[3][2]=7542;
	N[3][3]=92764;
	N[3][4]=15294;
	N[3][5]=6643;
	N[3][6]=7419;
	//invertierte effizienzmatrizen
	double ineps[4][4]= {
		{ 2.55823, 8.11301e-005, -0.0134764, -3.93271e-005 },
		{ -0.000506061, 1.10909, -0.00807654, -3.49376e-006 },
		{ -0.00119715, -0.0124972, 1.19233, -0.00116104 },
		{ -0.000184582, 7.48372e-005, -0.00827242, 1.01041 }
	};

//fehler auf ineps
	double ineps_err[4][4]={
		{ 0.0103981, 4.30681e-005, 0.000713902, 3.67883e-005 },
		{ 0.000123138, 0.00118701, 0.000363229, 1.11884e-005 },
		{ 0.000195327, 0.000414756, 0.00182158, 0.000118175 },
		{ 7.32518e-005, 1.2723e-005, 0.000349603, 0.000323775 }
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
		dat[i]->Draw("AP");
		//func -> Draw("SAME");
		r=dat[i] -> Fit("func","S","",88.48021,93.71841);
		r->Print("V");
		c->SaveAs("../results/data_results/wqs/WQ"+names[i]+".png");
		c->Close();
	}
}
