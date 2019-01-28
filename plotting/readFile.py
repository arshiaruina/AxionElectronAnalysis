import matplotlib.pyplot as plt
import numpy as np
import math

x, y = np.loadtxt('resources/axion_gae_flux.txt', delimiter='\t', unpack=True)
#g = pow(10,-10)
g = pow(10,-12)
z = pow(g,2) * 3.82 * pow(10,30) * pow(x,3) / (np.exp(x/1.103)-1)
z = z * 50 * 365 * 86400 * pow(10,4)/ pow(10,20)
plt.plot(x,y, label='ABC processes')
plt.plot(x,z, label='Primakoff contribution')

plt.xlabel('Energy [keV]')
plt.ylabel('$d\Phi / d\omega$[10$^{20}$ keV$^{-1}$year$^{-1}$m$^{-2}$]')
#plt.ylabel('$d\Phi / d\omega$[keV$^{-1}$s$^{-1}$cm$^{-2}$]')
plt.title('Solar axion flux')
plt.legend()
plt.show() 
