#!/bin/env python



#dy1jets_lo.txt:After filter: final cross section = 1.013e+03 +- 7.822e-01 pb
#dy2jets_lo.txt:After filter: final cross section = 3.347e+02 +- 2.643e-01 pb
#dy3jets_lo.txt:After filter: final cross section = 1.024e+02 +- 1.362e-01 pb
#dy4jets_lo.txt:After filter: final cross section = 5.445e+01 +- 4.216e-02 pb
#dyjets_lo.txt:After filter: final cross section = 4.959e+03 +- 2.627e+00 pb

lo_njets = 4.959e+03
lo_1jets = 1.013e+03
lo_2jets = 3.347e+02
lo_3jets = 1.024e+02
lo_4jets = 5.445e+01

lo_0jets = lo_njets-lo_1jets-lo_2jets-lo_3jets-lo_4jets


print "lo_njets=",lo_njets
print "lo_0jets=",lo_0jets
print "lo_1jets=",lo_1jets
print "lo_2jets=",lo_2jets
print "lo_3jets=",lo_3jets
print "lo_4jets=",lo_4jets


# print results:
#njets= 4959.0
#0jets= 3454.45
#1jets= 1013.0
#2jets= 334.7
#3jets= 102.4
#4jets= 54.45 

nlo_njets=1921.8*3
print "nlo_njets=",nlo_njets

r_nlo_lo_njets = nlo_njets/lo_njets

print "r_nlo_lo_njets=",r_nlo_lo_njets


