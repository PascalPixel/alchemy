typedef signed int s32;

extern void Func_02000308(void);
extern void Func_02000342(s32, s32, s32);
extern void Func_02000310(s32);
extern void Func_02000352(s32, s32, s32);
extern void Func_02000320(s32);
/* 0x0200036e serves two imports: the one-argument dialogue line and the
 * two-argument presentation select. */
extern void Func_0200036e(s32);
extern void Func_0200036e_b(s32, s32);
/* 0x0200037e likewise: the two-argument act and the zero-argument bracket
 * close. */
extern void Func_0200037e(s32, s32);
extern void Func_0200037e_b(void);
extern void Func_0200035e(s32, s32);
extern void Func_02000344(s32);
extern void Func_0200038e(s32, s32, s32);
extern void Func_02000354(s32);
extern void Func_0200038c(s32, s32);
extern void Func_02000362(s32);
extern void Func_020003ba(s32, s32);

void Func_02000094(void)
{
    Func_02000308();

    Func_02000342(8, 9, 0);
    Func_02000310(40);

    Func_02000352(8, 10, 0);
    Func_02000320(40);

    Func_0200036e(0x138A);
    Func_0200037e(8, 0);

    Func_0200035e(9, 2);
    Func_0200036e_b(10, 2);
    Func_02000344(20);

    Func_0200038e(8, 0, 0);
    Func_02000354(20);

    Func_0200038c(8, 1);
    Func_02000362(20);

    Func_020003ba(8, 0);

    Func_0200037e_b();
}
