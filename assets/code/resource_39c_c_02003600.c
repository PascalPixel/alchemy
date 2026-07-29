typedef signed short s16; typedef signed int s32; typedef unsigned char u8;

extern void Func_020090b8(void);
extern void Func_02009138(s32, s32);
extern s32 Func_02009096(s32);
extern void Func_0200906a(s32, s32);
extern void Func_02009074(s32, s32);
extern s32 Func_020090aa(s32);
extern void Func_02009104(void);
extern u8 *Data_03001ebc;

void Func_02003600(void)
{
    Func_020090b8();
    Func_02009138(0, 1);
    if (Func_02009096(0x881) == 0)
        Func_0200906a(0x1636, 1);
    else
        Func_02009074(0x1635, 1);
    if (Func_020090aa(0xb9) != -1) {
        s16 *slot = (s16 *)Data_03001ebc + 185;
        s32 one = 1;

        *slot = one;
    }
    Func_02009104();
}
