typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

void Func_0800e280(u8 *arg0, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        *(u16 *)(arg0 + 6) = value;
    } else if (operation == 1) {
        *(u16 *)(arg0 + 6) += value;
    } else {
        result = 0;
        if (*(u16 *)(arg0 + 6) == (u16)value)
            result = 1;
        arg0[87] = result;
    }
}
