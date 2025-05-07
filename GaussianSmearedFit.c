#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include "TLegend.h"

void GaussianSmearing() {
    TFile *inputFile = new TFile("B4.root", "READ");
    if (!inputFile || inputFile->IsZombie()) {
        std::cerr << "Error: Could not open the ROOT file!" << std::endl;
        return;
    }

    const int numDetectors = 6;
    std::string detectorNames[numDetectors] = {"Si1_Edep", "Si2_Edep", "Si3_Edep", "Si4_Edep", "Si5_Edep", "Si6_Edep"};
    
    TRandom3 randGen;
    
    for (int i = 0; i < numDetectors; i++) {
        TTree *tree = (TTree*)inputFile->Get("B4;1");
        if (!tree) {
            std::cerr << "Error: Could not retrieve tree!" << std::endl;
            return;
        }

        Double_t edep;
        tree->SetBranchAddress(detectorNames[i].c_str(), &edep);

        TH1D *hSmeared = new TH1D(("h" + detectorNames[i] + "_smeared").c_str(), (detectorNames[i] + " Spectra").c_str(), 100, 0, 3);
        hSmeared->GetXaxis()->SetTitle("Energy Deposition (MeV)");
        hSmeared->GetYaxis()->SetTitle("Counts");

        Long64_t nEntries = tree->GetEntries();
        for (Long64_t j = 0; j < nEntries; j++) {
            tree->GetEntry(j);
            if (edep > 0) {  // Ignore zero-energy events
                Double_t smearedEdep = randGen.Gaus(edep, 0.1 * edep);  // Apply Gaussian smearing
                hSmeared->Fill(smearedEdep);
            }
        }

        TCanvas *canvas = new TCanvas(("c" + detectorNames[i]).c_str(), detectorNames[i].c_str(), 800, 600);
        hSmeared->Draw("HIST");
        canvas->SaveAs((detectorNames[i] + "_smeared.png").c_str());

        delete hSmeared;
        delete canvas;
    }
    
    inputFile->Close();
    delete inputFile;
}
