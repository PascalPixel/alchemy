#include "types.h"

/*
 * Configure this overlay's scene parameters, enable the configuration, and
 * apply the final magnitude.  The complete owner is the saved-link function
 * at 0x020027f4-0x02002820; its stack frame holds arguments five and six for
 * the first call.
 */





extern void Func_020055e2(s32, s32, s32, s32, s32, s32);
extern void Func_02005590(s32);
extern void Func_02005638(s32);
void Func_020027f4(void)
{
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 18;
    s32 sixth = 7;

    Func_020055e2(82, 7, 1, 2, fifth, sixth);
    Func_02005590(1);
    Func_02005638(768);
}
