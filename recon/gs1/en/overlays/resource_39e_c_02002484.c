#include "types.h"

/*
 * resource_39e owner at 0x02002484, 132 bytes.
 *
 * Two guards decide between a short path and a long one. Both tests skip to the
 * same address, which is the short-circuit `&&` -- one condition, not two
 * nested ifs, because the else arm belongs to the pair and not to the first
 * test alone.
 *
 * The long arm sets up a display: two coordinate pairs built as `n << 1` or
 * `128 << k`, which is how this compiler materialises a constant too large for
 * a `movs` immediate.
 */

s32 Func_0200684a();
s32 Func_02006854();
void Func_02006864();
void Func_02006858();
void Func_0200688c();
void Func_02006a2c();
void Func_0200684e();
void Func_020068dc();
void Func_02006908();
void Func_0200690c();
void Func_020068ba();
void Func_020069f0();
void Func_020068d4();

void Func_02002484(void)
{
    Func_02006864();
    if (Func_0200684a(2202) == 0 && Func_02006854(2197) == 0) {
        Func_02006858(6317, 1);
        Func_0200688c();
    } else {
        Func_02006a2c(158);
        Func_0200684e(0x0200c77a, 78, 13);
        Func_020068dc(0, 32768, 16384);
        Func_02006908(0, 306, 248);
        Func_0200690c(0, 304, 216);
        Func_020068ba(20);
        Func_020069f0(4);
        Func_020068d4();
    }
}
