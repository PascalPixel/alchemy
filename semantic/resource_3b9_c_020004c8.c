#include "types.h"

/*
 * resource_3b9 owner at 0x020004c8, 68 bytes: the sixth published
 * handler in the bank at 0x020002a0-0x020006bc, for actor 14.
 *
 * PUBLISHED, NOT CALLED. Found by
 * `bun tools/overlay_published.ts resource_3b9`. The bank is described
 * in resource_3b9_c_020002a0.c. Straight-line, no flag test.
 *
 * The constant 258 appears twice here, both times built as
 * `movs r1,#129 / lsls r1,#1` -- once as Func_0808a1f0's second
 * argument and once as Func_0808a1e8's. The same instruction pair in
 * 0x0200039c yields 256 from `movs r1,#128`. Transcribed each time
 * rather than pattern-matched, which is the only way this shape stays
 * right.
 *
 * Complete owner: `push {lr}` at 0x020004c8 through `pop {r0} / bx r0`
 * at 0x02000504-0x02000506, then the one-word literal pool at
 * 0x02000508 (line id 0x2116) and a zero alignment halfword at
 * 0x0200050a; the next owner's prologue is at 0x0200050c.
 *
 * All `bl` targets resolved through the import-veneer table under the
 * +2 rule; the Func_02001c48 call is a direct intra-image branch.
 *
 * Uncertainty: Func_0808a1f0's and Func_0808a1e8's argument roles are
 * open.
 */










extern void Func_020030c0(void);
extern void Func_020031a2(s32 id, s32 arg1);
extern void Func_0200315a(s32 id, s32 arg1);
extern void Func_02003178(s32 dialogueId);
extern void Func_02002132(s32 arg0);
extern void Func_020031ba(s32 id, s32 arg1, s32 arg2);
extern void Func_020030fe(void);
void Func_020004c8(void)
{
    Func_020030c0();
    Func_020031a2(14, 258);
    Func_0200315a(14, 2);
    Func_02003178(0x2116);
    Func_02002132(14);
    Func_020031ba(14, 258, 40);
    Func_020031a2(14, 0);
    Func_020030fe();
}
