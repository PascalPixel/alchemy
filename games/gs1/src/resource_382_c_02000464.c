/*
 * BYTE-EXACT and adopted 2026-08-07.  The last residual was the epilogue, which
 * the reference returns through r1 (`pop {r1} ; bx r1') rather than r0: the
 * owner passes the result of its final call back to its caller, so r0 has to
 * survive the pop.  The tail call is written as `return Func_02001e10(2);' and
 * the owner's type changed from void to s32 to say so; the fall-through path
 * returns nothing, exactly as the reference does.
 */

#include "types.h"
 
  

extern void Func_02001f28(void);
extern u8 * Func_02001dde(int);
extern void Func_02001e26(int,int,int,int);
extern void Func_02001f54(int);
extern void Func_02001e0a(int);
extern s32 Func_02001e10(int);
s32 Func_02000464(void)
{
    Func_02001f28();
    if ((*(u32 *)(Func_02001dde(2) + 248) & 1) != 0) {
        Func_02001e26(2, 0, 0, 0);
        Func_02001f54(126);
        Func_02001e0a(0);
        return Func_02001e10(2);
    }
}
