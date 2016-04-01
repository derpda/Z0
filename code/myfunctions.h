#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H

#include <string>

using namespace std;




/*This function is supposed to do the standard styling of graphs. Inputs are one root graph object and its axis titles.*/

void graphstyle(TH1F *histo, string xin, string yin) {
	//put char arrays into string

	//set axis title styles
	histo->GetXaxis()->SetTitleOffset(1.3);
	histo->GetYaxis()->SetTitleOffset(1.3);
	histo->GetXaxis()->CenterTitle();
	histo->GetYaxis()->CenterTitle();
	histo->GetXaxis()->SetTitleSize(0.039);
	histo->GetYaxis()->SetTitleSize(0.039);
	//set axis titles

	histo->GetXaxis()->SetTitle(("#bf{" + xin +"}").c_str());
	histo->GetYaxis()->SetTitle(("#bf{" + yin +"}").c_str());
}
/*Overloading of above function to accomodate TH2F objects.*/

void graphstyle(TH2F *histo, string xin, string yin) {
	//put char arrays into string

	//set axis title styles
	histo->GetXaxis()->SetTitleOffset(1.3);
	histo->GetYaxis()->SetTitleOffset(1.3);
	histo->GetXaxis()->CenterTitle();
	histo->GetYaxis()->CenterTitle();
	histo->GetXaxis()->SetTitleSize(0.039);
	histo->GetYaxis()->SetTitleSize(0.039);
	//set axis titles

	histo->GetXaxis()->SetTitle(("#bf{" + xin + "}").c_str());
	histo->GetYaxis()->SetTitle(("#bf{" + yin + "}").c_str());
}





#endif // !MYFUNCTIONS_H()