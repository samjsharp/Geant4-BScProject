import numpy as np
import matplotlib.pyplot as plt

# Data: Distance (mm) vs. Lost Events for different energies
distances = np.array([3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15])

lost_500keV = np.array([1770, 1858, 1937, 2024, 2113, 2179, 2265, 2332, 2399, 2472, 2530, 2602, 2661])
lost_1000keV = np.array([5599, 6353, 7023, 7705, 8424, 9152, 9843, 10524, 11193, 11897, 12565, 13272, 13909])
lost_5000keV = np.array([99090, 99139, 99171, 99206, 99254, 99290, 99320, 99350, 99381, 99406, 99427, 99455, 99475])

total_events = 100000  # Total simulated events per distance

# Calculate Efficiency: η = (Total - Lost) / Total * 100%
efficiency_500keV = ((total_events - lost_500keV) / total_events) * 100
efficiency_1000keV = ((total_events - lost_1000keV) / total_events) * 100
efficiency_5000keV = ((total_events - lost_5000keV) / total_events) * 100

# Uncertainty estimation: sqrt(N) / N * 100% (Poisson statistics)
error_500keV = (np.sqrt(total_events - lost_500keV) / total_events) * 100
error_1000keV = (np.sqrt(total_events - lost_1000keV) / total_events) * 100
error_5000keV = (np.sqrt(total_events - lost_5000keV) / total_events) * 100

# Create figure with multiple subplots
fig, axs = plt.subplots(2, 2, figsize=(12, 10))

# Main plot
axs[0, 0].errorbar(distances, efficiency_500keV, yerr=error_500keV, marker='o', linestyle='-', label='500 keV', color='blue', capsize=3)
axs[0, 0].errorbar(distances, efficiency_1000keV, yerr=error_1000keV, marker='s', linestyle='-', label='1000 keV', color='red', capsize=3)
axs[0, 0].errorbar(distances, efficiency_5000keV, yerr=error_5000keV, marker='^', linestyle='-', label='5000 keV', color='green', capsize=3)
axs[0, 0].set_xlabel("Detector Distance (mm)", fontsize=12)
axs[0, 0].set_ylabel("Detection Efficiency (%)", fontsize=12)
axs[0, 0].set_title("Detector Efficiency vs. Distance (All Energies)", fontsize=14)
axs[0, 0].legend()
axs[0, 0].grid(True)

# Zoomed-in plot for 500 keV
axs[0, 1].errorbar(distances, efficiency_500keV, yerr=error_500keV, marker='o', linestyle='-', color='blue', capsize=3)
axs[0, 1].set_xlabel("Detector Distance (mm)", fontsize=12)
axs[0, 1].set_ylabel("Efficiency (%)", fontsize=12)
axs[0, 1].set_title("500 keV Efficiency vs. Distance", fontsize=14)
axs[0, 1].grid(True)

# Zoomed-in plot for 1000 keV
axs[1, 0].errorbar(distances, efficiency_1000keV, yerr=error_1000keV, marker='s', linestyle='-', color='red', capsize=3)
axs[1, 0].set_xlabel("Detector Distance (mm)", fontsize=12)
axs[1, 0].set_ylabel("Efficiency (%)", fontsize=12)
axs[1, 0].set_title("1000 keV Efficiency vs. Distance", fontsize=14)
axs[1, 0].grid(True)

# Zoomed-in plot for 5000 keV
axs[1, 1].errorbar(distances, efficiency_5000keV, yerr=error_5000keV, marker='^', linestyle='-', color='green', capsize=3)
axs[1, 1].set_xlabel("Detector Distance (mm)", fontsize=12)
axs[1, 1].set_ylabel("Efficiency (%)", fontsize=12)
axs[1, 1].set_title("5000 keV Efficiency vs. Distance", fontsize=14)
axs[1, 1].grid(True)

# Adjust layout and save figure
plt.tight_layout()
plt.savefig("detector_efficiency_vs_distance.png", dpi=300)
plt.show()
