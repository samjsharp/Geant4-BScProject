#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"

void CalculateMultiplicity() {
    // Open the ROOT file
    TFile *file = new TFile("B4.root", "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Error: Cannot open ROOT file!" << std::endl;
        return;
    }

    // Get the tree
    TTree *tree = (TTree*)file->Get("B4");
    if (!tree) {
        std::cerr << "Error: Cannot find tree in ROOT file!" << std::endl;
        return;
    }

    // Variables to hold energy depositions from Si1 to Si6
    double SiEdep[6];
    tree->SetBranchAddress("Si1_Edep", &SiEdep[0]);
    tree->SetBranchAddress("Si2_Edep", &SiEdep[1]);
    tree->SetBranchAddress("Si3_Edep", &SiEdep[2]);
    tree->SetBranchAddress("Si4_Edep", &SiEdep[3]);
    tree->SetBranchAddress("Si5_Edep", &SiEdep[4]);
    tree->SetBranchAddress("Si6_Edep", &SiEdep[5]);

    // Histogram for Multiplicity
    TH1D *hMultiplicity = new TH1D("hMultiplicity", "Multiplicity Distribution; Multiplicity; Counts", 7, 0, 7);

    // Loop over all events
    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; i++) {
        tree->GetEntry(i);

 int multiplicity = 0;
for (int i = 0; i < 6; i++) { // Treat all Si detectors equally
    if (SiEdep[i] > 0) multiplicity++;
}


        hMultiplicity->Fill(multiplicity);
    }

    // Draw the histogram and save it
    TCanvas *canvas = new TCanvas("canvas", "Multiplicity Distribution", 800, 600);
    hMultiplicity->SetLineColor(kBlue);
    hMultiplicity->SetLineWidth(2);
    hMultiplicity->Draw();

    canvas->SaveAs("Multiplicity_Distribution.png");

    // Save the histogram to a ROOT file for further analysis
    TFile outFile("Multiplicity.root", "RECREATE");
    hMultiplicity->Write();
    outFile.Close();

    // Clean up
    file->Close();
    delete file;
    delete canvas;
}
