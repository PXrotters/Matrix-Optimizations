import matplotlib.pyplot as plt

N_values = [200, 400, 600, 1000, 1200]

time_neopt = [0.096525, 0.794986, 2.587077, 11.969311, 21.755663]
time_opt_m = [0.059677, 0.463116, 1.549367, 7.142748, 12.421175]
time_blas = [0.008355, 0.066546, 0.199384, 0.839092, 1.430622]

plt.figure(figsize=(10, 6))

plt.plot(N_values, time_neopt, label='Neopt')
plt.plot(N_values, time_opt_m, label='Opt_m')
plt.plot(N_values, time_blas, label='BLAS')

plt.title('Performances Graphic')
plt.xlabel('N = Size of the Square Matrix')
plt.ylabel('Elapsed time(seconds)')

plt.ylim(0, 25)
plt.yticks(range(0, 26, 5))

plt.legend()
plt.savefig('performance.png')