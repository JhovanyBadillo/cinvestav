import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

plt.rcParams.update(
    {
        "font.family": "serif",
        "font.size": 9,
        "axes.labelsize": 10,
        "axes.titlesize": 10,
        "xtick.labelsize": 8.5,
        "ytick.labelsize": 8.5,
        "legend.fontsize": 8,
        "mathtext.fontset": "cm",
        "axes.linewidth": 0.8,
        "grid.linewidth": 0.5,
        "grid.alpha": 0.45,
    }
)

datasets = [
    {
        "file": "data_amdahl_turbo_boost_enabled.txt",
        "label": r"Amdahl",
        "color": "#2A2288",
        "marker": "^",
    },
    {
        "file": "data_turbo_boost_enabled.txt",
        "label": r"Turbo Boost habilitado",
        "color": "#228833",
        "marker": "o",
    },
    {
        "file": "data_turbo_boost_disabled.txt",
        "label": r"Turbo Boost deshabilitado",
        "color": "#886122",
        "marker": "*",
    },
]

fig, ax = plt.subplots(figsize=(7, 5.8), dpi=300)

for ds in datasets:
    threads, time_s = np.loadtxt(ds["file"], skiprows=0, unpack=True)

    ax.plot(
        threads,
        time_s,
        label=ds["label"],
        color=ds["color"],
        linewidth=1.1,
        marker=ds["marker"],
        markersize=3.8,
        markerfacecolor="white",
        markeredgewidth=0.9,
    )

ax.set_xlim(1, 8)
ax.set_ylim(0, 1.4)

ax.set_xlabel(r"Número de hilos")
ax.set_ylabel(r"Aceleración")

ax.xaxis.set_major_locator(ticker.MultipleLocator(1))
ax.xaxis.set_minor_locator(ticker.MultipleLocator(1))

ax.grid(True, which="major", linestyle="-", color="#d0d0d0")
ax.grid(True, which="minor", linestyle=":", color="#ebebeb")

ax.legend(
    frameon=True,
    framealpha=0.92,
    edgecolor="#cccccc",
    loc="lower right",
    handletextpad=0.4,
    borderpad=0.4,
)

plt.tight_layout(pad=0.4)

plt.savefig("amdahl.svg", format="svg", bbox_inches="tight")

plt.show()
