typedef signed int s32; typedef unsigned char u8;

extern void Func_02007c7a(s32, s32);
extern u8 Data_0200a2c5[];

s32 Func_020022ec(void)
{
    s32 n = 0xc80;

    Func_02007c7a((s32)Data_0200a2c5, n);
    return 0;
}
