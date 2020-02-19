import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
 
df = pd.read_csv('../datafile/adjust.txt', header=None, delimiter=' ')
df2 = pd.read_csv('../datafile/betare.txt', header=None, delimiter='\n')

labels = df[0].values
beta = df2[0].values
font={'fontname':'Helvetica','size':10}
wordfont={'fontname':'Helvetica','size':7}
x = np.arange(len(labels))  # the label locations

fig, ax = plt.subplots()

def plotData(plt):
    #plt.plot(labels, infected, '-o',label = "predict")
    plt.plot(labels, beta,'-o', label = "beta")
    plt.legend(loc='upper right')
     
ax.set_ylabel('Infected/Recover cases')
ax.set_xlabel('Day',**font)
ax.set_title('Novel CoronaVirus cases Prediction')
ax.set_xticks(x)
ax.set_xticklabels(labels,rotation=90,**wordfont)


            
#autolabel(infected)
#autolabel(recover)
plotData(plt)

print('HI')
plt.show()
