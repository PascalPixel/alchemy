#include "types.h"

/*
 * Resource 3c8 at 0x02002f30: nine-call camera/scene setup step.
 *
 * The owner is complete: `push {lr}` at 0x02002f30 and the matching
 * `pop {r0} ; bx r0` at 0x02002f88..0x02002f8a.  No literal pool sits inside
 * the span and no live frame or register state escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * Every constant here is materialised as `movs #imm ; lsls #shift`, so the
 * values below are exact: 0x80<<8 = 0x8000, 0x80<<7 = 0x4000, 130<<2 = 520,
 * 178<<2 = 712, 130<<18 = 0x02080000 and 196<<18 = 0x03100000.
 *
 * CALL SYMBOLS ARE PER-SITE: the raw assembly (assets/code/resource_3c8_overlay.s
 * around line 3973) spells eight of the nine calls as direct `bl sub_02007xxx`
 * to absolute addresses inside the 0x0200xxxx EWRAM range (overlay-local or
 * resident-service routines, NOT main-image ROM imports) -- lifted verbatim
 * here, not re-derived from any veneer math. The fifth call (`bl .L_02002fc4`)
 * is the one genuinely indirect main-image call, routed automatically through
 * this overlay's own `_call_via_rN` bank; Func_02000058 is correct for it.
 */

/* Old-style declarations: the imports' real interfaces are not known here. */
void Func_02007dc8();
void Func_02007df6();
void Func_02007e2c();
void Func_02007eb8();
void Func_02000058();
void Func_02007e92();
void Func_02007e08();
void Func_02007f26();
void Func_02007e22();

void Func_02002f30(void)
{
    Func_02007dc8();
    Func_02007df6(0, 0x8000, 0x4000);
    Func_02007e2c(0, 520, 712);
    Func_02007eb8(0, 0x4000, 10);
    Func_02000058(0x02080000, 0, 0x03100000, 223);
    Func_02007e92(0, 6, 0);
    Func_02007e08(60);
    Func_02007f26(20);
    Func_02007e22();
}
