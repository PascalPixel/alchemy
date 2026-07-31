typedef unsigned char u8;
typedef signed int s32;

extern void Func_020045c0(void);
extern void Func_020026cc(s32, s32);
extern void Func_020025c4(s32, s32, s32, s32);
/* Two sites of the frame wait, so two names. */
extern void Func_020045d6(s32);
extern void Func_020045f6(s32);
extern void Func_02002658(s32);
extern void Func_0200466c(s32, s32);
extern u8 *Func_0200460a(s32);
extern void Func_020045b0(u8 *, s32);
extern void Func_020045f4(s32);
extern void Func_02004662(s32, s32, s32);
extern void Func_0200461e(void);

void Func_02001818(void)
{
    /* No argument register is written before this branch: the caller's r0-r3
     * reach the import unchanged. */
    Func_020045c0();

    Func_020026cc(14, 1);

    /* 424 = 212 << 1 and 480 = 240 << 1, both built with movs/lsls.  0x79999
     * comes from the pool word at 0x02001878. */
    Func_020025c4(14, 424, 480, 0x79999);

    Func_020045d6(2);
    Func_02002658(14);
    Func_0200466c(14, 15);

    /* Only r1 is set for the second branch; r0 still carries the value
     * Func_0808a080 left there.  Whether Func_080091e0 reads it is unverified,
     * but the dataflow is preserved as written. */
    Func_020045b0(Func_0200460a(14), 0);

    Func_020045f6(30);

    /* 0x305 from the pool word at 0x0200187c. */
    Func_020045f4(0x305);

    /* The same 424 and 480 as above, now promoted to 16.16 by shifting the
     * literals 17 instead of 1. */
    Func_02004662(17, 424 << 16, 480 << 16);

    /* No argument registers are set. */
    Func_0200461e();
}
