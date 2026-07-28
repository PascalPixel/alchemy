typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern u8 Data_080fb92c[];
extern s16 Data_080fb9b0[];
extern u8 Data_080fb9c8[];

s32 Func_080facf8(s32 kind_arg, s32 position_arg, s32 fraction_arg)
{
    u8 kind = kind_arg;
    u8 position = position_arg;
    u8 fraction = fraction_arg;

    if (kind == 4) {
        if (position <= 20) {
            position = 0;
        } else {
            position -= 21;
            if (position > 59)
                position = 59;
        }
        return Data_080fb9c8[position];
    }

    if (position <= 35) {
        fraction = 0;
        position = 0;
    } else {
        position -= 36;
        if (position > 130) {
            position = 130;
            fraction = 255;
        }
    }

    {
        s32 first = Data_080fb92c[position];
        s32 second;
        first = Data_080fb9b0[first & 15] >> (first >> 4);
        second = Data_080fb92c[position + 1];
        second = Data_080fb9b0[second & 15] >> (second >> 4);
        return first + ((fraction * (second - first)) >> 8) + 0x800;
    }
}
