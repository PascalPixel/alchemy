typedef unsigned char u8;
typedef signed int s32;

extern s32 Func_02009062(s32, s32, s32);
extern s32 Func_02009030(s32, s32, s32);
extern s32 Func_0200903e(s32, s32, s32);
extern s32 Func_0200904c(s32, s32, s32);

s32 Func_02004840(u8 *p)
{
    s32 x;
    s32 y;

    if (p == 0) {
        return 1;
    }
    x = (p[0] << 19) + 0x480000;
    y = (p[1] << 19) + 0x2780000;
    if (Func_02009062(x, y, 0) != 0 || Func_02009030(x, y, 2) != 0
        || Func_0200903e(x, y, 24) != 0 || Func_0200904c(x, y, 25) != 0) {
        return -1;
    }
    return 0;
}
