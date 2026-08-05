#include "types.h"

/*
 * resource_3cd owner at 0x0200071c, 76 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay_driver.ts resource_3cd), so the loader enters
 * here and no bl inside the image reaches it.  The smallest of the
 * twenty-one unowned drivers the audit turned up.
 *
 * It takes no branch at all: seed the workspace's +448 and +456 s32
 * cells with 516 and 24, give record 11 the same 0x19999 value in both
 * its +24 and +28 fields, then set modes on records 13 and 14 and
 * return 0.  Two separate Func_0808a080(11) calls fetch the same
 * record for the two stores, which is transcribed as compiled rather
 * than folded into one local.
 *
 * Complete owner: `push {r5, lr}` at 0x0200071c through `movs r0, #0 /
 * pop {r5} / pop {r1} / bx r1` at 0x02000756-0x0200075c, alignment
 * halfword, then the two-word literal pool 0x02000760-0x02000767
 * (0x03001ebc, 0x00019999); the next function's `push {r5, lr}`
 * follows at 0x02000768.  Returns a literal 0.
 *
 * Uncertainty: the workspace +448 value (516 here; neighbouring
 * overlays write 513, 514, 515 and 521 to the same cell) and the
 * record's +24/+28 fields are recorded by offset, roles open.
 */

extern u8 *Data_03001ebc;

extern u8 *Func_02000fd2(s32 id);
extern void Func_02000fec(s32 id, s32 arg1);

s32 Func_0200071c(void)
{
    *(s32 *)(Data_03001ebc + 448) = 516;
    *(s32 *)(Data_03001ebc + 456) = 24;
    *(s32 *)(Func_02000fc8(11) + 28) = 0x19999;
    *(s32 *)(Func_02000fd2(11) + 24) = 0x19999;
    Func_02000fe4(13, 5);
    Func_02000fec(14, 2);
    return 0;
}
