typedef unsigned char u8;
typedef signed int s32;

extern s32 Func_020049c6(s32);
extern void Func_02004ba8(s32);
extern void Func_0200498a(u8 *, s32, s32);
extern void Func_02004a80(s32, s32, s32);
extern void Func_02004a06(s32);
extern void Func_02000474(s32);
extern u8 Value_00000205;
extern u8 Data_0200d78a[];

void Func_020002e4(void)
{
    if (Func_020049c6((s32)&Value_00000205) == 0) {
        Func_02004ba8(0x9E);
        Func_0200498a(Data_0200d78a, 50, 44);
    }
    Func_02004a80(0, 0x154, 0x378);
    Func_02004a06(3);
    Func_02000474(7);
}
