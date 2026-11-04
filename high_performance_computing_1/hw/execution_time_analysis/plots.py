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
    {"file": "N_100.txt", "label": r"$N = 100$", "color": "#4477AA", "marker": "s"},
    {"file": "N_500.txt", "label": r"$N = 500$", "color": "#66CCEE", "marker": "^"},
    {"file": "N_1000.txt", "label": r"$N = 1000$", "color": "#228833", "marker": "D"},
    {"file": "N_1500.txt", "label": r"$N = 1500$", "color": "#CCBB44", "marker": "v"},
    {"file": "N_2500.txt", "label": r"$N = 2500$", "color": "#EE6677", "marker": "o"},
]

fig, ax = plt.subplots(figsize=(7, 5.8), dpi=300)

for ds in datasets:
    threads, time_ms = np.loadtxt(ds["file"], skiprows=1, unpack=True)

    # Submuestreo de marcadores según la cantidad total de puntos
    stride = 2 if len(threads) <= 20 else 10

    ax.plot(
        threads,
        time_ms,
        label=ds["label"],
        color=ds["color"],
        linewidth=1.1,
        marker=ds["marker"],
        markersize=3.8,
        markevery=stride,
        markerfacecolor="white",
        markeredgewidth=0.9,
    )

# Escala logarítmica para abarcar de 1 ms a 10^5 ms
ax.set_yscale("log")
ax.set_xlim(0, 103)
ax.set_ylim(1, 2.5e5)

ax.set_xlabel(r"Número de hilos")
ax.set_ylabel(r"Tiempo de ejecución (ms)")

ax.xaxis.set_major_locator(ticker.MultipleLocator(20))
ax.xaxis.set_minor_locator(ticker.MultipleLocator(5))

ax.yaxis.set_major_formatter(
    ticker.FuncFormatter(lambda y, _: f"${int(y):,}$" if y >= 1 else f"{y}")
)

ax.grid(True, which="major", linestyle="-", color="#d0d0d0")
ax.grid(True, which="minor", linestyle=":", color="#ebebeb")

ax.legend(
    frameon=True,
    framealpha=0.92,
    edgecolor="#cccccc",
    loc="upper right",
    handletextpad=0.4,
    borderpad=0.4,
)

plt.tight_layout(pad=0.4)

# plt.savefig("execution_time_all_matrices.pdf", format="pdf", bbox_inches="tight")
plt.savefig("execution_time_all_matrices.svg", format="svg", bbox_inches="tight")

plt.show()
