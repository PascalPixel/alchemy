typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;

extern void Func_02006ab6(s32);
extern void Func_02006c6c(s32);
extern void Func_02006aee(s32, s32, s32);
extern void Func_02006bb6(s32, s32);
extern void Func_02006b3a(s32, s32, s32);
extern s32 Func_02006b00(s32);
extern void Func_02006a5e(s32, s32);
extern void Func_02006ae4(s32);
extern void Func_02006b6e(s32, s32, s32);
extern void Func_02006afc(s32);

void Func_02000ffc(s32 a)
{
    Func_02006ab6(a);
    Func_02006c6c(0xe4);
    Func_02006aee(0, 0x6666, 0x3333);
    Func_02006bb6(0, 2);
    Func_02006b3a(0, 0, -8);
    Func_02006a5e(Func_02006b00(0), 0);
    Func_02006ae4(8);
    Func_02006b6e(0, (a << 19) + 0x80000, 0);
    Func_02006afc(30);
}
