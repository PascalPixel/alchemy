typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))

extern u8 *Func_02006a60(s32);
extern void Func_02006a46(void);
extern void Func_02006bfc(s32);
extern void Func_02006ad4(s32, s32);
extern void Func_02006ac8(s32, s32, s32);
extern void Func_02006ad6(s32);
extern void Func_02006b2e(s32, s32);
extern s32 Func_02006a9c(s32);
extern void Func_020069fa(s32, s32);
extern void Func_02006a84(s32);
extern void Func_02006bd8(void);
extern void Func_02006be4(void);
extern void Func_02006bb2(s32);
extern void Func_02006aa6(void);
extern u8 Value_02008cd1;

void Func_02000f84(s32 a)
{
    u8 *v = Func_02006a60(0);
    Func_02006a46();
    Func_02006bfc(0xe4);
    F(v, s32, 0x6c) = (s32)&Value_02008cd1;
    F(v, s32, 0x30) = 0x3333;
    Func_02006ad4(0, 2);
    Func_02006ac8(0, 0, -6);
    Func_02006ad6(0);
    Func_02006b2e(0, 15);
    Func_020069fa(Func_02006a9c(0), 0);
    F(v, s32, 0x6c) = 0;
    Func_02006a84(30);
    Func_02006bd8();
    Func_02006be4();
    Func_02006bb2(a);
    Func_02006aa6();
}
