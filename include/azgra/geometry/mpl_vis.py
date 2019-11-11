from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
from math import cos
from math import sqrt
from math import exp
from math import pi
from math import sin
from matplotlib import cm
from sys import argv


def griewank(xVec):
    sum = 0.0
    product = 0.0
    for i in range(len(xVec)):
        sum += ((xVec[i]**2) / 4000.0)
        product *= cos(xVec[i]) / sqrt(i+1)
    result = sum - product + 1
    return result


def ackley(xVec):
    a = 20
    b = 0.2
    c = 2*pi
    sum1 = 0.0
    sum2 = 0.0
    for i in range(len(xVec)):
        sum1 += xVec[i] ** 2
        sum2 += cos(c * xVec[i])
    result = (-1.0*a)*exp((-1.0*b)*sqrt((1.0/len(xVec))*sum1)) - \
        exp((1.0/len(xVec))*sum2) + a + exp(1.0)
    return result

def schwefel(xVec):
    sum = 0.0
    for i in range(len(xVec)):
        sum += xVec[i]*sin(sqrt(abs(xVec[i])))
    result = (418.9829 * len(xVec)) - sum
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


def get_surface_points(min, max, step, fn):
    X = []
    Y = []
    Z = []
    xRange = np.arange(min, max, step)
    yRange = np.arange(min, max, step)
    for x in xRange:
        X.append([x] * len(yRange))
        Y.append([y for y in yRange])
        Z.append([fn([x, y]) for y in yRange])

    return (X, Y, Z)


def viz(surfaceFn, min, max, zmin, zmax, step, pointFile):
    plt.ion()
    x, y, z = get_surface_points(min, max, step, surfaceFn)
    fig = plt.figure()
    axis3d = fig.gca(projection='3d')
    axis3d.set_xlim3d(min, max)
    axis3d.set_ylim3d(min, max)
    axis3d.set_zlim3d(zmin, zmax)
    surf = axis3d.plot_surface(
        np.array(x), np.array(y), np.array(z), cmap=cm.coolwarm)

    plt.draw()
    plt.pause(2)
    plt.cla()

    # "D:\\codes\\git\\BIA\\cmake-build-debug\\soma_points.pts")
    points = load_3d_point_iterations(pointFile)
    scatter = axis3d.scatter(points[0][0], points[0][1], points[0][2], c="red")

    for i in range(1, len(points)):
        pts = points[i]

        scatter._offsets3d = (pts[0], pts[1], pts[2])
        plt.pause(1.5)
        plt.draw()

    plt.pause(10)


def main():
    if (len(argv) < 3):
        print("E: pts file and function argument required")
        return

    fn = argv[2]
    if fn == "griewank":
        viz(griewank, -600, 600, 0, 200, 2.5, argv[1])
    elif fn == "ackley":
        viz(ackley, -33, 33, 0, 30, 0.5, argv[1])
    elif fn == "schwefel":
        viz(schwefel, -500, 500, 0, 1900, 2.5, argv[1])
    else:
        raise "Unknown surface function."

    # plt.ion()
    # min = -600.0
    # max = 600.0
    # step = 2.5
    # x, y, z = get_surface_points(min, max, step, griewank)
    # fig = plt.figure()
    # axis3d = fig.gca(projection='3d')
    # axis3d.set_xlim3d(min, max)
    # axis3d.set_ylim3d(min, max)
    # axis3d.set_zlim3d(0, 200)
    # surf = axis3d.plot_surface(
    #     np.array(x), np.array(y), np.array(z), cmap=cm.gray)

    # # "D:\\codes\\git\\BIA\\cmake-build-debug\\soma_points.pts")
    # points = load_3d_point_iterations(argv[1])
    # scatter = axis3d.scatter(points[0][0], points[0][1], points[0][2], c="red")

    # for i in range(1, len(points)):
    #     pts = points[i]

    #     scatter._offsets3d = (pts[0], pts[1], pts[2])
    #     plt.pause(1.5)
    #     plt.draw()

    # plt.pause(10)


main()
