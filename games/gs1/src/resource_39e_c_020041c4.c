#include "types.h"

/*
 * resource_39e owner at 0x020041c4, 36 bytes: NOT in the unindexed
 * population -- found by boundary-checking 0x02004140's pool end
 * against 0x020041ec's prologue, the same way resource_3b9's bonus
 * 0x02001a14 was found. Tests bit 1 of the RAM flag word at
 * 0x03001e40 (the cell resource_36f_c_02000238.c tests bit 0 of) and
 * calls Func_0808a160(arg0, 10) if set, else Func_0808a160(arg0, 9)
 * -- both bl sites resolve to the SAME veneer (0x02004494 ->
 * Func_0808a160), only the second argument differs.
 *
 * Complete owner: `push {lr}` at 0x020041c4 through `pop {r0} / bx r0`
 * at 0x020041e2-0x020041e4, one pool word (0x03001e40) after an
 * alignment halfword, ending 0x020041eb; the next owner prologue
 * (`push {r5, r6, r7, lr}` plus the r8/r9/sl spill, the queue's
 * 0x020041ec row) begins at 0x020041ec.
 *
 * Uncertainty: r0 is untouched between entry and both call sites, so
 * the first argument is this function's own first argument by shape;
 * Func_0808a160's precedent signature (resource_39a_c_02001c18.c)
 * takes an object pointer first.
 */

extern void Func_0200866c(s32 arg0, s32 arg1);
extern void Func_02008674(s32 arg0, s32 arg1);

void Func_020041c4(s32 arg0)
{
    if ((*(u32 *)0x03001e40 >> 1) & 1) {
        Func_0200866c(arg0, 10);
    } else {
        Func_02008674(arg0, 9);
    }
}
