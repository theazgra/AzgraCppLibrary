from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import matplotlib.animation
import numpy as np
import os

def load_3d_point_iterations(path):
    result = []
    lines = []
    with open(path,'r') as f:
        lines = [line.strip() for line in f]
        lineCount = len(lines)
    itCount = lines[0][lines[0].index('=')+1:]
    offset = 1
    while offset < lineCount:
        currentPointCount = int(lines[offset][lines[offset].index('=')+1:])
        offset += 1
        x = []
        y = []
        z = []
        for pI in range(currentPointCount):
            lineValues = lines[offset + pI].split(';')
            assert (len(lineValues) == 3)
            x.append(float(lineValues[0]))
            y.append(float(lineValues[1]))
            z.append(float(lineValues[2]))
        result.append((x,y,z))
        offset += currentPointCount
    return result


os.chdir("D:/codes/git/AzgraCppLibrary/cmake-build-debug/")
points = load_3d_point_iterations("points.pts")

fig = plt.figure()
ax = Axes3D(fig)
for pts in points:
    ax.scatter(pts[0],pts[1],pts[2])
    plt.pause(0.5)
    plt.cla()