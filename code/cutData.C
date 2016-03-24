#include <sstream>

cutData()
{
	gStyle->SetHistLineWidth(2.);
	gStyle->SetLabelSize(0.05, "x");
	gStyle->SetLabelSize(0.05, "y");
	gStyle->SetOptStat(0);
	gStyle->SetLabelFont(42, "x");
	gStyle->SetLabelFont(42, "y");


	TFile *data;
	data = new TFile("../daten/daten/daten_1.root", "READ");
	TTree *mc_trees;
	TTree *mc_trees = (TTree*) data->Get("h33");



	//Loop over invariant mass
	int i_im;
	const int n_invmass = 7;
	TString invmassname[n_invmass];
	invmassname[0]="44.2";
	invmassname[1]="44.7";
	invmassname[2]="45.1";
	invmassname[3]="45.6";
	invmassname[4]="46.0";
	invmassname[5]="46.5";
	invmassname[6]="46.9";


	TFile* f_out[n_invmass];
	f_out[0] = new TFile("data_graphs/histos44.2.root", "RECREATE");
	f_out[1] = new TFile("data_graphs/histos44.7.root", "RECREATE");
	f_out[2] = new TFile("data_graphs/histos45.1.root", "RECREATE");
	f_out[3] = new TFile("data_graphs/histos45.6.root", "RECREATE");
	f_out[4] = new TFile("data_graphs/histos46.0.root", "RECREATE");
	f_out[5] = new TFile("data_graphs/histos46.5.root", "RECREATE");
	f_out[6] = new TFile("data_graphs/histos46.9.root", "RECREATE");

	//create pointers to histograms
	TH1F *h_Ncharged[n_invmass];
	TH1F *h_Pcharged[n_invmass];
	TH1F *h_E_Ecal[n_invmass];
	TH1F *h_E_Hcal[n_invmass];
	TH1F *h_cos_thru[n_invmass];
	TH1F *h_cos_thet[n_invmass];
	TH1F *h_E_Lep[n_invmass];
	
	//create pointer to 2dim histogram
	TH2F *h_E_Ecal_vs_Pcharged[n_invmass];

	for (int i=0; i < n_invmass; ++i) {
	h_Ncharged[i] = new TH1F("h_Ncharged_" + invmassname[i], "Number of tracks", 40, 0., 40.);
	h_Ncharged[i]->Sumw2();
	h_Pcharged[i] = new TH1F("h_Pcharged_" + invmassname[i], "Sum of track energies", 120, 0., 120.);
	h_Pcharged[i]->Sumw2();
	h_E_Ecal[i] = new TH1F("h_Eecal_" + invmassname[i], "E_ECal", 60, 0., 120.);
	h_E_Ecal[i]->Sumw2();
	h_E_Hcal[i] = new TH1F("h_Ehcal_" + invmassname[i], "E_HCal", 50, 0., 50.);
	h_E_Hcal[i]->Sumw2();
	h_cos_thru[i] = new TH1F("h_costhru_" + invmassname[i], "Cos thrust", 40, -1., 1.);
	h_cos_thru[i]->Sumw2();
	h_cos_thet[i] = new TH1F("h_costhet_" + invmassname[i], "Cos theta", 50, -1., 1.);
	h_cos_thet[i]->Sumw2();
	h_E_Lep[i] = new TH1F("h_E_Lep_" + invmassname[i], "Lepton energy", 100, 44., 47.);
	h_E_Lep[i]->Sumw2();
	h_E_Ecal_vs_Pcharged[i] = new TH2F("h_E_Ecal_vs_Pcharged_" + invmassname[i], "E_Ecal vs Pcharged", 60, 0., 120., 120, 0., 120.);
	h_E_Ecal_vs_Pcharged[i]->Sumw2();
	}


	//Loop over the four different cuts as well no the no cut option
	const int n_cutregions = 5;
	TString cutname[n_cutregions];
	cutname[0]="nocut";
	cutname[1]="ee_cut";
	cutname[2]="mm_cut";
	cutname[3]="tt_cut";
	cutname[4]="qq_cut";

	int a_eventcount[n_invmass][n_cutregions]={{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},{0, 0, 0, 0, 0}};

	for(int i_cr = 0; i_cr < n_cutregions; ++i_cr) {
		//define parameters
		float event, run, Ncharged, Pcharged, E_ECal, E_HCal, E_LEP, cos_thru, cos_theta;


		//reset TH1F histograms
		for (int i=0; i < n_invmass; ++i) {
		h_Ncharged[i]->Reset("M");
		h_Pcharged[i]->Reset("M");
		h_E_Ecal[i]->Reset("M");
		h_E_Hcal[i]->Reset("M");
		h_cos_thru[i]->Reset("M");
		h_cos_thet[i]->Reset("M");
		h_E_Lep[i]->Reset("M");
		h_E_Ecal_vs_Pcharged[i]->Reset("M");
		}

		mc_trees->SetBranchAddress("event", &event);
		mc_trees->SetBranchAddress("run", &run);
		mc_trees->SetBranchAddress("Ncharged", &Ncharged);
		mc_trees->SetBranchAddress("Pcharged", &Pcharged);
		mc_trees->SetBranchAddress("E_ecal", &E_ECal);
		mc_trees->SetBranchAddress("E_hcal", &E_HCal);
		mc_trees->SetBranchAddress("E_lep", &E_LEP);
		mc_trees->SetBranchAddress("cos_thru", &cos_thru);
		mc_trees->SetBranchAddress("cos_thet", &cos_theta);
		const int nevents = mc_trees->GetEntries();

cout << "cut number: " << i_cr << endl;
		for(int iev = 0; iev < nevents; ++iev) {
			mc_trees->GetEntry(iev);

			switch(i_cr) {
				case 0:
				//no cuts
				//cout << iev << endl;
					//determine i_im
					if( E_LEP > 44.1 && E_LEP < 44.4 ) {i_im=0;
					}else if( E_LEP > 44.5 && E_LEP < 44.9 ) {i_im=1;
					}else if( E_LEP > 45.0 && E_LEP < 45.3 ) {i_im=2;
					}else if( E_LEP > 45.4 && E_LEP < 45.8 ) {i_im=3;
					}else if( E_LEP > 45.8 && E_LEP < 46.2 ) {i_im=4;
					}else if( E_LEP > 46.3 && E_LEP < 46.7 ) {i_im=5;
					}else if( E_LEP > 46.7 && E_LEP < 47.0 ) {i_im=6;
					}
					h_Ncharged[i_im]->Fill(Ncharged);
					h_Pcharged[i_im]->Fill(Pcharged);
					h_E_Ecal[i_im]->Fill(E_ECal);
					h_E_Hcal[i_im]->Fill(E_HCal);
					h_cos_thru[i_im]->Fill(cos_thru);
					h_cos_thet[i_im]->Fill(cos_theta);
					h_E_Lep[i_im]->Fill(E_LEP);
					h_E_Ecal_vs_Pcharged[i_im]->Fill(Pcharged, E_ECal);
					a_eventcount[i_im][i_cr] += 1;
					break;
				case 1:
				//ee cuts
					if (Ncharged < 7 && E_ECal >= 70 && Pcharged !=0 && ( (cos_theta > -0.9 && cos_theta < 0.9) || cos_theta > 1) ) {
					//cout << iev << endl;
					//determine i_im
						if( E_LEP > 44.1 && E_LEP < 44.4 ) {i_im=0;
						}else if( E_LEP > 44.5 && E_LEP < 44.9 ) {i_im=1;
						}else if( E_LEP > 45.0 && E_LEP < 45.3 ) {i_im=2;
						}else if( E_LEP > 45.4 && E_LEP < 45.8 ) {i_im=3;
						}else if( E_LEP > 45.8 && E_LEP < 46.2 ) {i_im=4;
						}else if( E_LEP > 46.3 && E_LEP < 46.7 ) {i_im=5;
						}else if( E_LEP > 46.7 && E_LEP < 47.0 ) {i_im=6;
						}
						h_Ncharged[i_im]->Fill(Ncharged);
						h_Pcharged[i_im]->Fill(Pcharged);
						h_E_Ecal[i_im]->Fill(E_ECal);
						h_E_Hcal[i_im]->Fill(E_HCal);
						h_cos_thru[i_im]->Fill(cos_thru);
						h_cos_thet[i_im]->Fill(cos_theta);
						h_E_Lep[i_im]->Fill(E_LEP);
						h_E_Ecal_vs_Pcharged[i_im]->Fill(Pcharged, E_ECal);
						a_eventcount[i_im][i_cr] += 1;
					}
					break;
				case 2:
				//mm cuts
					if ( Pcharged > 71 && Pcharged !=0 && E_ECal < 50 && Ncharged ==2) {
					//cout << iev << endl;
						//determine i_im
						if( E_LEP > 44.1 && E_LEP < 44.4 ) {i_im=0;
						}else if( E_LEP > 44.5 && E_LEP < 44.9 ) {i_im=1;
						}else if( E_LEP > 45.0 && E_LEP < 45.3 ) {i_im=2;
						}else if( E_LEP > 45.4 && E_LEP < 45.8 ) {i_im=3;
						}else if( E_LEP > 45.8 && E_LEP < 46.2 ) {i_im=4;
						}else if( E_LEP > 46.3 && E_LEP < 46.7 ) {i_im=5;
						}else if( E_LEP > 46.7 && E_LEP < 47.0 ) {i_im=6;
						}
						h_Ncharged[i_im]->Fill(Ncharged);
						h_Pcharged[i_im]->Fill(Pcharged);
						h_E_Ecal[i_im]->Fill(E_ECal);
						h_E_Hcal[i_im]->Fill(E_HCal);
						h_cos_thru[i_im]->Fill(cos_thru);
						h_cos_thet[i_im]->Fill(cos_theta);
						h_E_Lep[i_im]->Fill(E_LEP);
						h_E_Ecal_vs_Pcharged[i_im]->Fill(Pcharged, E_ECal);
						a_eventcount[i_im][i_cr] += 1;
					} 
					break;
				case 3:
				//tt cuts
					if ( Pcharged != 0 && Pcharged <= 71 && Ncharged <7 && E_ECal < 75 && ( (cos_theta > -0.9 && cos_theta < 0.9) || cos_theta > 1) 
					( cos_thru > -0.9 && cos_thru < 0.9) ) {
					//cout << iev << endl;
						//determine i_im
						if( E_LEP > 44.1 && E_LEP < 44.4 ) {i_im=0;
						}else if( E_LEP > 44.5 && E_LEP < 44.9 ) {i_im=1;
						}else if( E_LEP > 45.0 && E_LEP < 45.3 ) {i_im=2;
						}else if( E_LEP > 45.4 && E_LEP < 45.8 ) {i_im=3;
						}else if( E_LEP > 45.8 && E_LEP < 46.2 ) {i_im=4;
						}else if( E_LEP > 46.3 && E_LEP < 46.7 ) {i_im=5;
						}else if( E_LEP > 46.7 && E_LEP < 47.0 ) {i_im=6;
						}
						h_Ncharged[i_im]->Fill(Ncharged);
						h_Pcharged[i_im]->Fill(Pcharged);
						h_E_Ecal[i_im]->Fill(E_ECal);
						h_E_Hcal[i_im]->Fill(E_HCal);
						h_cos_thru[i_im]->Fill(cos_thru);
						h_cos_thet[i_im]->Fill(cos_theta); 
						h_E_Lep[i_im]->Fill(E_LEP);
						h_E_Ecal_vs_Pcharged[i_im]->Fill(Pcharged, E_ECal);
						a_eventcount[i_im][i_cr] += 1;
					}
					break;
				case 4:
				//qq cuts
					if (Ncharged >= 8 && Pcharged != 0 ) {
					//cout << iev << endl;
						//determine i_im
						if( E_LEP > 44.1 && E_LEP < 44.4 ) {i_im=0;
						}else if( E_LEP > 44.5 && E_LEP < 44.9 ) {i_im=1;
						}else if( E_LEP > 45.0 && E_LEP < 45.3 ) {i_im=2;
						}else if( E_LEP > 45.4 && E_LEP < 45.8 ) {i_im=3;
						}else if( E_LEP > 45.8 && E_LEP < 46.2 ) {i_im=4;
						}else if( E_LEP > 46.3 && E_LEP < 46.7 ) {i_im=5;
						}else if( E_LEP > 46.7 && E_LEP < 47.0 ) {i_im=6;
						}
						h_Ncharged[i_im]->Fill(Ncharged);
						h_Pcharged[i_im]->Fill(Pcharged);
						h_E_Ecal[i_im]->Fill(E_ECal);
						h_E_Hcal[i_im]->Fill(E_HCal);
						h_cos_thru[i_im]->Fill(cos_thru);
						h_cos_thet[i_im]->Fill(cos_theta);
						h_E_Lep[i_im]->Fill(E_LEP);
						h_E_Ecal_vs_Pcharged[i_im]->Fill(Pcharged, E_ECal);
						a_eventcount[i_im][i_cr] += 1;
					}
					break;
				case default:
				break;
			}//end of switch
		}//end of events loop


		//loop to save graphs for each i_im
		for (int i_im=0; i_im < n_invmass; ++i_im) {

			TCanvas *c[n_invmass+1];
			c[0] = new TCanvas("c0", "c0",1920,1080);
			c[1] = new TCanvas("c1", "c1",1920,1080);
			c[2] = new TCanvas("c2", "c2",1920,1080);
			c[3] = new TCanvas("c3", "c3",1920,1080);
			c[4] = new TCanvas("c4", "c4",1920,1080);
			c[5] = new TCanvas("c5", "c5",1920,1080);
			c[6] = new TCanvas("c6", "c6",1920,1080);
			c[7] = new TCanvas("c7", "c7",1920,1080);
			
			c[0]->cd();
			h_Ncharged[i_im]->Draw("HIST");
			c[0]->SaveAs("data_graphs/" + invmassname[i_im] + "/" + cutname[i_cr] +  "_Ncharged.png");
			c[0]->Close();

			c[1]->cd();
			h_Pcharged[i_im]->Draw("HIST");
			c[1]->SaveAs("data_graphs/" + invmassname[i_im] + "/" + cutname[i_cr] +  "_Pcharged.png");
			c[1]->Close();

			c[2]->cd();
			h_E_Ecal[i_im]->Draw("HIST");
			c[2]->SaveAs("data_graphs/" + invmassname[i_im] + "/" + cutname[i_cr] +  "_E_Ecal.png");
			c[2]->Close();

			c[3]->cd();
			h_E_Hcal[i_im]->Draw("HIST");
			c[3]->SaveAs("data_graphs/" + invmassname[i_im] + "/" + cutname[i_cr] +  "_E_Hcal.png");
			c[3]->Close();

			c[4]->cd();
			h_cos_thru[i_im]->Draw("HIST");
			c[4]->SaveAs("data_graphs/" + invmassname[i_im] + "/" + cutname[i_cr] +  "_cos_thru.png");
			c[4]->Close();

			c[5]->cd();
			h_cos_thet[i_im]->Draw("HIST");
			c[5]->SaveAs("data_graphs/" + invmassname[i_im] + "/" + cutname[i_cr] +  "_cos_theta.png");
			c[5]->Close();

			c[6]->cd();
			h_E_Lep[i_im]->Draw("HIST");
			c[6]->SaveAs("data_graphs/" + invmassname[i_im] + "/" + cutname[i_cr] +  "_E_Lep.png");
			c[6]->Close();

			c[7]->cd();
			h_E_Ecal_vs_Pcharged[i_im]->Draw("HIST COLZ");
			h_E_Ecal_vs_Pcharged[i_im]->GetXaxis()->SetTitle("Pcharged");
			h_E_Ecal_vs_Pcharged[i_im]->GetYaxis()->SetTitle("E_Ecal");
			h_E_Ecal_vs_Pcharged[i_im]->Draw("HIST COLZ");
			c[7]->SaveAs("data_graphs/" + invmassname[i_im] + "/" + cutname[i_cr] +  "_E_Ecal_vs_Pcharged.png");
			c[7]->Close();
		}//end of save graphics loop


		//write ee-cut histogram
		if(i_cr==1) {
			for (int s=0; s<n_invmass; ++s) {
				f_out[s]->WriteTObject(h_cos_thet[s]);
			}
		}

	} //end of cut region loop


	cout << "Event counts of the cuts at different invariant masses" << endl;
	cout << "\t" << cutname[0] << "\t" << cutname[1] << cutname[2] << "\t" << cutname[3] << "\t" << cutname[4] << "\t" << endl;
	for(int i=0; i < n_invmass; ++i){
		cout << invmassname[i] << "\t" << a_eventcount[i][0] << "\t" << a_eventcount[i][1] << "\t" << 
			a_eventcount[i][2] << "\t" << a_eventcount[i][3] << "\t" << a_eventcount[i][4] << endl;
	}

	for(int i=0; i < n_invmass; ++i) {
		f_out[i]->Close();
	}
	//close opened files
	data->Close();
}
