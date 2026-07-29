typedef unsigned char u8;
typedef signed int s32;

extern s32 Func_02004946(s32);
extern void Func_02004ad0(s32);
extern void Func_02004b70(s32);
extern void Func_02004ade(s32);
extern u8 Value_0000085a;

void Func_020006b4(void)
{
    if (Func_02004946((s32)&Value_0000085a) == 0) {
        Func_02004ad0(101);
    } else {
        Func_02004b70(123);
        Func_02004ade(3);
    }
}
