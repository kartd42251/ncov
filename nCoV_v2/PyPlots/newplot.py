import matplotlib.pyplot as plt

addr1 = '../datafile/KR.txt'
addr2 = '../datafile/adjust.txt'

fobj = open(addr1)
datas = fobj.readlines()
fobj.close()
fobj2 = open(addr2)
datas2 = fobj2.readlines()
fobj2.close()

Sdata = []
Rdata = []

for data in datas:
    a = data.split()
    Sdata.append(int(a[2]))
    Rdata.append(int(a[4]))

Sdata2 = []
Rdata2 = []
for data2 in datas2:
    b = data2.split()
    Sdata2.append(float(b[2]))
    #Rdata2.append(float(b[3])) 

plt.plot(Sdata,label = 'real infected')
plt.plot(Rdata, label = 'real rocover')
plt.plot(Sdata2,label = 'predict infected')
plt.legend(loc = 'best')
plt.axis([0,80,0,8000])
plt.savefig('../predict.png')
plt.show()

