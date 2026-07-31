typedef signed int s32;

extern void Func_02000d0c(void);
/* 0x02000d6a serves two imports: the one-argument message at +0x04 and the
 * three-argument setter at +0x0e. `_b` is the same address with its own
 * prototype. */
extern void Func_02000d6a(s32);
extern void Func_02000d6a_b(s32, s32, s32);
extern void Func_02000d10(s32);
extern void Func_02000d74(s32, s32, s32);
extern void Func_02000d9c(s32, s32);
extern void Func_02000d32(s32);
extern void Func_02000dbe(s32, s32, s32);
extern void Func_02000d44(s32);
extern void Func_02000d94(s32, s32);
extern void Func_02000d52(s32);
extern void Func_02000dca(s32, s32);
extern void Func_02000d60(s32);
extern void Func_02000db0(s32, s32);
extern void Func_02000d6e(s32);
extern void Func_02000de6(s32, s32);
extern void Func_02000d7c(s32);
extern void Func_02000ddc(s32, s32);
extern void Func_02000d8a(s32);
extern void Func_02000e02(s32, s32);
extern void Func_02000d98(s32);
extern void Func_02000de8(s32, s32);
extern void Func_02000da6(s32);
extern void Func_02000dba(void);

void Func_02000328(void)
{
    Func_02000d0c();
    Func_02000d6a(0x23A4);
    Func_02000d10(30);
    /* Same import, same first two arguments, differing only in the third.
     * Two call sites, not a loop. */
    Func_02000d6a_b(31, 4, 13);
    Func_02000d74(31, 4, 30);
    Func_02000d9c(31, 0);
    Func_02000d32(10);
    /* r1 = 129 << 1 = 0x102. Argument registers are set r1, r2, r0. */
    Func_02000dbe(32, 0x102, 50);
    Func_02000d44(10);
    Func_02000d94(32, 3);
    Func_02000d52(30);
    Func_02000dca(32, 0);
    Func_02000d60(10);
    Func_02000db0(33, 4);
    Func_02000d6e(20);
    Func_02000de6(33, 0);
    Func_02000d7c(10);
    Func_02000ddc(31, 2);
    Func_02000d8a(20);
    Func_02000e02(31, 0);
    Func_02000d98(10);
    /* Repeats the (32, 3) call made above; a second site, deliberately not
     * folded with the first. */
    Func_02000de8(32, 3);
    Func_02000da6(30);
    Func_02000dba();
}
