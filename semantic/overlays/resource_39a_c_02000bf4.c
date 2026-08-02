typedef signed int s32;

/*
 * Run this scene transition's six setup services.  The literal word following
 * the return belongs to this owner and supplies 0x121 to Func_0200301e.
 */

extern void Func_0200300a(s32);
extern void Func_02000d84(s32, s32, s32);
extern void Func_020017d0(void);
extern void Func_0200301e(s32);
extern void Func_02002e74(s32);
extern void Func_02001ee4(void);

void Func_02000bf4(void)
{
    Func_0200300a(241);
    Func_02000d84(11, 0, 128);
    Func_020017d0();
    Func_0200301e(0x121);
    Func_02002e74(2);
    Func_02001ee4();
}
