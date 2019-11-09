#pragma once

#include <azgra/azgra.h>

namespace azgra::geometry
{
    using namespace azgra;

    template<typename CoordinateType = int>
    struct Point2D
    {
        CoordinateType x;
        CoordinateType y;

        Point2D() = default;

        Point2D(CoordinateType _x, CoordinateType _y) : x(_x), y(_y)
        {}

        constexpr f64 euclidean_distance(const Point2D<CoordinateType> &other) const noexcept
        {
            f64 euclidDist = sqrt(pow((static_cast<f64>(x) - static_cast<f64>(other.x)), 2) + pow((static_cast<f64>(y) - static_cast<f64>(other.y)), 2));
            return euclidDist;
        }
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