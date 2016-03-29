#include "TMatrixD.h"
#include <iostream>
#include <algorithm>
#include <vector>	
#include <sstream>
using namespace std;

void useMatrixAll(){
	//Filling a matrix

	const int n_invmass=7;
	float factors[n_invmass]={0.583014, 0.664385, 0.684967, 0.701125, 0.589088, 0.459561, 0.308238};
	TString invmassname[n_invmass];
	invmassname[0]="44.2";
	invmassname[1]="44.7";
	invmassname[2]="45.1";
	invmassname[3]="45.6";
	invmassname[4]="46.0";
	invmassname[5]="46.5";
	invmassname[6]="46.9";



	//for loop over entire file from here on out
	for(int i_im = 0; i_im < n_invmass; ++i_im) {

	ofstream matrixprint[n_invmass];
	//FOR BENNI: EDIT FILE PATHS
	matrixprint[i_im].open("matrix\\matrices_" + invmassname[i_im] + ".txt");
	

	TMatrixD matrix(4,4);
	matrix(0,0)=0.48668*factors[i_im];
	matrix(0,1)=0.00002;
	matrix(0,2)=0.00988;
	matrix(0,3)=0.00036;
	matrix(1,0)=0.00018;
	matrix(1,1)=0.90171;
	matrix(1,2)=0.00611;
	matrix(1,3)=0.00001;
	matrix(2,0)=0.00301;
	matrix(2,1)=0.02421;
	matrix(2,2)=0.91962;
	matrix(2,3)=0.00153;
	matrix(3,0)=0.00007;
	matrix(3,1)=0.00001;
	matrix(3,2)=0.00687;
	matrix(3,3)=0.98970;

	matrixprint[i_im] <<"Original matrix\n" << endl;
	//print original matrix
	matrixprint[i_im] << "{";
	for(int i=0; i<4; ++i){
		matrixprint[i_im] << "{";
		for(int j=0; j<3; ++j) {
			if(j<3) {matrixprint[i_im] << matrix[i][j] <<", ";
			}
			else { matrixprint[i_im] << matrix[i][j];
			}
		}
		if(i<3) {matrixprint[i_im] << matrix[i][3] << "}, ";
		}
		else {matrixprint[i_im] << matrix[i][3] << "}";
		}
	}
	matrixprint[i_im] << "}\n" << endl;

	//Total Number of simulated events per kind
	double N[4];
	N[0]=93802.;
	N[1]=94381.;
	N[2]=79214.;
	N[3]=98563.;
	
	double mean[4][4];
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			mean[i][j]=matrix(i,j);
		}
	}

	matrixprint[i_im]<<"\n\nBinomial error matrix\n"<< endl;
	//binomial error
	double err[4][4];

	//calculate and print binomial error matrix to file
	matrixprint[i_im] << "{";
	for(int i=0; i<4; ++i){
		matrixprint[i_im] << "{";
		for(int j=0; j<3; ++j) {
			err[i][j]=sqrt(mean[i][j]*(1.0-mean[i][j])/N[j]);
			if(j<3) {matrixprint[i_im] << err[i][j] <<", ";
			}
			else { matrixprint[i_im] << err[i][j];
			}
		}
		if(i<3) {matrixprint[i_im] << err[i][3] << "}, ";
		}
		else {matrixprint[i_im] << err[i][3] << "}";
		}
	}
	matrixprint[i_im] << "}\n" << endl;

	//Inverting the matrix 
	TMatrixD Inverse(4,4);
	Inverse = matrix.Invert();
	matrixprint[i_im]<<"\n\nInverse matrix\n"<< endl;

	//print inverse matrix to file
	matrixprint[i_im] << "{";
	for(int i=0; i<4; ++i){
		matrixprint[i_im] << "{";
		for(int j=0; j<3; ++j) {
			if(j<3) {matrixprint[i_im] << Inverse[i][j] <<", ";
			}
			else { matrixprint[i_im] << Inverse[i][j];
			}
		}
		if(i<3) {matrixprint[i_im] << Inverse[i][3] << "}, ";
		}
		else {matrixprint[i_im] << Inverse[i][3] << "}";
		}
	}
	matrixprint[i_im] << "}\n" << endl;


	TRandom3 *r = new TRandom3();
	int ntoy = 10000;
	
	vector<float> eps_00;
	vector<float> eps_01;
	vector<float> eps_02;
	vector<float> eps_03;
	vector<float> eps_10;
	vector<float> eps_11;
	vector<float> eps_12;
	vector<float> eps_13;
	vector<float> eps_20;
	vector<float> eps_21;
	vector<float> eps_22;
	vector<float> eps_23;
	vector<float> eps_30;
	vector<float> eps_31;
	vector<float> eps_32;
	vector<float> eps_33;
	
	//vector<vector<vector<float> > > epsilons;

	for(int i=0;i<ntoy;i++){
		TMatrixD matrix_toy(4,4);
		for(int j=0;j<4;j++){
			for(int k=0;k<4;k++){
				matrix_toy(j,k)=r->Gaus(mean[j][k],err[j][k]);
			}
		}

		TMatrixD Inverse_toy(4,4);
		Inverse_toy = matrix_toy.Invert();
		/*
		for(int i=0; i<4; ++i) {
			vector<vector<float> > eps_v;
			for(int j=0; j<4; ++j) {
				vector<float> eps;
				epsilons.push_back(eps);
			}
			eps_v.push_back(
		}
		*/
		eps_00.push_back(Inverse_toy(0, 0));
		eps_01.push_back(Inverse_toy(0, 1));
		eps_02.push_back(Inverse_toy(0, 2));
		eps_03.push_back(Inverse_toy(0, 3));
		eps_10.push_back(Inverse_toy(1, 0));
		eps_11.push_back(Inverse_toy(1, 1));
		eps_12.push_back(Inverse_toy(1, 2));
		eps_13.push_back(Inverse_toy(1, 3));
		eps_20.push_back(Inverse_toy(2, 0));
		eps_21.push_back(Inverse_toy(2, 1));
		eps_22.push_back(Inverse_toy(2, 2));
		eps_23.push_back(Inverse_toy(2, 3));
		eps_30.push_back(Inverse_toy(3, 0));
		eps_31.push_back(Inverse_toy(3, 1));
		eps_32.push_back(Inverse_toy(3, 2));
		eps_33.push_back(Inverse_toy(3, 3));
		
		
		//for(int j=0;j<4;j++){
		 // for(int k=0;k<4;k++){
	//histo[j][k]->Fill(Inverse_toy(j,k));
				
			//}
		//}
		//float min_val[4][4], max_val[4][4];
		//min_val[0][1] = std::min_element(eps_00.begin(), eps_00.end());
		//cout << min_val[0][1]<<endl;
	}

	float min_val[4][4], max_val[4][4];
	min_val[0][0] = std::min_element(eps_00.begin(), eps_00.end());
	max_val[0][0] = std::max_element(eps_00.begin(), eps_00.end());
	min_val[0][1] = std::min_element(eps_01.begin(), eps_01.end());
	max_val[0][1] = std::max_element(eps_01.begin(), eps_01.end());
	min_val[0][2] = std::min_element(eps_02.begin(), eps_02.end());
	max_val[0][2] = std::max_element(eps_02.begin(), eps_02.end());
	min_val[0][3] = std::min_element(eps_03.begin(), eps_03.end());
	max_val[0][3] = std::max_element(eps_03.begin(), eps_03.end());
	min_val[1][0] = std::min_element(eps_10.begin(), eps_10.end());
	max_val[1][0] = std::max_element(eps_10.begin(), eps_10.end());
	min_val[1][1] = std::min_element(eps_11.begin(), eps_11.end());
	max_val[1][1] = std::max_element(eps_11.begin(), eps_11.end());
	min_val[1][2] = std::min_element(eps_12.begin(), eps_12.end());
	max_val[1][2] = std::max_element(eps_12.begin(), eps_12.end());
	min_val[1][3] = std::min_element(eps_13.begin(), eps_13.end());
	max_val[1][3] = std::max_element(eps_13.begin(), eps_13.end());
	min_val[2][0] = std::min_element(eps_20.begin(), eps_20.end());
	max_val[2][0] = std::max_element(eps_20.begin(), eps_20.end());
	min_val[2][1] = std::min_element(eps_21.begin(), eps_21.end());
	max_val[2][1] = std::max_element(eps_21.begin(), eps_21.end());
	min_val[2][2] = std::min_element(eps_22.begin(), eps_22.end());
	max_val[2][2] = std::max_element(eps_22.begin(), eps_22.end());
	min_val[2][3] = std::min_element(eps_23.begin(), eps_23.end());
	max_val[2][3] = std::max_element(eps_23.begin(), eps_23.end());
	min_val[3][0] = std::min_element(eps_30.begin(), eps_30.end());
	max_val[3][0] = std::max_element(eps_30.begin(), eps_30.end());
	min_val[3][1] = std::min_element(eps_31.begin(), eps_31.end());
	max_val[3][1] = std::max_element(eps_31.begin(), eps_31.end());
	min_val[3][2] = std::min_element(eps_32.begin(), eps_32.end());
	max_val[3][2] = std::max_element(eps_32.begin(), eps_32.end());
	min_val[3][3] = std::min_element(eps_33.begin(), eps_33.end());
	max_val[3][3] = std::max_element(eps_33.begin(), eps_33.end());
	
	TH1F* histo[4][4];
	histo[0][0] = new TH1F("h00","h00",50,min_val[0][0], max_val[0][0]);
	histo[0][1] = new TH1F("h01","h01",50,min_val[0][1], max_val[0][1]);
	histo[0][2] = new TH1F("h02","h02",50,min_val[0][2], max_val[0][2]);
	histo[0][3] = new TH1F("h03","h03",50,min_val[0][3], max_val[0][3]);
	histo[1][0] = new TH1F("h10","h10",50,min_val[1][0], max_val[1][0]);
	histo[1][1] = new TH1F("h11","h11",50,min_val[1][1], max_val[1][1]);
	histo[1][2] = new TH1F("h12","h12",50,min_val[1][2], max_val[1][2]);
	histo[1][3] = new TH1F("h13","h13",50,min_val[1][3], max_val[1][3]);
	histo[2][0] = new TH1F("h20","h20",50,min_val[2][0], max_val[2][0]);
	histo[2][1] = new TH1F("h21","h21",50,min_val[2][1], max_val[2][1]);
	histo[2][2] = new TH1F("h22","h22",50,min_val[2][2], max_val[2][2]);
	histo[2][3] = new TH1F("h23","h23",50,min_val[2][3], max_val[2][3]);
	histo[3][0] = new TH1F("h30","h30",50,min_val[3][0], max_val[3][0]);
	histo[3][1] = new TH1F("h31","h31",50,min_val[3][1], max_val[3][1]);
	histo[3][2] = new TH1F("h32","h32",50,min_val[3][2], max_val[3][2]);
	histo[3][3] = new TH1F("h33","h33",50,min_val[3][3], max_val[3][3]);
	
	for(int t=0; t<ntoy; ++t) {
		histo[0][0]->Fill(eps_00.at(t));
		histo[0][1]->Fill(eps_01.at(t));
		histo[0][2]->Fill(eps_02.at(t));
		histo[0][3]->Fill(eps_03.at(t));
		histo[1][0]->Fill(eps_10.at(t));
		histo[1][1]->Fill(eps_11.at(t));
		histo[1][2]->Fill(eps_12.at(t));
		histo[1][3]->Fill(eps_13.at(t));
		histo[2][0]->Fill(eps_20.at(t));
		histo[2][1]->Fill(eps_21.at(t));
		histo[2][2]->Fill(eps_22.at(t));
		histo[2][3]->Fill(eps_23.at(t));
		histo[3][0]->Fill(eps_30.at(t));
		histo[3][1]->Fill(eps_31.at(t));
		histo[3][2]->Fill(eps_32.at(t));
		histo[3][3]->Fill(eps_33.at(t));
	}
	
	double inverse_err[4][4];
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			TString name = "c";
			name+=i;
			name+=j;
			TCanvas *c = new TCanvas(name,name,800,700);
			c->cd();
			histo[i][j]->Draw();
			histo[i][j]->Fit("gaus");
			TF1 *fit = histo[i][j]->GetFunction("gaus");
			inverse_err[i][j] = fit->GetParameter(2);
			c->Close();
		}
	}
	
	matrixprint[i_im]<<"\n\nErrormatrix for inverse matrix\n"<<endl;
	matrixprint[i_im] << "{";
	for(int i=0; i<4; ++i){
		matrixprint[i_im] << "{";
		for(int j=0; j<3; ++j) {
			err[i][j]=sqrt(mean[i][j]*(1.0-mean[i][j])/N[j]);
			if(j<3) {matrixprint[i_im] << inverse_err[i][j] <<", ";
			}
			else { matrixprint[i_im] << inverse_err[i][j];
			}
		}
		if(i<3) {matrixprint[i_im] << inverse_err[i][3] << "}, ";
		}
		else {matrixprint[i_im] << inverse_err[i][3] << "}";
		}
	}
}//end of for loop over invariant masses
}
