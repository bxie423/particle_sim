# createplot.py
# Takes an output coordinates file from the physics simulation and creates
# 2D and 3D plots using Matplotlib.

import sys
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

inputname = "OUTPUTCOORDS"
outname = sys.argv[1]

# class definitions

class simulation():
    def __init__(self):
        self.particles = {}
        self.count = 0
    def new_particle(self, num):
        self.particles[num] = particle()
    def update_particle(self, num, x, y, z):
        self.particles[num].add_coord(x, y, z)
        self.count += 1
    def total_steps(self):
        return self.count // len(self.particles.items())

class particle():
    def __init__(self):
        self.coordlist = []
        self.coords = 0
    def add_coord(self, x, y, z):
        self.coordlist.append([x, y, z])
        self.coords += 1

# function definitions

def plot2D(simulation, xidx, yidx):
    names = { 0:'x', 1:'y', 2:'z' }

    fig = plt.figure(figsize=(10, 10), dpi=300)
    ax = fig.add_subplot(111)
    for i in range(0, nump):
        xlist = []
        ylist = []
        x2list = []
        y2list = []
        for j in range(0, total):
            xlist.append(simulation.particles[i].coordlist[j][xidx])
            ylist.append(simulation.particles[i].coordlist[j][yidx])
            if not j % (total // 10):
                x2list.append(simulation.particles[i].coordlist[j][xidx])
                y2list.append(simulation.particles[i].coordlist[j][yidx])
        ax.plot(xlist, ylist, color=colorarray[i % 8])
        ax.scatter(x2list, y2list, color=colorarray[i % 8], marker='s', s=5)
        ax.scatter(xlist[total - 1], ylist[total - 1], color=colorarray[i % 8], s=25)
    ax.set_xlabel(names[xidx], fontsize=12)
    ax.set_ylabel(names[yidx], fontsize=12)
    ax.set_title((names[xidx] + '-' + names[yidx]).upper() + ' view', fontsize=16)

    plt.savefig(outname + '_' + names[xidx] + names[yidx] + 'plot' + '.png')

# main execution starts here

colorarray = { 0:'black', 1:'red', 2:'navy', 3:'green', 4:'orange', 5:'maroon', 6:'aqua', 7:'silver' }

sim = simulation()

f = open(inputname, 'r')
nump = int(f.readline().split(': ')[1])

for i in range(0, nump):
    sim.new_particle(i)

for line in f:
    arr = line.strip('\n').split('\t')
    x = float(arr[2])
    y = float(arr[3])
    z = float(arr[4])
    sim.update_particle(int(arr[1]), x, y, z)

total = sim.total_steps()

plot2D(sim, 0, 1)
plot2D(sim, 0, 2)
plot2D(sim, 1, 2)

# Produce a 3D plot

fig = plt.figure(figsize=(10, 10), dpi=300)
ax = fig.add_subplot(111, projection='3d')
for i in range(0, nump):
    xlist = []
    ylist = []
    zlist = []
    x2list = []
    y2list = []
    z2list = []
    for j in range(0, total):
        xlist.append(sim.particles[i].coordlist[j][0])
        ylist.append(sim.particles[i].coordlist[j][1])
        zlist.append(sim.particles[i].coordlist[j][2])
        if not j % (total // 10):
            x2list.append(sim.particles[i].coordlist[j][0])
            y2list.append(sim.particles[i].coordlist[j][1])
            z2list.append(sim.particles[i].coordlist[j][2])
    ax.plot(xlist, ylist, zlist, color=colorarray[i % 8])
    ax.scatter(x2list, y2list, z2list, marker='s', color=colorarray[i % 8], s=5)
    ax.scatter(xlist[total - 1], ylist[total - 1], zlist[total - 1], color=colorarray[i % 8], s=25)
ax.set_xlabel('x', fontsize=12)
ax.set_ylabel('y', fontsize=12)
ax.set_zlabel('z', fontsize=12)
ax.set_title('3D view', fontsize=16)

plt.savefig(outname + '_xyzplot.png')
