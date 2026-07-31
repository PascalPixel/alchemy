typedef signed int s32;

extern void Func_02004a68(void);
extern void Func_02004ac0(s32, s32);
extern s32 Func_02004a56(s32);
extern s32 Func_02004a60(s32);
extern void Func_02004a64(s32, s32);
extern void Func_02004a6e(s32, s32);
extern void Func_02004aa2(void);

void Func_02001970(void)
{
    Func_02004a68();
    Func_02004ac0(0, 1);
    if (Func_02004a56(0x982) != 0 || Func_02004a60(0x983) != 0) {
        Func_02004a64(0x268A, 1);
    } else {
        Func_02004a6e(0x2689, 1);
    }
    Func_02004aa2();
}
