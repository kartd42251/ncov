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
    temp1 = data.split()
    Sdata.append(int(temp1[2]))
    Rdata.append(int(temp1[4]))

Sdata2 = []
Rdata2 = []
for data in datas2:
    temp2 = data.split()
    Sdata2.append(float(temp2[2]))
    #Rdata2.append(float(b[3])) 

plt.plot(Sdata,label = 'real infected')
plt.plot(Rdata, label = 'real rocover')
plt.plot(Sdata2,label = 'predict infected')
plt.legend(loc = 'best')
plt.axis([0,80,0,8000])
plt.savefig('./predict.png')
plt.show()

