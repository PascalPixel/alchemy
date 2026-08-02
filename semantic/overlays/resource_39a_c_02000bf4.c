typedef signed int s32;

/*
 * Run this scene transition's six setup services.  The literal word following
 * the return belongs to this owner and supplies 0x121 to Func_080f9010.
 */

extern void Func_080f9010(s32);
extern void Func_02000180(s32, s32, s32);
extern void Func_02000bc8(void);
extern void Func_080000c0(s32);
extern void Func_020012cc(void);

void Func_02000bf4(void)
{
    Func_080f9010(241);
    Func_02000180(11, 0, 128);
    Func_02000bc8();
    Func_080f9010(0x121);
    Func_080000c0(2);
    Func_020012cc();
}
