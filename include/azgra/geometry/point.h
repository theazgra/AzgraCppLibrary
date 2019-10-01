#pragma once

namespace azgra::geometry
{
    template<typename CoordinateType = int>
    struct Point2D
    {
        CoordinateType x;
        CoordinateType y;

        Point2D() = default;

        Point2D(CoordinateType _x, CoordinateType _y) : x(_x), y(_y)
        {}
    };

    template<typename CoordinateType = int>
    struct Point3D
    {
        CoordinateType x;
        CoordinateType y;
        CoordinateType z;

        Point3D() = default;

        Point3D(CoordinateType _x, CoordinateType _y, CoordinateType _z) : x(_x), y(_y), z(_z)
        {}
    };
}