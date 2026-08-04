typedef signed int s32;

/*
 * Configure this overlay's scene parameters, enable the configuration, and
 * apply the final magnitude.  The complete owner is the saved-link function
 * at 0x020027f4-0x02002820; its stack frame holds arguments five and six for
 * the first call.
 */

extern void Func_080091c0(s32, s32, s32, s32, s32, s32);
extern void Func_080000c0(s32);
extern void Func_080770c8(s32);

void Func_020027f4(void)
{
    Func_080091c0(82, 7, 1, 2, 18, 7);
    Func_080000c0(1);
    Func_080770c8(768);
}
