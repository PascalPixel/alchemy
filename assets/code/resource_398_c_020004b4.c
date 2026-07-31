typedef signed int s32;

/* Two sites reach this one address with different arities, so the prototype
 * has to be old-style or the six-argument call materialises the wrong shape. */
extern s32 Func_02000e04();
extern void Func_02000e64(s32, s32);

void Func_020004b4(void)
{
    if (Func_02000e04(0x305) != 0) {
        s32 fifth = 8;
        s32 sixth = 13;

        Func_02000e04(31, 0, 1, 1, fifth, sixth);
        Func_02000e64(8, 0);
    }
}
