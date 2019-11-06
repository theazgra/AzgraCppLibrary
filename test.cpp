#include <azgra/stream/in_binary_file_stream.h>
//#include <azgra/stream/binary_converter.h>

using namespace azgra;
/*
 *  public static int[] convertU16BytesToInt(final byte[] bytes) {
        assert (bytes.length % 2 == 0);
        int[] values = new int[bytes.length / 2];

        int index = 0;
        for (int i = 0; i < bytes.length; i += 2) {
            final int value = (int) (((bytes[i] & 0xff) << 8) | (bytes[i + 1] & 0xff));
            if (value > 0) {
                values[index++] = value;
                continue;
            }
            values[index++] = value;
        }
        return values;
    }
 * */

int main(int argc, char **argv)
{

    InBinaryFileStream stream(argv[1]);
    auto bytes = stream.consume_whole_file();
    auto u32Values = bytes_to_ushort_array(bytes);

    std::ofstream out(argv[2]);
    always_assert(out.is_open());
    for (u32 val : u32Values)
        out << val << "\n";


    return 0;
}
