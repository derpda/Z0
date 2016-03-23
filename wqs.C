#include "TFile.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TString.h"
#include "TLegend.h"
#include <iostream>

void wqs{
	//Anzahl Ereignisse nach Schnit
	TVectorD N(4);
	N(0)=36000;//e-events
	N(1)=34000;//mu-events
	N(2)=32000;//t-events
	N(3)=31000;//q-events
	//invertierte effizienzmatrix, ineps[0][0] wird mit fs
	//korrigiert
	TMatrixD ineps(4,4);
	ineps(0,0)=1.019;
	ineps(0,1)=0.0002589;
	ineps(0,2)=-0.0118;
	ineps(0,3)=-0.0003524;
	ineps(1,0)=-0.0004819;
	ineps(1,1)=1.059;
	ineps(1,2)=-0.008628;
	ineps(1,3)=2.724e-006;
	ineps(2,0)=-0.003167;
	ineps(2,1)=-0.02594;
	ineps(2,2)=1.091;
	ineps(2,3)=-0.001675;
	ineps(3,0)=-5.002e-005;
	ineps(3,1)=0.0001696;
	ineps(3,2)=-0.007586;
	ineps(3,3)=1.01;
	//tatsächliche Ereignisse, Multiplikation mit eps^-1
	TVectorD Nk(4);
	Nk=ineps*N;
}
	
	
