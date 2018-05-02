import math
import numpy


def printW(W):
    for a in W:
        print("[")
        for b in a:
            print(b,",")
        print("],")


def softmax(X):
    if(isinstance(X[0],list) or isinstance(X[0],numpy.ndarray)):
        R=[]
        for L in X:
            #if(max(L)>=700):
                #print("Error max is :",max(L))
            P=[min(a,700) for a in L]
            U=[numpy.exp(a) for a in P]
            R.append([a/sum(U) for a in U])
        return R
    else:
        U=[math.exp(a) for a in X]
        return [a/sum(U) for a in U]
def ReLu(X):
    if(isinstance(X[0],list)):
        R=[]
        for L in X:
            L=[max(0,a) for a in L]
            R.append(L)
        return R
    else:
        return [math.max(0,a) for a in X]   


    
def logloss(R,labels):
    loss=0
    if(len(labels)!=len(R)):
        print("Lenght not matching")
        exit
    for k in range(len(R)):
        #print("error",R[k][labels[k]])
        try:
            loss+=math.log(R[k][labels[k]])
        except:
            print("Error log compute : ",R[k][labels[k]])
    return -1*loss

    