#Reads csv file from load cell data and produces graph of filtered data
#Based on the code from Shreeyam Kacker for the 02/02/2020 test fire
#Last updated: 03/03/2020
import numpy as np
from scipy import signal
import matplotlib.pyplot as plt

def moving_average(a, n=3) :
    ret = np.cumsum(a, dtype=float)
    ret[n:] = ret[n:] - ret[:-n]
    return ret[n - 1:] / n


#-----------------------VARIABLES------------------
fname = 'sensor_data_full.csv'



g = 9.81
test_mass = 1.03    #Mass (kg) of test weight used to calibrate load cell
timeoffset = 235e3  #Time offset (ms) for when plot data begins
testduration = 4000 #Duration (ms) of test fire

cell = 3            #Load cell to pull data from
mov_avg_samp = 15   #Number of samples for moving average of data
zero_start = 0      #Starting index for zeroing out measurements
zero_end = 20       #Ending index for zeroing out measurements
calib_start = 24791 #Starting index for calibrated mass measurements
calib_end = 25025   #Ending index for calibrated mass measurements

graphtitle = 'G60 Red Lightning Static Fire 02-02-2020'
graphxlabel = 'Time [ms]'
graphylabel = 'Force [kN]'

#-----------------------PROGRAM--------------------
#Data import from csv into numpy array
data = np.genfromtxt(fname, delimiter=',')

time = data[:,14]
load1 = data[:,10]
load2 = data[:,11]
load3 = data[:,12]
load4 = data[:,13]

#Select load cell to use
if cell == 1:
  load = load1
elif cell == 2:  
  load = load2
elif cell == 3:  
  load = load3
else:
  load = load4


#Data processing and filtering
mass = moving_average(load, mov_avg_samp)
b, a = signal.butter(2, .1, 'low')
filt = signal.filtfilt(b, a, load)

zero = np.average(load3[zero_start:zero_end]) #Load cell value for 0 mass
calib_mass = np.average(load3[calib_start:calib_end])                      #Load cell value for calibrated mass
forcepercount = (calib_mass - zero)/test_mass #Ratio to convert raw load cell measurement into force in kN


#Plot data
plt.plot(time[mov_avg_samp-1:]-timeoffset, g*(mass-zero)/forcepercount, 'b-', linewidth=2)
plt.xlim(0,testduration)
plt.grid(True)
plt.title(graphtitle)
plt.xlabel(graphxlabel)
plt.ylabel(graphylabel)
plt.show()