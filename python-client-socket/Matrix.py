import math
from math import *
from random import *
from math import floor

def lineadd(a,b):
    d=[]
    for i in range(min(len(a),len(b))):
        d.append(a[i]+b[i])
    return d
def Copy(a):
    m=[]
    for k in range(len(a)):
        m.append([a[k][i] for i in range(len(a[k]))])
    return m
def Column(M,k):
    if(k>=len(M[0])):
        return -1
    return [M[p][k] for p in range(len(M))]
    
def Summ(a,b):
    if(len(a)!=len(b) or len(a[0])!=len(b[0])):
        print("Different sizes")
        return -1
    for k in range(len(a)):
        for i in range(len(a[0])):
            a[k][i]+=b[k][i]
    return a
def Transpose(a):
    l=len(a)
    c=len(a[0])
    b=[]
    for j in range(c):
        x=[]
        for i in range(l):
            x.append(a[i][j])
        b.append(x)
    return b
def Mult(a,b):
    l1=len(a)
    c1=len(a[0])
    l2=len(b)
    c2=len(b[0])
    if(c1!=l2):
        return -1
    d=[]    
    for k in range(l1):
        d.append(list([0]*c2))
    for k in range(c2):
        for i in range(l1):
            s=0
            for j in range(l2):
                s+=a[i][j]*b[j][k]
            d[i][k]=s            
    return d
def Comatrice(a,i,j):
    l=len(a)
    c=len(a[0])
    if(l!=c):
        print("Not a square matrix")
        return NaN
    b=[]
    for k in range(l):
        if(k!=i):
            x=[]
            for m in range(c):
                if(m!=j):
                    x.append(a[k][m])
            b.append(x)
    return b
def Determinant(a):
    l=len(a)
    c=len(a[0])
    if(l!=c):
        print("Not a square matrix")
        return NaN
    if(l==2):
        return a[0][0]*a[1][1]-a[0][1]*a[1][0] 
    det=0
    for k in range(c):
        det+=(-1)**(k)*a[0][k]*Determinant(Comatrice(a,0,k))
    return det
def Print(a):
    for k in range(len(a)):
        print(a[k])
def Permute(A,i):
    print("Exchanged")
    for k in range(i+1,len(A)):
        if(A[k][i]!=0):
            l1=A[i]
            A[i]=A[k]
            A[k]=l1
            return A
    print("Cannot exchange lines")
    return -1
def Gen(n):
    return [[randint(-10,10) for i in range(n)]for j in range(n)]

def Id(LA,n):
    M=[]
    for k in range(n):
        M.append([[0,LA][i==k] for i in range(n)])
    return M
def Diag(Mat,La):
       M=[]
       n=len(Mat)
       for k in range(n):
           M.append([[0,La*Mat[k][i]][i==k] for i in range(n)])
       return M
def Smult(a,alpha):
    for i in range(len(a)):
        for j in range(len(a[i])):
            a[i][j]*=alpha
    return a