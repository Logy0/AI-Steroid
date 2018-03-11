import numpy as np
import math
import Matrix
from Matrix import lineadd
from Matrix import Transpose
from Matrix import Summ

import FFNNSGD

from FFNNSGD import softmax
from FFNNSGD import printW

from GenTrain import GenSort
from GenTrain import init

import matplotlib.pyplot as plt

import socket
import time

def Qf():
    for e in range(3,29):
        for k in range(20):
            print(Q[e][k*20:(k+1)*20:])
        print(A[e])
        print("")
        


HOST = '127.0.0.1'    # The remote host
PORT = 55001          # The same port as used by the server


s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
nNN=600
Ns=init(20*20,9,nNN)

#W=np.random.rand(20*20,8)
#B=np.random.rand(8)
S=[]
GRID_X=20
GRID_Y=20

sec=0
D=[]
Dy=[]
Q=[]
A=[]
timeout=0
for start in range(0,150):
    print("--------------",start,"--------------")
    for k in range(len(Ns)):
        
        W=Ns[k][0]
        B=Ns[k][1]
        score=0
        vx=10
        vy=10
        game=1
        
        while(game!=0):
            X=[]
            count=0
            t1=time.clock()
            while(count<20 and timeout==0):                
                d = s.recv(20)
                count+=1
                if(time.clock()-t1>=1.0):
                    timeout=1
                for a in d:
                    X.append(a)
            
            #if(sum(X)<29):
            #print("S",sum(X))
            
            if(timeout==0):
                C=np.dot(X,W)
                R=softmax(np.add(C,B))
                action=R.index(max(R))
                acc=(1-action%2)+1
                if(action//2==0):
                    vx-=acc
                elif(action//2==1):
                    vy+=acc
                elif(action//2==2):
                    vx+=acc
                elif(action//2==3):
                    vy-=acc
                if(vx>=GRID_X):
                    vx=0
                if(vy>=GRID_Y):
                    vy=0
                if(vx<0):
                    vx=GRID_X-1
                if(vy<0):
                    vy=GRID_Y-1
                    
                sec=1
                if(start==0):
                    D.append(vx)
                    Dy.append(vy)
                if(X[vy*20+vx]!=0 and score>1):
                    game=0
                else:
                    X[vy*20+vx]=-2
                    score+=sec
            else:
                timeout=0
            
        Ns[k][2]=score
        S.append(score)
        print(score)
    print("Max --------- :",max(S[len(S)-nNN::]))
    keepbest=15
    Ns=GenSort(Ns,keepbest)

plt.plot(S)
