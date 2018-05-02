import random
import numpy as np
def init(a,b,n):
    NN=[]
    for k in range(n):
        NN.append([np.random.rand(20*20,9),np.random.rand(9),np.random.rand(9,9),np.random.rand(9),0])
    return NN
    
def train(N,pkeep,sig):
    for k in range(len(N[0])):
        for i in range(len(N[0][0])):
            if(random.random()<pkeep):
                N[0][k][i]+=random.normalvariate(0,sig)
                N[0][k][i]=max(min(2.0,N[0][k][i]),-2.0)
    
    for k in range(len(N[1])):
        if(random.random()<pkeep):
            N[1][k]+=random.normalvariate(0,sig)
            N[1][k]=max(min(2.0,N[1][k]),-2.0)
    
    for k in range(len(N[2])):
        for i in range(len(N[2][0])):
            if(random.random()<pkeep):
                N[2][k][i]+=random.normalvariate(0,sig)
                N[2][k][i]=max(min(2.0,N[0][k][i]),-2.0)
    
    for k in range(len(N[3])):
        if(random.random()<pkeep):
            N[3][k]+=random.normalvariate(0,sig)
            N[3][k]=max(min(2.0,N[3][k]),-2.0)

    
def breed(ParentA,ParentB):
    WC1=[]
    BC1=[]
    WA=ParentA[0]
    WB=ParentB[0]
    BA=ParentA[1]
    BB=ParentB[1]
    BC1=[[BA[i],BB[i]][random.randint(0,1)] for i in range(len(BA))]
    for k in range(len(WA)):
        WC1.append([[WA[k][i],WB[k][i]][random.randint(0,1)] for i in range(len(WA[0]))])
    WC2=[]
    BC2=[]
    """
    WA=ParentA[2]
    WB=ParentB[2]
    BA=ParentA[3]
    BB=ParentB[3]
    BC2=[[BA[i],BB[i]][random.randint(0,1)] for i in range(len(BA))]
    for k in range(len(WA)):
        WC2.append([[WA[k][i],WB[k][i]][random.randint(0,1)] for i in range(len(WA[0]))])
    """
    Child=[WC1,BC1,WC2,BC2,0]
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
    Ns=sorted(Ns,key=lambda a:a[4],reverse=True)
    n=len(Ns)
    Ns=Ns[0:keepbest:]
    Child=CreateChild(Ns,n-keepbest)
    Ns+=Child
    for k in range(keepbest,n):
        train(Ns[k],0.01,-0.02)
    return Ns