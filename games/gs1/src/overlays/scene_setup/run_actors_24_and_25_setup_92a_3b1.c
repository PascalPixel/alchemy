#include "types.h"

/*
 * Actors 24 and 25 setup for overlay resource_3b1. Each callee slot uses
 * its own local veneer, so the names are per call site and not the shared
 * main-image symbol.
 */

extern u8 Value_0000092a;

s32 Func_0200a730();
s32 Func_0200a73a();
void Func_0200bc60(void);
void Func_0200a08a();
void Func_0200a094();
void Func_02008f8a();
void Func_0200a0a4();
void Func_0200bd0e();
void Func_0200bd18();
void Func_0200aac8(void);
void Func_0200bc82();
void Func_0200bcae(void);

/*
 * A flat setter sequence, no branches. The 108-byte owner at 0x02005780
 * includes its one pool word, the address taken as Value_0000092a.
 */
void FieldScene_RunActors24And25SetupWithValue92a(void)
{
    s32 handle = Func_0200a730(0, 0);
    s32 other = Func_0200a73a(1, 0);

    Func_0200bc60();
    Func_0200a08a(24, 1, 0);
    Func_0200a094(25, 0, 0);
    Func_02008f8a(0);
    Func_0200a0a4(19, handle, other);
    Func_0200bd0e(11, 0, 0);
    Func_0200bd18(12, 0, 0);
    Func_0200aac8();
    Func_0200bc82((s32)&Value_0000092a);
    Func_0200bcae();
}
