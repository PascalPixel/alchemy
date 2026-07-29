typedef unsigned char u8;
typedef signed int s32;

extern void Func_0200958c(s32, s32, s32, s32);

void Func_020048a4(s32 a, u8 *p)
{
    Func_0200958c(a, (p[0] << 19) + 0x480000, 0, (p[1] << 19) + 0x2780000);
}
