typedef unsigned char u8;
typedef signed int s32;

void Func_0800e7d4(u8 *arg0, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        arg0[93] = value;
    } else if (operation == 1) {
        arg0[93] += value;
    } else {
        result = 0;
        if (arg0[93] == (u8)value)
            result = 1;
        arg0[87] = result;
    }
}
