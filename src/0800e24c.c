typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

void Func_0800e24c(u8 *arg0, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        *(u16 *)(arg0 + 4) = value;
    } else if (operation == 1) {
        *(u16 *)(arg0 + 4) += value;
    } else {
        result = 0;
        if (*(s16 *)(arg0 + 4) == (s16)value)
            result = 1;
        arg0[87] = result;
    }
}
