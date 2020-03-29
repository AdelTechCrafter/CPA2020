# -*- coding: utf-8 -*-
#import numpy as np
import matplotlib.pyplot as plt


def readcorrelation_1_2(fname,lx,ly):
    lx.clear()
    ly.clear()
    f = open(fname,'r')
    f1=f.readlines()
    #i=0
    for line in f1:
        #if(i%10==0):
        l=line.split()
        lx.append(float(l[0]))
        ly.append(int(l[1]))
        #i=i+1
    f.close()

def readcorrelation_3_4_5_6(fname,lx,ly):
    lx.clear()
    ly.clear()
    f = open(fname,'r')
    f1=f.readlines()
    #i=0
    for line in f1:
        #if(i%10==0):
        l=line.split()
        lx.append(float(l[0]))
        ly.append(float(l[1]))
        #i=i=1
    f.close()
    
def plot1(fname,lx,ly):
    readcorrelation_1_2(fname,lx,ly)
    plt.xlim(0, max(lx))
    plt.ylim(0, max(ly))
    plt.scatter(lx,ly, c='blue')
    plt.xlabel("PageRank with alpha=0.15")
    plt.ylabel("in-degree")
    plt.savefig('plot/plot1.png')
    plt.show()

def plot2(fname,lx,ly):
    readcorrelation_1_2(fname,lx,ly)
    plt.xlim(0, max(lx))
    plt.ylim(0, max(ly))
    plt.scatter(lx,ly, c='blue')
    plt.xlabel("PageRank with alpha=0.15")
    plt.ylabel("out-degree")
    plt.savefig('plot/plot2.png')
    plt.show()
    
def plot3(fname,lx,ly):
    readcorrelation_3_4_5_6(fname,lx,ly)
    plt.xlim(0, max(lx))
    plt.ylim(0, max(ly))
    plt.scatter(lx,ly, c='blue')
    plt.xlabel("PageRank with alpha=0.15")
    plt.ylabel("PageRank with alpha=0.1")
    plt.savefig('plot/plot3.png')
    plt.show()
    
def plot4(fname,lx,ly):
    readcorrelation_3_4_5_6(fname,lx,ly)
    plt.xlim(0, max(lx))
    plt.ylim(0, max(ly))
    plt.scatter(lx,ly, c='blue')
    plt.xlabel("PageRank with alpha=0.15")
    plt.ylabel("PageRank with alpha=0.2")
    plt.savefig('plot/plot4.png')
    plt.show()

def plot5(fname,lx,ly):
    readcorrelation_3_4_5_6(fname,lx,ly)
    plt.xlim(0, max(lx))
    plt.ylim(0, max(ly))
    plt.scatter(lx,ly, c='blue')
    plt.xlabel("PageRank with alpha=0.15")
    plt.ylabel("PageRank with alpha=0.5")
    plt.savefig('plot/plot5.png')
    plt.show()

def plot6(fname,lx,ly):
    readcorrelation_3_4_5_6(fname,lx,ly)
    plt.xlim(0, max(lx))
    plt.ylim(0, max(ly))
    plt.scatter(lx,ly, c='blue')
    plt.xlabel("PageRank with alpha=0.15")
    plt.ylabel("PageRank with alpha=0.9")
    plt.savefig('plot/plot6.png')
    plt.show()   

def plot1Logscale(fname,lx,ly):
    readcorrelation_1_2(fname,lx,ly)
    plt.xscale("log")
    plt.yscale("log")
    plt.xlim(min(lx), max(lx))
    plt.ylim(min(ly), max(ly))
    plt.scatter(lx,ly, c='blue')
    plt.xlabel("PageRank with alpha=0.15")
    plt.ylabel("in-degree")
    plt.savefig('plot/plot1LogScale.png')
    plt.show()

def plot2Logscale(fname,lx,ly):
    readcorrelation_1_2(fname,lx,ly)
    plt.xscale("log")
    plt.yscale("log")
    plt.xlim(min(lx), max(lx))
    plt.ylim(min(ly), max(ly))
    plt.scatter(lx,ly, c='blue')
    plt.xlabel("PageRank with alpha=0.15")
    plt.ylabel("out-degree")
    plt.savefig('plot/plot2LogScale.png')
    plt.show()
    
def plot3Logscale(fname,lx,ly):
    readcorrelation_3_4_5_6(fname,lx,ly)
    plt.xscale("log")
    plt.yscale("log")
    plt.xlim(min(lx), max(lx))
    plt.ylim(min(ly), max(ly))
    plt.scatter(lx,ly, c='blue')
    plt.xlabel("PageRank with alpha=0.15")
    plt.ylabel("PageRank with alpha=0.1")
    plt.savefig('plot/plot3LogScale.png')
    plt.show()
    
def plot4Logscale(fname,lx,ly):
    readcorrelation_3_4_5_6(fname,lx,ly)
    plt.xscale("log")
    plt.yscale("log")
    plt.xlim(min(lx), max(lx))
    plt.ylim(min(ly), max(ly))
    plt.scatter(lx,ly, c='blue')
    plt.xlabel("PageRank with alpha=0.15")
    plt.ylabel("PageRank with alpha=0.2")
    plt.savefig('plot/plot4LogScale.png')
    plt.show()

def plot5Logscale(fname,lx,ly):
    readcorrelation_3_4_5_6(fname,lx,ly)
    plt.xscale("log")
    plt.yscale("log")
    plt.xlim(min(lx), max(lx))
    plt.ylim(min(ly), max(ly))
    plt.scatter(lx,ly, c='blue')
    plt.xlabel("PageRank with alpha=0.15")
    plt.ylabel("PageRank with alpha=0.5")
    plt.savefig('plot/plot5LogScale.png')
    plt.show()

def plot6Logscale(fname,lx,ly):
    readcorrelation_3_4_5_6(fname,lx,ly)
    plt.xscale("log")
    plt.yscale("log")
    plt.xlim(min(lx), max(lx))
    plt.ylim(min(ly), max(ly))
    plt.scatter(lx,ly, c='blue')
    plt.xlabel("PageRank with alpha=0.15")
    plt.ylabel("PageRank with alpha=0.9")
    plt.savefig('plot/plot6LogScale.png')
    plt.show()
         
def main():
    lx=[]
    ly=[]
    #plot1("correlation1.txt",lx,ly)
    #plot2("correlation2.txt",lx,ly)
    #plot3("correlation3.txt",lx,ly)
    #plot4("correlation4.txt",lx,ly)
    #plot5("correlation5.txt",lx,ly)
    #plot6("correlation6.txt",lx,ly)
    plot1Logscale("correlation1.txt",lx,ly)
    #plot2Logscale("correlation2.txt",lx,ly)
    #plot3Logscale("correlation3.txt",lx,ly)
    #plot4Logscale("correlation4.txt",lx,ly)
    #plot5Logscale("correlation5.txt",lx,ly)
    #plot6Logscale("correlation6.txt",lx,ly)
    
if __name__ == "__main__":
    # execute only if run as a script
    main()
    

