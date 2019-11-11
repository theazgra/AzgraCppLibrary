from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import matplotlib.animation
import numpy as np
import os
from math import cos
from math import sqrt
from matplotlib import cm


def griewank(xVec):
    sum = 0.0
    product = 0.0
    for i in range(len(xVec)):
        sum += ((xVec[i]**2) / 4000.0)
        product *= cos(xVec[i]) / sqrt(i+1)
    result = sum - product + 1
    return result


def load_3d_point_iterations(path):
    result = []
    lines = []
    with open(path, 'r') as f:
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
        result.append((x, y, z))
        offset += currentPointCount
    return result


def visualize(path):
    points = load_3d_point_iterations(path)

    min = -600
    max = 600

    fig = plt.figure()
    ax = Axes3D(fig)
    for pts in points:
        ax.set_xlim3d(min, max)
        ax.set_ylim3d(min, max)
        ax.set_zlim3d(min, max)
        ax.scatter(pts[0], pts[1], pts[2])
        plt.pause(0.25)
        plt.cla()

def get_surface_points(min, max, step, fn):
    X = []
    Y = []
    Z = []
    xRange = np.arange(min, max, step)
    yRange = np.arange(min, max, step)
    for x in xRange:
        X.append([x] * len(yRange))
        Y.append([y for y in yRange])
        Z.append([fn([x,y]) for y in yRange])

    return (X, Y, Z)


def main():
    min = -600.0
    max = 600.0
    step = 2.5

    x, y, z = get_surface_points(min, max, step, griewank)

    fig = plt.figure()
    axis3d = fig.gca(projection='3d')
    # plt.hold(True)
    surf = axis3d.plot_surface(np.array(x), np.array(y), np.array(z), cmap=cm.coolwarm)

    points = load_3d_point_iterations("D:\\codes\\git\\BIA\\cmake-build-debug\\soma_points.pts")

    for pts in points:
        axis3d.set_xlim3d(min, max)
        axis3d.set_ylim3d(min, max)
        axis3d.set_zlim3d(min, max)
        # surf = axis3d.plot_surface(np.array(x), np.array(y), np.array(z), cmap=cm.coolwarm)
        axis3d.scatter(pts[0], pts[1], pts[2],c="red")
        plt.pause(0.9)
        plt.cla()

    # axis3d.scatter(points[0][0], points[0][1], points[0][2])
    # plt.show()


    # fig = plt.figure()
    # ax = Axes3D(fig)


    # plt.show()

main()
