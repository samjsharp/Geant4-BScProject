#include <iostream>
#include "TFile.h"
#include "TTree.h"

void CalculateEfficiency() {
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

    // Counters
    Long64_t nEntries = tree->GetEntries();
    Long64_t uniqueHits = 0;
    Long64_t totalHits = 0;

    // Loop over all events
    for (Long64_t i = 0; i < nEntries; i++) {
        tree->GetEntry(i);
        int hitCount = 0;

        // Count how many detectors registered energy
        for (int j = 0; j < 6; j++) {
            if (SiEdep[j] > 0) {
                hitCount++;
            }
        }

        if (hitCount == 1) {  // Only count if the event was detected in one detector
            uniqueHits++;
        }

        if (hitCount > 0) {
            totalHits++;
        }
    }

    // Efficiency calculation
    double efficiency = (double)uniqueHits / (double)nEntries * 100.0;
    double detectionRate = (double)totalHits / (double)nEntries * 100.0;

    // Output results
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Total Events Simulated:  " << nEntries << std::endl;
    std::cout << "Total Events Detected:   " << totalHits << std::endl;
    std::cout << "Unique Single Detector Hits: " << uniqueHits << std::endl;
    std::cout << "Detection Rate (any detector): " << detectionRate << " %" << std::endl;
    std::cout << "Efficiency (unique hits only): " << efficiency << " %" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    // Clean up
    file->Close();
    delete file;
}
