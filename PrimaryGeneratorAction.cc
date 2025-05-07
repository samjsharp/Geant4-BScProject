#include "PrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

namespace B4 {

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    // Using G4ParticleGun
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    // Set the default particle as an electron
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "e-";
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
    fParticleGun->SetParticleDefinition(particle);

    // Set initial position at the center of the world
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // Set isotropic direction for electrons
    G4double theta = std::acos(1.0 - 2.0 * G4UniformRand()); // Uniform in cos(theta)
    G4double phi = G4UniformRand() * 2. * CLHEP::pi;
    
    G4double sinTheta = std::sin(theta);
    G4double cosTheta = std::cos(theta);
    G4double sinPhi = std::sin(phi);
    G4double cosPhi = std::cos(phi);

    G4ThreeVector direction(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta);
    fParticleGun->SetParticleMomentumDirection(direction);

    // Gaussian energy distribution for electrons
    G4double meanEnergy = 1.5 * MeV; // Center of Gaussian
    G4double sigmaEnergy = 0.5 * MeV; // Standard deviation

    G4double energy = -1.0;
    while (energy < 0.1 * MeV || energy > 3.0 * MeV) { // Ensure physical energy values
        energy = G4RandGauss::shoot(meanEnergy, sigmaEnergy);
    }
    
    fParticleGun->SetParticleEnergy(energy);

    // Generate primary vertex
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

} // namespace B4
