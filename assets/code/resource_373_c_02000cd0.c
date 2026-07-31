typedef signed int s32;

extern void Func_02006b76(s32);
extern void Func_02006c38(s32, s32, s32);
extern void Func_02006c4c(s32, s32, s32);
extern void Func_02006c90(void);
extern void Func_02006ce2(void);
extern void Func_02006d7e(s32, s32, s32);
extern void Func_02006d82(s32);
extern void Func_02006da4(s32, s32, s32);
extern void Func_02006dae(s32, s32);

void Func_02000cd0(void)
{
    Func_02006c90();
    /* 0xc0 << 10 twice and 0x80 << 9; the first two arguments are the same
     * value and the reference builds each in place, which is why this row
     * needs the constant-sharing mode off. */
    Func_02006c38(0x30000, 0x30000, 0x10000);
    Func_02006b76(10);
    /* Both -1 are negated in place for the same reason. */
    Func_02006c4c(-1, -1, 0x0000E666);
    Func_02006d82(0x00001C9A);
    Func_02006da4(17, 0, 20);
    Func_02006d7e(17, 0, 20);
    Func_02006dae(17, 0);
    Func_02006ce2();
}
