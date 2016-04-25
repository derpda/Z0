#include <sstream>
#include <myfunctions.h>
#include <string> //for to_string

using namespace std;

void cutMC()
{
	gStyle->SetHistLineWidth(2.);
	gStyle->SetLabelSize(0.05, "x");
	gStyle->SetLabelSize(0.05, "y");
	gStyle->SetOptStat(0);
	gStyle->SetLabelFont(42, "x");
	gStyle->SetLabelFont(42, "y");
	gStyle->SetOptTitle(0);

	const int n_sim = 4;
	TFile *files[n_sim];
	files[0] = new TFile("../daten/mc/ee.root", "READ");
	files[1] = new TFile("../daten/mc/mm.root", "READ");
	files[2] = new TFile("../daten/mc/tt.root", "READ");
	files[3] = new TFile("../daten/mc/qq.root", "READ");
	TTree *mc_trees[n_sim];
	for(int i=0; i<n_sim; ++i) mc_trees[i] = (TTree*) files[i]->Get("h3");

	//Graph settings
	const int w = 1920;
	const int h = 1080;
	const float offset = 1.3;


	//define histograms
	TString name[n_sim];
	name[0] = "ee";
	name[1] = "mm";
	name[2] = "tt";
	name[3] = "qq";

	TH1F *h_Ncharged[n_sim];
	TH1F *h_Pcharged[n_sim];
	TH1F *h_E_Ecal[n_sim];
	TH1F *h_E_Hcal[n_sim];
	TH1F *h_cos_thru[n_sim];
	TH1F *h_cos_thet[n_sim];
	TH2F *h_E_Ecal_vs_Pcharged[n_sim];
	TH2F *h_Ncharged_vs_Pcharged[n_sim];

	const int n_histos = 8;
	for(int isim = 0; isim < n_sim; ++isim) {
		h_Ncharged[isim] = new TH1F("h_Ncharged_" + name[isim], "Number of tracks", 40, 0., 40.);
		h_Ncharged[isim]->Sumw2();
		h_Pcharged[isim] = new TH1F("h_Pcharged_" + name[isim], "Sum of track energies", 120, 1., 120.);
		h_Pcharged[isim]->Sumw2();
		h_E_Ecal[isim] = new TH1F("h_Eecal_" + name[isim], "E_ECal", 60, 0., 120.);
		h_E_Ecal[isim]->Sumw2();
		h_E_Hcal[isim] = new TH1F("h_Ehcal_" + name[isim], "E_HCal", 50, 0., 50.);
		h_E_Hcal[isim]->Sumw2();
		h_cos_thru[isim] = new TH1F("h_costhru_" + name[isim], "Cos thrust", 40, -1., 1.);
		h_cos_thru[isim]->Sumw2();
		h_cos_thet[isim] = new TH1F("h_costhet_" + name[isim], "Cos theta", 100, -1., 1.);
		h_cos_thet[isim]->Sumw2();
		h_E_Ecal_vs_Pcharged[isim] = new TH2F("h_E_Ecal_vs_Pcharged_" + name[isim], "E_Ecal vs Pcharged", 120, 0., 120., 120, 0., 120.);
		h_E_Ecal_vs_Pcharged[isim]->Sumw2();
		h_Ncharged_vs_Pcharged[isim] = new TH2F("h_Ncharged_vs_Pcharged_" + name[isim], "Ncharged vs Pcharged", 120, 0., 120., 7, 0., 7.);
		h_Ncharged_vs_Pcharged[isim]->Sumw2();
	}


	//Loop over the four different cuts as well no the no cut option
	int n_cutregions = 5;
	TString cutname[n_cutregions];
	cutname[0]="nocut";
	cutname[1]="ee_cut";
	cutname[2]="mm_cut";
	cutname[3]="tt_cut";
	cutname[4]="qq_cut";

	//counter for fwd and bwd
	int forward_count = 0;
	int backward_count = 0;

	for (int i_cr = 0; i_cr < n_cutregions; ++i_cr) {
		//define parameters
		float event, run, Ncharged, Pcharged, E_ECal, E_HCal, E_LEP, cos_thru, cos_theta;


		for (int isim = 0; isim < n_sim; ++isim) {
			//reset TH1F histograms
			h_Ncharged[isim]->Reset("M");
			h_Pcharged[isim]->Reset("M");
			h_E_Ecal[isim]->Reset("M");
			h_E_Hcal[isim]->Reset("M");
			h_cos_thru[isim]->Reset("M");
			h_cos_thet[isim]->Reset("M");
			h_E_Ecal_vs_Pcharged[isim]->Reset("M");
			h_Ncharged_vs_Pcharged[isim]->Reset("M");
			mc_trees[isim]->SetBranchAddress("event", &event);
			mc_trees[isim]->SetBranchAddress("run", &run);
			mc_trees[isim]->SetBranchAddress("Ncharged", &Ncharged);
			mc_trees[isim]->SetBranchAddress("Pcharged", &Pcharged);
			mc_trees[isim]->SetBranchAddress("E_ecal", &E_ECal);
			mc_trees[isim]->SetBranchAddress("E_hcal", &E_HCal);
			mc_trees[isim]->SetBranchAddress("E_lep", &E_LEP);
			mc_trees[isim]->SetBranchAddress("cos_thru", &cos_thru);
			mc_trees[isim]->SetBranchAddress("cos_thet", &cos_theta);
			const int nevents = mc_trees[isim]->GetEntries();
			//			cout << "number of events for " << name[isim] <<" :" << nevents <<endl;

						//event counters and efficiency, purity matrix
			float n_events_cut = 0; //save to variable later
			float n_events_cut_w[4] = ; //save to variable later
			float efficiency[4][4];
			float purity[4] = ;

			//set weights for different decays
			float weights[n_sim] = { 83.8 / 2486.2,83.8 / 2486.2,83.8 / 2486.2,1732 / 2486.2 };

			//apply the different cuts via switch
			for (int iev = 0; iev < nevents; ++iev) {
				mc_trees[isim]->GetEntry(iev);
				switch (i_cr) {

				case 0:
					//no cuts
					h_Ncharged[isim]->Fill(Ncharged);
					h_Pcharged[isim]->Fill(Pcharged);
					h_E_Ecal[isim]->Fill(E_ECal);
					h_E_Hcal[isim]->Fill(E_HCal);
					h_cos_thru[isim]->Fill(cos_thru);
					h_cos_thet[isim]->Fill(cos_theta);
					h_E_Ecal_vs_Pcharged[isim]->Fill(Pcharged, E_ECal);
					break;
				case 1:
					//ee cuts
					if (Ncharged < 7 && E_ECal >= 70 /*&& Pcharged > 5 && cos_theta > -0.9 && cos_theta < 0.9*/) {
						n_events_cut += 1;
						h_Ncharged[isim]->Fill(Ncharged, weights[isim]);
						h_Pcharged[isim]->Fill(Pcharged, weights[isim]);
						h_E_Ecal[isim]->Fill(E_ECal, weights[isim]);
						h_E_Hcal[isim]->Fill(E_HCal, weights[isim]);
						h_cos_thru[isim]->Fill(cos_thru, weights[isim]);
						h_cos_thet[isim]->Fill(cos_theta, weights[isim]);
						h_E_Ecal_vs_Pcharged[isim]->Fill(Pcharged, E_ECal, weights[isim]);
						h_Ncharged_vs_Pcharged[isim]->Fill(Pcharged, Ncharged, weights[isim]);
					}
					break;
				case 2:
					//mm cuts
					if (/*Pcharged > 71 && Pcharged > 5 && */E_ECal < 50 && Ncharged == 2) {
						n_events_cut += 1;
						h_Ncharged[isim]->Fill(Ncharged, weights[isim]);
						h_Pcharged[isim]->Fill(Pcharged, weights[isim]);
						h_E_Ecal[isim]->Fill(E_ECal, weights[isim]);
						h_E_Hcal[isim]->Fill(E_HCal, weights[isim]);
						h_cos_thru[isim]->Fill(cos_thru, weights[isim]);
						h_cos_thet[isim]->Fill(cos_theta, weights[isim]);
						h_E_Ecal_vs_Pcharged[isim]->Fill(Pcharged, E_ECal, weights[isim]);
						h_Ncharged_vs_Pcharged[isim]->Fill(Pcharged, Ncharged, weights[isim]);
						if (cos_theta < 0.9 && cos_theta > 0) {
							forward_count += 1;
						}
						else if (cos_theta < 0 && cos_theta > -0.9) { backward_count += 1; }
					}
					break;
				case 3:
					//tt cuts
					if ( /*Pcharged > 5 && Pcharged <= 60 && */Ncharged < 7 && E_ECal < 60 /* &&
						cos_thru > -0.9 && cos_thru < 0.9*/) {
						n_events_cut += 1;
						h_Ncharged[isim]->Fill(Ncharged, weights[isim]);
						h_Pcharged[isim]->Fill(Pcharged, weights[isim]);
						h_E_Ecal[isim]->Fill(E_ECal, weights[isim]);
						h_E_Hcal[isim]->Fill(E_HCal, weights[isim]);
						h_cos_thru[isim]->Fill(cos_thru, weights[isim]);
						h_cos_thet[isim]->Fill(cos_theta, weights[isim]);
						h_E_Ecal_vs_Pcharged[isim]->Fill(Pcharged, E_ECal, weights[isim]);
						h_Ncharged_vs_Pcharged[isim]->Fill(Pcharged, Ncharged, weights[isim]);
					}
					break;
				case 4:
					//qq cuts
					if (Ncharged >= 8/* && Pcharged > 5*/) {
						n_events_cut += 1;
						h_Ncharged[isim]->Fill(Ncharged, weights[isim]);
						h_Pcharged[isim]->Fill(Pcharged, weights[isim]);
						h_E_Ecal[isim]->Fill(E_ECal, weights[isim]);
						h_E_Hcal[isim]->Fill(E_HCal, weights[isim]);
						h_cos_thru[isim]->Fill(cos_thru, weights[isim]);
						h_cos_thet[isim]->Fill(cos_theta, weights[isim]);
						h_E_Ecal_vs_Pcharged[isim]->Fill(Pcharged, E_ECal, weights[isim]);
						h_Ncharged_vs_Pcharged[isim]->Fill(Pcharged, Ncharged, weights[isim]);
					}
					break;
				}//end of switch

				//if no events registered, add one
				if (n_events_cut == 0) n_events_cut += 1;

			}//end of events loop

		//fill efficiency matrix for all but first cut region (no cut) and last isim (data)
			if (i_cr > 0) efficiency[i_cr - 1][isim] = n_events_cut / nevents;

		}//end of isim loop

		//fill purity vector (needs all results of isim, thus after loop)
		if (i_cr > 0) {
			purity[i_cr - 1] = weights[i_cr - 1] * efficiency[i_cr - 1][i_cr - 1] / (weights[0] * efficiency[i_cr - 1][0] + weights[1] * efficiency[i_cr - 1][1] +
				weights[2] * efficiency[i_cr - 1][2] + weights[3] * efficiency[i_cr - 1][3]);
		}


		TCanvas *c[n_histos];
		//for (i = 0; i < n_histos; ++i) {
		//	c[i] = new TCanvas("c" + to_string(i), "c" + to_string(i), w, h);
		//}
		c[0] = new TCanvas("c0", "c0", 1920, 1080);
		c[1] = new TCanvas("c1", "c1", 1920, 1080);
		c[2] = new TCanvas("c2", "c2", 1920, 1080);
		c[3] = new TCanvas("c3", "c3", 1920, 1080);
		c[4] = new TCanvas("c4", "c4", 1920, 1080);
		c[5] = new TCanvas("c5", "c5", 1920, 1080);
		c[6] = new TCanvas("c6", "c6", 1920, 1080);
		c[7] = new TCanvas("c7", "c7", 1920, 1080);
		EColor color[n_sim];
		color[0] = kBlue;
		color[1] = kRed;
		color[2] = kGreen;
		color[3] = kBlack;


		float max[n_histos] = { 0., 0., 0., 0., 0., 0., 0., 0. };
		for (int i = 0; i < n_sim; ++i) {
			float act_max = h_Ncharged[i]->GetMaximum();
			if (act_max > max[0]) {
				max[0] = act_max;
			}
		}
		for (int i = 0; i < n_sim; ++i) {
			float act_max = h_Pcharged[i]->GetMaximum();
			if (act_max > max[1]) {
				max[1] = act_max;
			}
		}
		for (int i = 0; i < n_sim; ++i) {
			float act_max = h_E_Ecal[i]->GetMaximum();
			if (act_max > max[2]) {
				max[2] = act_max;
			}
		}
		for (int i = 0; i < n_sim; ++i) {
			float act_max = h_E_Hcal[i]->GetMaximum();
			if (act_max > max[3]) {
				max[3] = act_max;
			}
		}
		for (int i = 0; i < n_sim; ++i) {
			float act_max = h_cos_thru[i]->GetMaximum();
			if (act_max > max[4]) {
				max[4] = act_max;
			}
		}
		for (int i = 0; i < n_sim; ++i) {
			float act_max = h_cos_thet[i]->GetMaximum();
			if (act_max > max[5]) {
				max[5] = act_max;
			}
		}
		for (int i = 0; i < n_sim; ++i) {
			float act_max = h_E_Ecal_vs_Pcharged[i]->GetMaximum();
			if (act_max > max[6]) {
				max[6] = act_max;
			}
		}
		for (int i = 0; i < n_sim; ++i) {
			float act_max = h_Ncharged_vs_Pcharged[i]->GetMaximum();
			if (act_max > max[7]) {
				max[7] = act_max;
			}
		}
		//define legend
		TLegend *leg = new TLegend(0.76, 0.64, 0.88, 0.88);


		c[0]->cd();
		//set windows size for first graph to prevent bug
		c[0]->SetWindowSize(w + (w - c[0]->GetWw()), h + (h - c[0]->GetWh()));
		for(int ip=0; ip < n_sim; ++ip) {
			graphstyle(h_Ncharged[ip], "Ncharged", "Relative number of events per bin");
			if(i_cr==0) {
			h_Ncharged[ip]->Scale(1/(h_Ncharged[ip]->GetEntries()));
			h_Ncharged[ip]->SetMaximum(0.7);
			}
			graphstyle(h_Ncharged[ip], "Ncharged", "Number of events per bin");
			if(i_cr>0) h_Ncharged[ip]->SetMaximum(max[0]*1.3);
			h_Ncharged[ip]->SetLineColor(color[ip]);
			if(ip==0) h_Ncharged[ip]->Draw("HIST");
			else if(ip < 4) h_Ncharged[ip]->Draw("SAME HIST");
			if(ip<4) leg->AddEntry(h_Ncharged[ip], name[ip], "L");
		}
		leg->Draw("SAME");
		c[0]->SaveAs("../results/MC_results/" + cutname[i_cr] + "/Ncharged_vergleich.png");
		c[0]->Close();

		/*c[1]->cd();
		for(int ip=0; ip < n_sim; ++ip) {
			graphstyle(h_Pcharged[ip], "Pcharged [GeV]", "Relative number of events per bin");
			if(i_cr==0) {
			h_Pcharged[ip]->Scale(1/(h_Pcharged[ip]->GetEntries()));
			h_Pcharged[ip]->SetMaximum(0.16);
			}
			if(i_cr>0) h_Pcharged[ip]->SetMaximum(max[1]*1.3);
			h_Pcharged[ip]->SetLineColor(color[ip]);
			if(ip==0) h_Pcharged[ip]->Draw("HIST");
			else if(ip<4) h_Pcharged[ip]->Draw("SAME HIST");
		}
		leg->Draw("SAME");
		c[1]->SaveAs("../results/MC_results/" + cutname[i_cr] + "/Pcharged.png");
		c[1]->Close();

		c[2]->cd();
		for(int ip=0; ip < n_sim; ++ip) {
			graphstyle(h_E_Ecal[ip], "E_Ecal [GeV]", "Relative number of events per bin");
			if(i_cr==0) {
			h_E_Ecal[ip]->Scale(1/(h_E_Ecal[ip]->GetEntries()));
			h_E_Ecal[ip]->SetMaximum(0.6);
			}
			if(i_cr>0) h_E_Ecal[ip]->SetMaximum(max[2]*1.3);
			h_E_Ecal[ip]->SetLineColor(color[ip]);
			if(ip==0) h_E_Ecal[ip]->Draw("HIST");
			else if(ip<4) h_E_Ecal[ip]->Draw("SAME HIST");
		}
		leg->Draw("SAME");
		c[2]->SaveAs("../results/MC_results/" + cutname[i_cr] + "/E_Ecal.png");
		c[2]->Close();

		c[3]->cd();
		for(int ip=0; ip < n_sim; ++ip) {
			graphstyle(h_E_Hcal[ip], "E_Hcal [GeV]","Relative number of events per bin");
			if(i_cr==0) {
			h_E_Hcal[ip]->Scale(1/(h_E_Hcal[ip]->GetEntries()));			
			h_E_Hcal[ip]->SetMaximum(0.8);
			}
			if(i_cr!=0) h_E_Hcal[ip]->SetMaximum(max[3]*1.3);
			h_E_Hcal[ip]->SetLineColor(color[ip]);
			if(ip==0) h_E_Hcal[ip]->Draw("HIST");
			else if(ip<4) h_E_Hcal[ip]->Draw("SAME HIST");
		}
		leg->Draw("SAME");
		c[3]->SaveAs("../results/MC_results/" + cutname[i_cr] + "/E_Hcal.png");
		c[3]->Close();

		c[4]->cd();
		for(int ip=0; ip < n_sim; ++ip) {
			graphstyle(h_cos_thru[ip], "Cos_thrust", "Relative number of entries");
			if(i_cr==0) {
			h_cos_thru[ip]->Scale(1/(h_cos_thru[ip]->GetEntries()));			
			h_cos_thru[ip]->SetMaximum(0.1);
			}
			if(i_cr>0) h_cos_thru[ip]->SetMaximum(max[4]*1.3);
			h_cos_thru[ip]->SetLineColor(color[ip]);
			if(ip==0) h_cos_thru[ip]->Draw("HIST");
			else if(ip<4) h_cos_thru[ip]->Draw("SAME HIST");
		}
		leg->Draw("SAME");
		c[4]->SaveAs("../results/MC_results/" + cutname[i_cr] + "/cos_thru.png");
		c[4]->Close();

		c[5]->cd();
		for(int ip=0; ip < n_sim; ++ip) {
			graphstyle(h_cos_thet[ip], "Cos_theta", "Relative number of entries");
			if(i_cr==0) {
			h_cos_thet[ip]->Scale(1/(h_cos_thet[ip]->GetEntries()));			
			h_cos_thet[ip]->SetMaximum(0.06);
			}
			if(i_cr>0) h_cos_thet[ip]->SetMaximum(max[5]*1.3);
			h_cos_thet[ip]->SetLineColor(color[ip]);
			if (ip == 0) h_cos_thet[ip]->Draw("HIST");
			else if (ip<4) h_cos_thet[ip]->Draw("SAME HIST");
		}
		leg->Draw("SAME");
		c[5]->SaveAs("../results/MC_results/" + cutname[i_cr] + "/cos_theta.png");
		c[5]->Close();


		for (int ip = 0; ip < n_sim; ++ip) {
			if (ip == i_cr-1) {
				c[6]->cd(); 
				graphstyle(h_E_Ecal_vs_Pcharged[ip], "Sum of track energies [Gev]", "Energy deposited in electronic calorimeter [GeV]");
				h_E_Ecal_vs_Pcharged[ip]->Draw("HIST COLZ");
				c[6]->SaveAs("../results/MC_results/" + cutname[i_cr] + "/E_Ecal_vs_Pcharged.png");
			}
		}
		c[6]->Close();
		
		for (int ip = 0; ip < n_sim; ++ip) {
			if (ip == i_cr - 1) {
				c[7]->cd();
				graphstyle(h_Ncharged_vs_Pcharged[ip], "Sum of track energies [GeV]", "Number of charged tracks");
				h_Ncharged_vs_Pcharged[ip]->Draw("HIST COLZ");
				c[7]->SaveAs("../results/MC_results/" + cutname[i_cr] + "/Ncharged_vs_Pcharged.png");
			}
		}
		c[7]->Close();*/
	} //end of cut region loop


	//close opened files
	for(int iFile = 0; iFile < n_sim; ++iFile) files[iFile]->Close();

	//print forward backward count
	cout << "Forward count: " << forward_count << endl;
	cout << "Backward count: " << backward_count << endl;

	ofstream pur_out;
	pur_out.open("../results/matrix/efficiency_and_purity.txt");
	pur_out << "Efficiency array: \n" << endl;
	pur_out << "\t  "<< name[0] <<"\t\t  "<< name[1] <<"\t\t  "<< name[2] <<"\t\t  "<< name[3] <<"\t\t\t"<< "Purity"
		<<"\t\t"<< "Product" << endl;
	for (i=0 ; i < n_cutregions -1 ; ++i) {
		pur_out << cutname[i+1] <<"\t"<< efficiency[i][0] <<"\t"
		<< efficiency[i][1] <<"\t"<< efficiency[i][2] <<"\t"<< efficiency[i][3] <<"\t\t"<< purity[i] 
		<< "\t\t"<< efficiency[i][i]*purity[i] << endl; 
	}
	pur_out.close();

	ofstream eff_out;
	eff_out.open("../results/matrix/efficiency_list.txt");
	for(int i=0; i < n_cutregions -1 ; ++i) {
		for(int j=0; j < n_sim; ++j) {
			eff_out << "matrix(" << i << "," << j << ")=" << efficiency[i][j] << ";" << endl;
		}
	}
	eff_out.close();


	//tex output for the table
	TString eventname[4] = {"$e^+e^-$","$\\mu^+\\mu^-$","$\\tau^+\\tau^-$","$q^+q^-$"};

	ofstream eff_tex;
	eff_tex.open("../results/matrix/efficiency_list_tex.txt");
	for (int i = 0; i < n_cutregions - 1; ++i) {
		eff_tex << setprecision(5) << fixed << "&" << eventname[i] <<"&"<< efficiency[i][0] << "&" << efficiency[i][1] << "&" << efficiency[i][2] << "&" << efficiency[i][3] <<"\\\\" << endl;
	}
	eff_tex.close();
}	
