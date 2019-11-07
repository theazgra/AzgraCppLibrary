#include <azgra/stream/in_binary_file_stream.h>
#include <azgra/geometry/plot.h>

using namespace azgra;
using namespace azgra::geometry;

int main(int argc, char **argv)
{
    std::vector<Point3D<f64>> points = {Point3D<f64>(0, 0, 0),
                                        Point3D<f64>(1, 1, 1),
                                        Point3D<f64>(2, 2, 2),
                                        Point3D<f64>(3, 3, 3),
                                        Point3D<f64>(4, 4, 4),
                                        Point3D<f64>(5, 5, 5)};

    std::vector<Point3D<f64>> points2 = {Point3D<f64>(0, 0, 9),
                                         Point3D<f64>(1, 1, 8),
                                         Point3D<f64>(2, 2, 7),
                                         Point3D<f64>(3, 3, 6),
                                         Point3D<f64>(4, 4, 5),
                                         Point3D<f64>(5, 5, 7)};
    dump_3d_points_history({points,points2}, "points.pts");
//    InBinaryFileStream stream(argv[1]);
//    auto bytes = stream.consume_whole_file();
//    auto u32Values = bytes_to_ushort_array(bytes);
//    std::ofstream out(argv[2]);
//    always_assert(out.is_open());
//    for (u32 val : u32Values)
//        out << val << "\n";


    return 0;
}
