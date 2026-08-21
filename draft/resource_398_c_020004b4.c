#include "types.h"

/*
 * resource_398 owner at 0x020004b4, 52 bytes.
 *
 * One guard around a two-call body. The guard's argument 0x305 is too wide
 * for a `movs` immediate, so it comes out of the literal pool at 0x020004e4.
 *
 * The six-argument call passes its last two on the stack, which is what the
 * `sub sp, #8` and the pair of stores before the register arguments are.
 * Explicit locals for each argument (a0..a5) match the reference's
 * register/stack staging exactly.
 *
 * Residual: two halfwords (movs r0,#31 / movs r1,#0) tie in scheduling order.
 * Traced via -fsched-verbose=5: a1's pseudo carries an extra anti-dependence
 * edge onto the SECOND call's zero argument, because both calls' second
 * argument (a1 here, 0 in Func_02000e64(8,0)) land in the same hardware
 * register r1 after reload -- an inherent consequence of both calls' argument
 * positions, not a source-spelling artifact. Every local respelling tried
 * (temp declaration order, byte-sized temps, named temps for the second
 * call, computed vs literal values) ties identically. Measured, not locally
 * reachable; matches the scheduler-tie class documented on other owners.
 */

s32 Func_02000e04();
void Func_02000e64();

void Func_020004b4(void)
{
    if (Func_02000e04(0x305) != 0) {
        s32 a0, a1, a2, a3, a4, a5;
        a0 = 31;
        a1 = 0;
        a2 = 1;
        a3 = 1;
        a4 = 8;
        a5 = 13;
        Func_02000e04(a0, a1, a2, a3, a4, a5);
        Func_02000e64(8, 0);
    }
}
