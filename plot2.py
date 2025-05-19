import matplotlib.pyplot as plt
import numpy as np

# Date performante
N_values    = [200, 400, 600, 1000, 1200]
time_neopt  = [0.096525, 0.794986, 2.587077, 11.969311, 21.755663]
time_opt_m  = [0.059677, 0.463116, 1.549367,  7.142748, 12.421175]
time_blas   = [0.008355, 0.066546, 0.199384,  0.839092,  1.430622]

# Date Cachegrind
variants        = ['Neoptimizata','Optimizata','BLAS']
I_refs          = np.array([6709150250,5609373607, 487143110])
D_refs          = np.array([3612136153,2253321174, 179193829])
D1_miss_rate    = np.array([5.9,0.8,9.7])
LLd_misses      = np.array([83033,123009,139610])
Branches        = np.array([261655355,82289891,9273240])
Mispred_rate    = np.array([0.3,5.3,1.5])
x = np.arange(len(variants))
w = 0.13

# Cream figura cu 2 linii de grafice
fig, (ax1, ax2) = plt.subplots(2,1, figsize=(12,10))

# → Subplot 1: performante
ax1.plot(N_values, time_neopt, marker='o', label='Neopt')
ax1.plot(N_values, time_opt_m, marker='o', label='Opt_m')
ax1.plot(N_values, time_blas,  marker='o', label='BLAS')
ax1.set_title('Performante în functie de dimensiunea matricii')
ax1.set_xlabel('N = dimensiunea matricei patratice')
ax1.set_ylabel('Timp executie (s)')
ax1.set_ylim(0,25)
ax1.set_yticks(range(0,26,5))
ax1.legend()

# → Subplot 2: metrice Cachegrind (normalizate)
ax2.bar(x-2.5*w, I_refs/1e9,   w, label='I refs (×10⁹)')
ax2.bar(x-1.5*w, D_refs/1e9,   w, label='D refs (×10⁹)')
ax2.bar(x-0.5*w, D1_miss_rate, w, label='Miss rate D1 (%)')
ax2.bar(x+0.5*w, LLd_misses/1e4, w, label='LLd misses (×10⁴)')
ax2.bar(x+1.5*w, Branches/1e8, w, label='Branches (×10⁸)')
ax2.bar(x+2.5*w, Mispred_rate, w, label='Branch mispred (%)')
ax2.set_title('Metrice Cachegrind comparativ')
ax2.set_xticks(x)
ax2.set_xticklabels(variants)
ax2.set_ylabel('Valori normalizate')
ax2.legend(loc='upper left', bbox_to_anchor=(1,1))

plt.tight_layout()
fig.savefig('grafice.png')
plt.show()
