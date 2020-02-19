import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

df = pd.read_csv('../datafile/adjust.txt', header=None, delimiter=' ')

labels = df[0].values
infected = df[2].values
recover = df[3].values
font={'fontname':'Helvetica','size':10}
wordfont={'fontname':'Helvetica','size':6}

x = np.arange(len(labels))  # the label locations
width = 0.35  # the width of the bars

fig, ax = plt.subplots()
rects1 = ax.bar(x - width/2, infected, width, label='Infected')
rects2 = ax.bar(x + width/2, recover, width, label='Recover')

# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_ylabel('Infected/Recover cases')
ax.set_xlabel('Day',**font)
ax.set_title('Novel CoronaVirus cases Prediction')
ax.set_xticks(x)
ax.set_xticklabels(labels,**wordfont)
ax.legend()


def autolabel(rects):
    M =0
    """Attach a text label above each bar in *rects*, displaying its height."""
    for rect in rects:
        height = rect.get_height()
        if height>M:
            M=height
    for rect in rects:
        height = rect.get_height()        
        if height ==M:
            ax.annotate('{}'.format(height),
                    xy=(rect.get_x() + rect.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')


autolabel(rects1)
autolabel(rects2)

fig.tight_layout()

plt.show()
