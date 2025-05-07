import ROOT
import matplotlib.pyplot as plt
import numpy as np

# Load ROOT file
root_file = ROOT.TFile.Open("Multiplicity.root")
hist = root_file.Get("hMultiplicity")

# Extract bin data
n_bins = hist.GetNbinsX()
multiplicity_values = np.array([hist.GetBinContent(i) for i in range(1, n_bins + 1)])
multiplicity_labels = np.arange(n_bins)  # Multiplicity bins (0-6)

# Plot using Matplotlib
plt.figure(figsize=(8, 6))
plt.bar(multiplicity_labels, multiplicity_values, color="blue", alpha=0.7, edgecolor="black")
plt.xlabel("Multiplicity (Number of Detectors Hit)")
plt.ylabel("Event Count")
plt.title("Multiplicity Distribution")
plt.xticks(multiplicity_labels)
plt.grid(axis="y", linestyle="--", alpha=0.7)

# Save and Show Plot
plt.savefig("Multiplicity_Plot.png", dpi=300)
plt.show()
