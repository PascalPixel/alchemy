typedef unsigned char u8;
typedef signed int s32;

extern u8 Data_02000380[];

s32 Func_08078aa0(s32 index, s32 delta)
{
    s32 position = index;
    u8 *data = Data_02000380;

    index = 0;
    if (position <= 127) {
        s32 value = data[position];

        value += delta;

        if (value < 0) {
            value = 0;
        } else if (value > 99) {
            value = 99;
            index = 99;
        } else {
            index = value;
        }
        data[position] = value;
    }
    return index;
}
