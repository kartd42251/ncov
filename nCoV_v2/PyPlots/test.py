import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
 
df = pd.read_csv('../datafile/adjust.txt', header=None, delimiter=' ')
df2 = pd.read_csv('../datafile/sampleorg.txt', header=None, delimiter=' ')

labels = df[0].values
infected = df[2].values
recover = df2[0].values
font={'fontname':'Helvetica','size':10}
wordfont={'fontname':'Helvetica','size':7}
x = np.arange(len(labels))  # the label locations

fig, ax = plt.subplots()

def plotData(plt, data):
 # x = [p[0] for p in data]
  #y = [p[1] for p in data]
     plt.plot(labels, infected, '-o')
     plt.plot(labels, recover, '-o')
     
ax.set_ylabel('Infected/Recover cases')
ax.set_xlabel('Day',**font)
ax.set_title('Novel CoronaVirus cases Prediction')
ax.set_xticks(x)
ax.set_xticklabels(labels,rotation=90,**wordfont)
#ax.legend()

def autolabel(index):
    M =0
    cnt =0
    """Attach a text label above each bar in *rects*, displaying its height."""
    for i in range (0,index.size):
        height = infected.item(int(i))
        if height>M:
            M=height
            print(M)
    for i in range (0,index.size):
        height = infected.item(int(i))
        cnt =cnt+1
        if height ==M:
            ax.annotate('{}'.format(height),
                    xy=(cnt,height),
                    xytext=(0, 1),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')
            
autolabel(infected)

plotData(plt, infected)
plotData(plt, recover)

plt.show()
