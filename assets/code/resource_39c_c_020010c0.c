typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;

extern s32 Func_02006b4c(s32);
extern s32 Func_02006b56(s32);
extern s32 Func_02006b60(s32);
extern void Func_02006b72(s32);
extern void Func_02006b98(s32);
extern void Func_02006b38(s32, s32, s32);
extern void Func_02006d66(s32);
extern void Func_02006bb4(s32);
extern void Func_02006d16(void);
extern void Func_02006d22(void);
extern void Func_02006d88(s32);
extern void Func_02006b6e(s32, s32, s32);
extern void Func_02006b7a(void);
extern void Func_02006d08(s32);
extern void Func_02006bd8(s32);
extern u8 *Data_03001ebc;
extern u8 Value_00000311;
extern u8 Value_00000312;
extern u8 Value_00000876;
extern u8 Value_00000121;

void Func_020010c0(void)
{
    if (Func_02006b4c(0x310) != 0
        && Func_02006b56((s32)&Value_00000311) != 0
        && Func_02006b60((s32)&Value_00000312) != 0) {
        Func_02006b72((s32)&Value_00000876);
        Func_02006b98(30);
        Func_02006b38(0x10000, 0x10000, 0x10000);
        Func_02006d66(0x8d);
        Func_02006bb4(0x3c);
        *(s32 *)(Data_03001ebc + 0x1c0) = 0x100;
        Func_02006d16();
        Func_02006d22();
        Func_02006d88((s32)&Value_00000121);
        Func_02006b6e(-1, -1, 0xe666);
        Func_02006b7a();
        Func_02006d08(13);
    } else {
        Func_02006bd8((s32)&Value_00000876);
    }
}
