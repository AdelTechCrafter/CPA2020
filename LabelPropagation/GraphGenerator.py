# -*- coding: utf-8 -*-
"""
Created on Sun Mar 29 16:08:06 2020

@author: Adel
"""

import random as r
import networkx as nx
import matplotlib.pyplot as plt


# To execute:
# "python mkgraph.py edgelist.txt".
# "edgelist.txt" is the output filename


def generateGraph(nbnodes,p, q):
    g = nx.Graph()
    for n1 in range(nbnodes):
        for n2 in range(n1, nbnodes):
            if(n1//100 == n2//100):
                if(r.random() < p ):
                    g.add_edge(n1, n2)
            elif(r.random() < q ):
                g.add_edge(n1,n2)
    return g

def generatecommunities(g):
    color_map=[]
    for node in g:
        if node//100 < 1:
            color_map.append('blue')
        elif node//100<2: 
            color_map.append('green')
        elif(node//100<3):
            color_map.append('red')
        else:
            color_map.append('purple')
    return color_map

def generatefigure(name,p,q):
    g =generateGraph(400,p,q)
    color_map=generatecommunities(g)
    nx.write_edgelist(g,name+".txt", delimiter =' ', data=False)
    nx.draw(g, node_color=color_map,node_size=20)
    plt.title("p= "+str(p)+" and q= "+str(q))
    plt.savefig(name+".png", format='png')
    plt.show()
    #plt.close()

def readcommunities(filename):
  f = open(filename,'r')
  f1=f.readlines()
  labels={}
  for line in f1:
      l=line.split()
      labels[l[0]]=int(l[1])
  f.close()
  return labels

def readgraph(filename):
    g = nx.Graph()
    g.add_edges_from(nx.read_edgelist(filename).edges())
    return g

def generatecommunitiesWithLabel(name,filegraph,filelabels):
    labels=readcommunities(filelabels)
    g=readgraph(filegraph)
    colors = [labels[node] for node in g.nodes()]
    nx.draw(g, node_color=colors, node_size=20)
    plt.title("label propagation with file "+filegraph)
    plt.savefig(name+".png", format='png')
    plt.show()
    
def main():
   #generatefigure("generatedGraphs/graph1",0.1,0.0001)
   #generatefigure("generatedGraphs/graph2",0.1,0.001)
   #generatefigure("generatedGraphs/graph3",0.1,0.05)
   generatecommunitiesWithLabel("generatedPropagations/labelprogation1","generatedGraphs/graph1.txt","labelpropagationgraph1.txt")
   generatecommunitiesWithLabel("generatedPropagations/labelprogation2","generatedGraphs/graph2.txt","labelpropagationgraph2.txt")
   generatecommunitiesWithLabel("generatedPropagations/labelprogation3","generatedGraphs/graph3.txt","labelpropagationgraph3.txt")
   #generatecommunitiesWithLabel("labelprogationEmail","F:\\Programmation\\WorkspaceCPA2020\\ressources\\email-Eu-core.txt","labelpropagationemail.txt")

if __name__ == "__main__":
    # execute only if run as a script
    main()

