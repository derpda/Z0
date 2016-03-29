//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Mar 21 11:46:20 2016 by ROOT version 5.34/30
// from TTree h3/FP2
// found on file: mm.root
//////////////////////////////////////////////////////////

#ifndef h3_h
#define h3_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class h3 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         run;
   Float_t         event;
   Float_t         Ncharged;
   Float_t         Pcharged;
   Float_t         E_ecal;
   Float_t         E_hcal;
   Float_t         E_lep;
   Float_t         cos_thru;
   Float_t         cos_thet;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_Ncharged;   //!
   TBranch        *b_Pcharged;   //!
   TBranch        *b_E_ecal;   //!
   TBranch        *b_E_hcal;   //!
   TBranch        *b_E_lep;   //!
   TBranch        *b_cos_thru;   //!
   TBranch        *b_cos_thet;   //!

   h3(TTree *tree=0);
   virtual ~h3();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef h3_cxx
h3::h3(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("mm.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("mm.root");
      }
      f->GetObject("h3",tree);

   }
   Init(tree);
}

h3::~h3()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t h3::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t h3::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void h3::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("Ncharged", &Ncharged, &b_Ncharged);
   fChain->SetBranchAddress("Pcharged", &Pcharged, &b_Pcharged);
   fChain->SetBranchAddress("E_ecal", &E_ecal, &b_E_ecal);
   fChain->SetBranchAddress("E_hcal", &E_hcal, &b_E_hcal);
   fChain->SetBranchAddress("E_lep", &E_lep, &b_E_lep);
   fChain->SetBranchAddress("cos_thru", &cos_thru, &b_cos_thru);
   fChain->SetBranchAddress("cos_thet", &cos_thet, &b_cos_thet);
   Notify();
}

Bool_t h3::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void h3::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t h3::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef h3_cxx
