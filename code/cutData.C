#include <sstream>
#include <myfunctions.h>

void cutData()
{
	gStyle->SetHistLineWidth(2.);
	gStyle->SetLabelSize(0.05, "x");
	gStyle->SetLabelSize(0.05, "y");
	gStyle->SetOptStat(0);
	gStyle->SetLabelFont(42, "x");
	gStyle->SetLabelFont(42, "y");
	gStyle->SetOptTitle(0);



	TFile *data;
	data = new TFile("../daten/daten/daten_1.root", "READ");
	TTree *mc_trees;
	TTree *mc_trees = (TTree*)data->Get("h33");




	int i_im;
	const int n_invmass = 7;
	TString invmassname[n_invmass];
	invmassname[0] = "44.2";
	invmassname[1] = "44.7";
	invmassname[2] = "45.1";
	invmassname[3] = "45.6";
	invmassname[4] = "46.0";
	invmassname[5] = "46.5";
	invmassname[6] = "46.9";

	TFile* f_out[n_invmass];
	f_out[0] = new TFile("../results/data_results/histos44.2.root", "RECREATE");
	f_out[1] = new TFile("../results/data_results/histos44.7.root", "RECREATE");
	f_out[2] = new TFile("../results/data_results/histos45.1.root", "RECREATE");
	f_out[3] = new TFile("../results/data_results/histos45.6.root", "RECREATE");
	f_out[4] = new TFile("../results/data_results/histos46.0.root", "RECREATE");
	f_out[5] = new TFile("../results/data_results/histos46.5.root", "RECREATE");
	f_out[6] = new TFile("../results/data_results/histos46.9.root", "RECREATE");

	//create pointers to histograms
	TH1F *h_Ncharged[n_invmass];
	TH1F *h_Pcharged[n_invmass];
	TH1F *h_E_Ecal[n_invmass];
	TH1F *h_E_Hcal[n_invmass];
	TH1F *h_cos_thru[n_invmass];
	TH1F *h_cos_thet[n_invmass];
	TH1F *h_E_Lep[n_invmass + 1];

	//create pointer to 2dim histogram
	TH2F *h_E_Ecal_vs_Pcharged[n_invmass];

	for (int i = 0; i < n_invmass; ++i) {
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
		h_cos_thet[i] = new TH1F("h_costhet_" + invmassname[i], "Cos theta", 100, -0.9, 0.9);
		h_cos_thet[i]->Sumw2();
		h_E_Lep[i] = new TH1F("h_E_Lep_" + invmassname[i], "Lepton energy", 100, 44., 47.);
		h_E_Lep[i]->Sumw2();
		h_E_Ecal_vs_Pcharged[i] = new TH2F("h_E_Ecal_vs_Pcharged_" + invmassname[i], "E_Ecal vs Pcharged", 60, 0., 120., 120, 0., 120.);
		h_E_Ecal_vs_Pcharged[i]->Sumw2();
	}
	//extra histogram to include all lepton energies
	h_E_Lep[7] = new TH1F("h_E_Lep", "Lepton energy", 100, 44., 47.);
	h_E_Lep[7]->Sumw2();

	//Loop over the four different cuts as well no the no cut option
	const int n_cutregions = 5;
	TString cutname[n_cutregions];
	cutname[0] = "nocut";
	cutname[1] = "ee_cut";
	cutname[2] = "mm_cut";
	cutname[3] = "tt_cut";
	cutname[4] = "qq_cut";

	//event counters
	int a_eventcount[n_invmass][n_cutregions] = { {0, 0, 0, 0, 0},{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},{0, 0, 0, 0, 0} };
	int forward_count[7] = { 0,0,0,0,0,0,0 };
	int backward_count[7] = { 0,0,0,0,0,0,0 };

	for (int i_cr = 0; i_cr < n_cutregions; ++i_cr) {
		//define parameters
		float event, run, Ncharged, Pcharged, E_ECal, E_HCal, E_LEP, cos_thru, cos_theta;

		//reset TH1F histograms
		for (int i = 0; i < n_invmass; ++i) {
			h_Ncharged[i]->Reset("M");
			h_Pcharged[i]->Reset("M");
			h_E_Ecal[i]->Reset("M");
			h_E_Hcal[i]->Reset("M");
			h_cos_thru[i]->Reset("M");
			h_cos_thet[i]->Reset("M");
			h_E_Lep[i]->Reset("M");
			h_E_Ecal_vs_Pcharged[i]->Reset("M");
		}
		h_E_Lep[7]->Reset();

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
		for (int iev = 0; iev < nevents; ++iev) {
			mc_trees->GetEntry(iev);

			switch (i_cr) {
			case 0:
				//no cuts
				//cout << iev << endl;
					//determine i_im
				if (E_LEP > 44.1 && E_LEP < 44.4) {
					i_im = 0;
				}
				else if (E_LEP > 44.5 && E_LEP < 44.9) {
					i_im = 1;
				}
				else if (E_LEP > 45.0 && E_LEP < 45.3) {
					i_im = 2;
				}
				else if (E_LEP > 45.4 && E_LEP < 45.8) {
					i_im = 3;
				}
				else if (E_LEP > 45.8 && E_LEP < 46.2) {
					i_im = 4;
				}
				else if (E_LEP > 46.3 && E_LEP < 46.7) {
					i_im = 5;
				}
				else if (E_LEP > 46.7 && E_LEP < 47.0) {
					i_im = 6;
				}
				h_Ncharged[i_im]->Fill(Ncharged);
				h_Pcharged[i_im]->Fill(Pcharged);
				h_E_Ecal[i_im]->Fill(E_ECal);
				h_E_Hcal[i_im]->Fill(E_HCal);
				h_cos_thru[i_im]->Fill(cos_thru);
				h_cos_thet[i_im]->Fill(cos_theta);
				h_E_Lep[i_im]->Fill(E_LEP);
				h_E_Lep[7]->Fill(E_LEP); //histo with all lepton energies
				h_E_Ecal_vs_Pcharged[i_im]->Fill(Pcharged, E_ECal);
				a_eventcount[i_im][i_cr] += 1;
				break;
			case 1:
				//ee cuts
				if (Ncharged < 7 && E_ECal >= 70 && Pcharged > 5 && cos_theta > -0.9 && cos_theta < 0.9) {
					//cout << iev << endl;
					//determine i_im
					if (E_LEP > 44.1 && E_LEP < 44.4) {
						i_im = 0;
					}
					else if (E_LEP > 44.5 && E_LEP < 44.9) {
						i_im = 1;
					}
					else if (E_LEP > 45.0 && E_LEP < 45.3) {
						i_im = 2;
					}
					else if (E_LEP > 45.4 && E_LEP < 45.8) {
						i_im = 3;
					}
					else if (E_LEP > 45.8 && E_LEP < 46.2) {
						i_im = 4;
					}
					else if (E_LEP > 46.3 && E_LEP < 46.7) {
						i_im = 5;
					}
					else if (E_LEP > 46.7 && E_LEP < 47.0) {
						i_im = 6;
					}
					h_Ncharged[i_im]->Fill(Ncharged);
					h_Pcharged[i_im]->Fill(Pcharged);
					h_E_Ecal[i_im]->Fill(E_ECal);
					h_E_Hcal[i_im]->Fill(E_HCal);
					h_cos_thru[i_im]->Fill(cos_thru);
					h_cos_thet[i_im]->Fill(cos_theta);
					h_E_Lep[i_im]->Fill(E_LEP);
					h_E_Lep[7]->Fill(E_LEP); //histo with all lepton energies
					h_E_Ecal_vs_Pcharged[i_im]->Fill(Pcharged, E_ECal);
					a_eventcount[i_im][i_cr] += 1;
				}
				break;
			case 2:
				//mm cuts
				if (Pcharged > 71 && Pcharged > 5 && E_ECal < 50 && Ncharged == 2) {
					//cout << iev << endl;
						//determine i_im
					if (E_LEP > 44.1 && E_LEP < 44.4) {
						i_im = 0;
						if (cos_theta < 1.0 && cos_theta > 0) { forward_count[0] += 1; }
						else if (cos_theta < 0 && cos_theta > -1.0) { backward_count[0] += 1; }
					}
					else if (E_LEP > 44.5 && E_LEP < 44.9) {
						i_im = 1;
						if (cos_theta < 1.0 && cos_theta > 0) { forward_count[1] += 1; }
						else if (cos_theta < 0 && cos_theta > -1.0) { backward_count[1] += 1; }
					}
					else if (E_LEP > 45.0 && E_LEP < 45.3) {
						i_im = 2;
						if (cos_theta < 1.0 && cos_theta > 0) { forward_count[2] += 1; }
						else if (cos_theta < 0 && cos_theta > -1.0) { backward_count[2] += 1; }
					}
					else if (E_LEP > 45.4 && E_LEP < 45.8) {
						i_im = 3;
						if (cos_theta < 1.0 && cos_theta > 0) { forward_count[3] += 1; }
						else if (cos_theta < 0 && cos_theta > -1.0) { backward_count[3] += 1; }
					}
					else if (E_LEP > 45.8 && E_LEP < 46.2) {
						i_im = 4;
						if (cos_theta < 1.0 && cos_theta > 0) { forward_count[4] += 1; }
						else if (cos_theta < 0 && cos_theta > -1.0) { backward_count[4] += 1; }
					}
					else if (E_LEP > 46.3 && E_LEP < 46.7) {
						i_im = 5;
						if (cos_theta < 1.0 && cos_theta > 0) { forward_count[5] += 1; }
						else if (cos_theta < 0 && cos_theta > -1.0) { backward_count[5] += 1; }
					}
					else if (E_LEP > 46.7 && E_LEP < 47.0) {
						i_im = 6;
						if (cos_theta < 1.0 && cos_theta > 0) { forward_count[6] += 1; }
						else if (cos_theta < 0 && cos_theta > -1.0) { backward_count[6] += 1; }
					}
					h_Ncharged[i_im]->Fill(Ncharged);
					h_Pcharged[i_im]->Fill(Pcharged);
					h_E_Ecal[i_im]->Fill(E_ECal);
					h_E_Hcal[i_im]->Fill(E_HCal);
					h_cos_thru[i_im]->Fill(cos_thru);
					h_cos_thet[i_im]->Fill(cos_theta);
					h_E_Lep[i_im]->Fill(E_LEP);
					h_E_Lep[7]->Fill(E_LEP); //histo with all lepton energies
					h_E_Ecal_vs_Pcharged[i_im]->Fill(Pcharged, E_ECal);
					a_eventcount[i_im][i_cr] += 1;
				}
				break;
			case 3:
				//tt cuts
				if (Pcharged > 5 && Pcharged <= 60 && Ncharged < 7 && E_ECal < 60 &&
					cos_thru > -0.9 && cos_thru < 0.9) {
					//cout << iev << endl;
						//determine i_im
					if (E_LEP > 44.1 && E_LEP < 44.4) {
						i_im = 0;
					}
					else if (E_LEP > 44.5 && E_LEP < 44.9) {
						i_im = 1;
					}
					else if (E_LEP > 45.0 && E_LEP < 45.3) {
						i_im = 2;
					}
					else if (E_LEP > 45.4 && E_LEP < 45.8) {
						i_im = 3;
					}
					else if (E_LEP > 45.8 && E_LEP < 46.2) {
						i_im = 4;
					}
					else if (E_LEP > 46.3 && E_LEP < 46.7) {
						i_im = 5;
					}
					else if (E_LEP > 46.7 && E_LEP < 47.0) {
						i_im = 6;
					}
					h_Ncharged[i_im]->Fill(Ncharged);
					h_Pcharged[i_im]->Fill(Pcharged);
					h_E_Ecal[i_im]->Fill(E_ECal);
					h_E_Hcal[i_im]->Fill(E_HCal);
					h_cos_thru[i_im]->Fill(cos_thru);
					h_cos_thet[i_im]->Fill(cos_theta);
					h_E_Lep[i_im]->Fill(E_LEP);
					h_E_Lep[7]->Fill(E_LEP); //histo with all lepton energies
					h_E_Ecal_vs_Pcharged[i_im]->Fill(Pcharged, E_ECal);
					a_eventcount[i_im][i_cr] += 1;
				}
				break;
			case 4:
				//qq cuts
				if (Ncharged >= 8 && Pcharged > 5) {
					//cout << iev << endl;
						//determine i_im
					if (E_LEP > 44.1 && E_LEP < 44.4) {
						i_im = 0;
					}
					else if (E_LEP > 44.5 && E_LEP < 44.9) {
						i_im = 1;
					}
					else if (E_LEP > 45.0 && E_LEP < 45.3) {
						i_im = 2;
					}
					else if (E_LEP > 45.4 && E_LEP < 45.8) {
						i_im = 3;
					}
					else if (E_LEP > 45.8 && E_LEP < 46.2) {
						i_im = 4;
					}
					else if (E_LEP > 46.3 && E_LEP < 46.7) {
						i_im = 5;
					}
					else if (E_LEP > 46.7 && E_LEP < 47.0) {
						i_im = 6;
					}
					h_Ncharged[i_im]->Fill(Ncharged);
					h_Pcharged[i_im]->Fill(Pcharged);
					h_E_Ecal[i_im]->Fill(E_ECal);
					h_E_Hcal[i_im]->Fill(E_HCal);
					h_cos_thru[i_im]->Fill(cos_thru);
					h_cos_thet[i_im]->Fill(cos_theta);
					h_E_Lep[i_im]->Fill(E_LEP);
					h_E_Lep[7]->Fill(E_LEP); //histo with all lepton energies
					h_E_Ecal_vs_Pcharged[i_im]->Fill(Pcharged, E_ECal);
					a_eventcount[i_im][i_cr] += 1;
				}
				break;
			case default:
				break;
			}//end of switch
		}//end of events loop

		//loop to save graphs for each i_im
		for (int i_im = 0; i_im < n_invmass; ++i_im) {

			TCanvas *c[n_invmass + 2];
			c[0] = new TCanvas("c0", "c0", 1920, 1080);
			c[1] = new TCanvas("c1", "c1", 1920, 1080);
			c[2] = new TCanvas("c2", "c2", 1920, 1080);
			c[3] = new TCanvas("c3", "c3", 1920, 1080);
			c[4] = new TCanvas("c4", "c4", 1920, 1080);
			c[5] = new TCanvas("c5", "c5", 1920, 1080);
			c[6] = new TCanvas("c6", "c6", 1920, 1080);
			c[7] = new TCanvas("c7", "c7", 1920, 1080);
			c[8] = new TCanvas("c8", "c8", 1920, 1080);

			c[0]->cd();
			graphstyle(h_Ncharged[i_im], "Ncharged", "Number of events");
			h_Ncharged[i_im]->Draw("HIST");
			c[0]->SaveAs("../results/data_results/" + invmassname[i_im] + "/" + cutname[i_cr] + "_Ncharged.png");
			c[0]->Close();

			c[1]->cd();
			h_Pcharged[i_im]->Draw("HIST");
			graphstyle(h_Pcharged[i_im], "Pcharged [GeV]", "Number of events");
			c[1]->SaveAs("../results/data_results/" + invmassname[i_im] + "/" + cutname[i_cr] + "_Pcharged.png");
			c[1]->Close();

			c[2]->cd();
			graphstyle(h_E_Ecal[i_im], "E_Ecal [GeV]", "Number of events");
			h_E_Ecal[i_im]->Draw("HIST");
			c[2]->SaveAs("../results/data_results/" + invmassname[i_im] + "/" + cutname[i_cr] + "_E_Ecal.png");
			c[2]->Close();

			c[3]->cd();
			graphstyle(h_E_Hcal[i_im], "E_Hcal [GeV]", "Number of events");
			h_E_Hcal[i_im]->Draw("HIST");
			c[3]->SaveAs("../results/data_results/" + invmassname[i_im] + "/" + cutname[i_cr] + "_E_Hcal.png");
			c[3]->Close();

			c[4]->cd();
			graphstyle(h_cos_thru[i_im], "Cos_thrust", "Number of events");
			h_cos_thru[i_im]->Draw("HIST");
			c[4]->SaveAs("../results/data_results/" + invmassname[i_im] + "/" + cutname[i_cr] + "_cos_thru.png");
			c[4]->Close();

			c[5]->cd();
			graphstyle(h_cos_thet[i_im], "Cos_theta", "Number of events");
			h_cos_thet[i_im]->Draw("HIST");
			c[5]->SaveAs("../results/data_results/" + invmassname[i_im] + "/" + cutname[i_cr] + "_cos_theta.png");
			c[5]->Close();

			c[6]->cd();
			graphstyle(h_E_Lep[i_im], "E_Lep [GeV]", "Number of entries");
			h_E_Lep[i_im]->Draw("HIST");
			c[6]->SaveAs("../results/data_results/" + invmassname[i_im] + "/" + cutname[i_cr] + "_E_Lep.png");
			c[6]->Close();

			c[7]->cd();
			graphstyle(h_E_Ecal_vs_Pcharged[i_im], "Sum of track energies [Gev]", "Energy deposited in electronic calorimeter [GeV]");
			h_E_Ecal_vs_Pcharged[i_im]->Draw("HIST COLZ");
			c[7]->SaveAs("../results/data_results/" + invmassname[i_im] + "/" + cutname[i_cr] + "_E_Ecal_vs_Pcharged.png");
			c[7]->Close();
		}//end of save graphics loop

		//print graph with non-cut E_LEP for report
		c[8]->cd();
		h_E_Lep[7]->Draw("HIST");
		c[8]->SaveAs("../results/data_results/all_lep/" + cutname[i_cr] + "_E_Lep.png");
		c[8]->Close();

		//write ee-cut histogram
		if (i_cr == 1) {
			for (int s = 0; s < n_invmass; ++s) {
				f_out[s]->WriteTObject(h_cos_thet[s]);
			}
		}

	} //end of cut region loop

	//print forward backward count
	cout << "Forward count sqrt(s)=88.48: " << forward_count[0] << endl;
	cout << "Backward count sqrt(s)=88.48: " << backward_count[0] << endl;
	cout << "Forward count sqrt(s)=89.47: " << forward_count[1] << endl;
	cout << "Backward count sqrt(s)=89.47: " << backward_count[1] << endl;
	cout << "Forward count sqrt(s)=90.23: " << forward_count[2] << endl;
	cout << "Backward count sqrt(s)=90.23: " << backward_count[2] << endl;
	cout << "Forward count sqrt(s)=91.23: " << forward_count[3] << endl;
	cout << "Backward count sqrt(s)=91.23: " << backward_count[3] << endl;
	cout << "Forward count sqrt(s)=91.97: " << forward_count[4] << endl;
	cout << "Backward count sqrt(s)=91.97: " << backward_count[4] << endl;
	cout << "Forward count sqrt(s)=92.97: " << forward_count[5] << endl;
	cout << "Backward count sqrt(s)=92.97: " << backward_count[5] << endl;
	cout << "Forward count sqrt(s)=93.72: " << forward_count[6] << endl;
	cout << "Backward count sqrt(s)=93.72: " << backward_count[6] << endl;

	ofstream neventprint;
	neventprint .open("../results/matrix/nevent.txt");
	neventprint<< "Event counts of the cuts at different invariant masses" << endl;
	neventprint << "\t" << cutname[0] << "\t" << cutname[1] << cutname[2] << "\t" << cutname[3] << "\t" << cutname[4] << "\t" << endl;
	for(int i=0; i < n_invmass; ++i){
		neventprint << invmassname[i] << "\t" << a_eventcount[i][0] << "\t" << a_eventcount[i][1] << "\t" << 
			a_eventcount[i][2] << "\t" << a_eventcount[i][3] << "\t" << a_eventcount[i][4] << endl;
	}
	neventprint.close();
	for(int i=0; i < n_invmass; ++i) {
		f_out[i]->Close();
	}
	//close opened files
	data->Close();
}
