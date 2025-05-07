#include <iostream> 
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include "TF1.h"
#include "TLegend.h"

void CombineSiSpectra() {
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

    // Create histogram for total energy deposition
    TH1D *hTotalEdep = new TH1D("hTotalEdep", "Total Energy Deposition Spectrum; Energy Deposition (MeV); Counts", 100, 0, 3.0);
    
    TRandom3 randGen;
    double sigma = 0.05; // Smearing factor

    // Loop over events in the tree
    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; i++) {
        tree->GetEntry(i);
        double totalEdep = 0.0;

        // Sum up energy depositions from all detectors
        for (int j = 0; j < 6; j++) {
            if (SiEdep[j] > 0) { // Ignore zero events
                totalEdep += SiEdep[j];
            }
        }

        // Apply Gaussian smearing
        if (totalEdep > 0) {
            double smearedEdep = randGen.Gaus(totalEdep, sigma * totalEdep);
            hTotalEdep->Fill(smearedEdep);
        }
    }

    // Get histogram properties for a better fit
    double mean = hTotalEdep->GetMean();
    double sigmaHist = hTotalEdep->GetRMS();
    double maxY = hTotalEdep->GetMaximum();

    // Define and fit a Gaussian function with refined parameters
    TF1 *fitFunc = new TF1("fitFunc", "gaus", 0.1, 2.9); // Extended range for better fit
    fitFunc->SetParameters(maxY, mean, sigmaHist);  // Set initial values
    fitFunc->SetParNames("Amplitude", "Mean", "Sigma");

    // Perform fit with improved range
    hTotalEdep->Fit(fitFunc, "R");

    // Draw histogram with fit
    TCanvas *canvas = new TCanvas("canvas", "Total Energy Deposition", 800, 600);
    hTotalEdep->SetLineColor(kBlue);
    hTotalEdep->SetLineWidth(2);
    hTotalEdep->Draw();

    fitFunc->SetLineColor(kRed);
    fitFunc->SetLineWidth(3);  // Thicker line for visibility
    fitFunc->Draw("same");

    // Add legend
    TLegend *legend = new TLegend(0.075, 0.75, 0.95, 0.92); // Moved further right
    legend->SetTextSize(0.035);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0); // Transparent background
    legend->AddEntry(hTotalEdep, "Total Energy Deposition", "l");
    legend->AddEntry(fitFunc, "Gaussian Fit", "l");
    legend->Draw();

    // Save the canvas to a file
    canvas->SaveAs("TotalEdep_Spectrum_Fit_Optimized.png");

    // Clean up
    file->Close();
    delete file;
    delete canvas;
}
