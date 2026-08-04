#include "types.h"

/*
 * Resource 378 scene reset at 0x020006e8 (100 bytes including its literal).
 * The prologue and the pop-{r0}/bx-r0 epilogue are unambiguous.  The literal
 * 0x116c is loaded as a value (not an in-image pointer), so it stays an
 * integer argument here.  All calls are retained in the ROM order.
 */

extern void Func_02003cdc();
extern void Func_02003dd8();
extern void Func_02003df8();
extern void Func_02003df4();
extern void Func_02003cf2();
extern void Func_02003da4();
extern void Func_02003d02();
extern void Func_02003d8a();
extern void Func_02003d10();
extern void Func_02003dd6();
extern void Func_02003dee();
extern void Func_02003d16();
extern void Func_02003d3a();

void Func_020006e8(void)
{
    Func_02003cdc();
    Func_02003dd8(0x10000, 0x2000);
    Func_02003df8(1, 1);
    Func_02003df4();
    Func_02003cf2(20);
    Func_02003da4(8, 0, 0);
    Func_02003d02(10);
    Func_02003d8a(8, 4);
    Func_02003d10(20);
    Func_02003dd6(0x116c);
    Func_02003dee(8, 0);
    Func_02003d16(0x200);
    Func_02003d3a();
}
