#include "types.h"

/*
 * A once-only cutscene guarded by scene flag 0x9c8: the flag is tested, set
 * immediately, and the whole body is skipped when it was already set.
 *
 * Declarations are old-style because two of the veneer entries are reached
 * with differing argument counts from different call sites.
 */

/* Scene flag test: zero means not yet set. */
s32 Func_02002fde();
/* Scene flag set. */
void Func_02002ff0();
void Func_0200300c();
void Func_020030a0();
void Func_020030b8();
void Func_020030b4();
void Func_0200309e();
void Func_0200303e();
void Func_02003074();
void Func_02003132();
/* Returns the record for a numbered slot. */
s32 *Func_02003078();
void Func_020030ac();
void Func_020030f0();
void Func_0200308c();

void Func_02000ad0(void)
{
    if (Func_02002fde(0x9c8) != 0) return;
    Func_02002ff0(0x9c8);

    Func_0200300c();
    Func_020030a0(0x20000, 0x4000);
    Func_020030b8(15, 1);
    Func_020030b4();
    Func_0200309e(15, 0, 20);
    Func_020030b8(15, 258);
    Func_020030a0(15, 2);
    Func_0200303e(20);
    Func_02003074(15, 0x10000, 0x8000);
    Func_02003132(152);

    Func_02003078(15)[10] = 0x80000;   /* +40 */

    Func_020030ac(15, 584, 680);
    Func_020030f0(15, 0x4000, 20);
    Func_0200308c();
}
