#include "types.h"

/*
 * resource_398 owner at 0x020004b4, 52 bytes.
 *
 * One guard around a two-call body. The guard's argument 0x305 is too wide for
 * a `movs` immediate, so it comes out of the literal pool at 0x020004e4.
 *
 * The six-argument call passes its last two on the stack, which is what the
 * `sub sp, #8` and the pair of stores before the register arguments are.
 */

s32 Func_02000e04();
void Func_02000e64();

void Func_020004b4(void)
{
    if (Func_02000e04(0x305) != 0) {
        Func_02000e04(31, 0, 1, 1, 8, 13);
        Func_02000e64(8, 0);
    }
}
