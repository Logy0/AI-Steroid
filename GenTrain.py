import random
import numpy as np
def init(a,b,n):
    NN=[]
    for k in range(n):
        NN.append([np.random.rand(20*20,9),np.random.rand(9),0])
    return NN
    
def train(W,B,pkeep,sig):
    Wn=len(W)
    Wp=len(W[0])
    Bn=len(B)
    for k in range(Wn):
        for i in range(Wp):
            if(random.random()<pkeep):
                W[k][i]+=random.normalvariate(0,sig)
                W[k][i]=max(min(2.0,W[k][i]),-2.0)
    
    for k in range(Bn):
        if(random.random()<pkeep):
            B[k]+=random.normalvariate(0,sig)
            B[k]=max(min(2.0,B[k]),-2.0)
    return [W,B,0]
def breed(ParentA,ParentB):
    WC=[]
    BC=[]
    WA=ParentA[0]
    WB=ParentB[0]
    BA=ParentA[1]
    BB=ParentB[1]
    for k in range(len(WA)):
        WC.append([[WA[k][i],WB[k][i]][random.randint(0,1)] for i in range(len(WA[0]))])
    BC=[[BA[i],BB[i]][random.randint(0,1)] for i in range(len(BA))]
    Child=[WC,BC,0]
    return Child
    
def CreateChild(Ns,nCh):
    Child=[]
    
    for k in range(nCh):
        a=random.randint(0,len(Ns)-1)
        b=random.randint(0,len(Ns)-1)
        while(b==a):
            b=random.randint(0,len(Ns)-1)
        Child.append(breed(Ns[a],Ns[b]))
    return Child
    
def GenSort(Ns,keepbest):
    Ns=sorted(Ns,key=lambda a:a[2],reverse=True)
    n=len(Ns)
    Ns=Ns[0:keepbest:]
    Child=CreateChild(Ns,n-keepbest)
    Ns+=Child
    for k in range(keepbest,n):
        Ns[k]=train(Ns[k][0],Ns[k][1],0.01,-0.02)
    return Ns