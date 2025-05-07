# Re-attempting plot generation

# Import necessary libraries
import numpy as np
import matplotlib.pyplot as plt

# Define Gaussian parameters for the energy distribution
mean_energy = 1.5  # MeV
sigma_energy = 0.5  # MeV
num_samples = 100000  # Number of electrons

# Generate Gaussian-distributed energies
energies = np.random.normal(mean_energy, sigma_energy, num_samples)

# Filter energies to ensure they remain within 0 - 3 MeV
energies = energies[(energies >= 0) & (energies <= 3)]

# Create histogram of the simulated distribution
plt.figure(figsize=(8, 5))
plt.hist(energies, bins=100, density=True, alpha=0.6, color='b', edgecolor='black', label='Simulated Energy Distribution')

# Generate theoretical Gaussian curve for comparison
x = np.linspace(0, 3, 1000)
gaussian = (1 / (sigma_energy * np.sqrt(2 * np.pi))) * np.exp(-0.5 * ((x - mean_energy) / sigma_energy) ** 2)
plt.plot(x, gaussian, 'r--', label='Theoretical Gaussian')

# Set labels and title
plt.xlabel('Electron Energy (MeV)')
plt.ylabel('Probability Density')
plt.title('Expected Energy Distribution of Simulated Electrons')
plt.legend()
plt.grid(True)

# Show plot
plt.show()
