import uproot
import numpy as np
import matplotlib.pyplot as plt

# Define ROOT files with corresponding colors and energy levels
files = {
    "B450_2_smeared.root": ("black", 50),
    "B4100_2_smeared.root": ("red", 100),
    "B4200_2_smeared.root": ("green", 200),
    "B4300_2_smeared.root": ("yellow", 300),
    "B4500_2_smeared.root": ("blue", 500),
    "B41000_2_smeared.root": ("magenta", 1000),
    "B45000_2_smeared.root": ("cyan", 5000)
}

labels = ["50 keV", "100 keV", "200 keV", "300 keV", "500 keV", "1000 keV", "5000 keV"]

# Adjusted opacity scaling
min_opacity = 0.4  # Higher minimum opacity to keep lower energy lines visible
max_opacity = 1.0  # Maximum opacity for highest energy

# Normalizing opacities based on energy values
energies = np.array([energy for _, energy in files.values()])
normalized_opacities = (energies - energies.min()) / (energies.max() - energies.min())
opacities = min_opacity + normalized_opacities * (max_opacity - min_opacity)

# Plot the full spectrum
fig, ax_main = plt.subplots(figsize=(12, 6))

for (file, (color, energy)), opacity, label in zip(files.items(), opacities, labels):
    # Open ROOT file with uproot
    with uproot.open(file) as root_file:
        tree = root_file["B4;1"]  # Ensure correct tree name

        # Extract data from the smeared branch
        values = tree["Eabs_smeared"].array(library="np")

    # Define binning similar to ROOT histograms
    bins = np.linspace(0.0, 6.0, 200)  # Ensure x-axis starts exactly at 0.0 MeV

    # Compute histogram
    counts, bin_edges = np.histogram(values, bins=bins)

    # Remove counts below 10
    counts[counts < 0.10] = 0

    # Plot histogram with adjusted opacity
    ax_main.step(bin_edges[:-1], counts, where='mid', color=color, alpha=opacity, label=label, linewidth=1.5)

# Plot settings for main plot
ax_main.set_yscale("log")  # Log scale for Y-axis
ax_main.set_xlabel("Energy (MeV)")
ax_main.set_ylabel("Counts")
ax_main.set_title("Combined Gaussian Smeared Spectra for a 2mm thick Detector")
ax_main.set_xlim(0, 6.0)  # Ensure x-axis starts exactly at 0
ax_main.legend()
ax_main.grid(True, which="both", linestyle="--", linewidth=0.5)

# Save full spectrum plot
plt.savefig("Combined_Spectrum.png")
plt.show()

# Now create a separate zoomed-in plot for low-energy range < 500 keV
fig_zoom, ax_zoom = plt.subplots(figsize=(12, 6))

for (file, (color, energy)), opacity, label in zip(files.items(), opacities, labels):
    if energy <= 500:  # Only plot energies below 500 keV
        # Open ROOT file with uproot
        with uproot.open(file) as root_file:
            tree = root_file["B4;1"]  # Ensure correct tree name

            # Extract data from the smeared branch
            values = tree["Eabs_smeared"].array(library="np")

        # Define binning similar to ROOT histograms
        bins = np.linspace(0.0, 0.5, 100)  # Zoomed-in range below 500 keV

        # Compute histogram
        counts, bin_edges = np.histogram(values, bins=bins)

        # Remove counts below 10
        counts[counts < 0.001] = 0

        # Plot histogram with adjusted opacity
        ax_zoom.step(bin_edges[:-1], counts, where='mid', color=color, alpha=opacity, label=label, linewidth=1.5)

# Zoomed-in plot settings
ax_zoom.set_yscale("log")
ax_zoom.set_xlabel("Energy (MeV)")
ax_zoom.set_ylabel("Counts")
ax_zoom.set_title("Zoomed-in Gaussian Smeared Spectra (<500 keV) for a 2mm Detector")
ax_zoom.set_xlim(0, 0.5)
ax_zoom.legend()
ax_zoom.grid(True, linestyle="--", linewidth=0.5)

# Save zoomed-in spectrum plot
plt.savefig("Zoomed_Spectrum.png")
plt.show()
