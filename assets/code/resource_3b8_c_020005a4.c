typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern u8 Value_00002352;
extern u8 Value_00000f31;

extern void Func_02004988(void);
extern void Func_02004ab4();
extern void Func_02004a4c();
extern void Func_02004a66(s32, s32);
extern void Func_0200499c(s32);
extern void Func_020049aa(s32);
extern void Func_02004a64(s32, s32, s32);
extern s32 Func_02004a84(s32, s32);
extern s32 Func_020049e4(s32, s32);
extern void Func_02004a8e(s32);
extern void Func_02004aa6(s32, s32);
extern void Func_020049de(s32);
extern void Func_02004aa4(s32);
extern void Func_02004abc(s32, s32);
extern void Func_020049f2(s32);
extern void Func_02004a8a(s32, s32);
extern void Func_02004a00(s32);
extern void Func_02004ae4(s32, s32, s32);
extern void Func_02004a12(s32);
extern void Func_02004a9c(s32, s32, s32);
extern void Func_02004b4c(s32, s32);
extern void Func_02004a0a(s32);

void Func_020005a4(void)
{
    s32 k;

    Func_02004988();
    Func_02004ab4();
    k = (s32)&Value_00002352;
    Func_02004a4c(k);
    Func_02004a66(-1, 0);
    Func_0200499c(10);
    Func_02004a4c(14, 2);
    Func_020049aa(30);
    Func_02004a64(0, 14, 30);
    Func_02004a84(14, 0);
    if (Func_020049e4(0, 0) != 0) {
        Func_02004a8e(k + 2);
        Func_02004aa6(14, 0);
    } else {
        Func_020049de(20);
        Func_02004aa4(k + 3);
        Func_02004abc(14, 0);
        Func_020049f2(10);
        Func_02004a8a(0, 3);
        Func_02004a00(30);
        Func_02004ae4(0, 0x4000, 0);
        Func_02004a12(30);
        Func_02004a9c(16, 0, 0);
        Func_02004b4c(205, 3);
        Func_02004ab4(0, 1);
        Func_02004a4c(205, 0);
        Func_02004a0a((s32)&Value_00000f31);
    }
}
