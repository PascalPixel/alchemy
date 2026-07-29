typedef signed int s32;

extern void Func_02006c20(void);
extern s32 Func_02006c06(s32);
extern void Func_02006cf8(s32);
extern void Func_02006d00(s32);
extern void Func_02006d18(s32, s32);
extern void Func_02006c4c(void);

void Func_02000c60(void)
{
    Func_02006c20();
    if (Func_02006c06(0x202) != 0) {
        Func_02006cf8(0xf68);
    } else {
        Func_02006d00(0xf69);
    }
    Func_02006d18(21, 0);
    Func_02006c4c();
}
