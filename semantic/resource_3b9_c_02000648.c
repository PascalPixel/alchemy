#include "types.h"

/*
 * resource_3b9 owner at 0x02000648, 116 bytes: the tenth published
 * handler in the bank at 0x020002a0-0x020006bc, for actor 18.
 *
 * PUBLISHED, NOT CALLED. Found by
 * `bun tools/overlay_published.ts resource_3b9`. The bank is described
 * in resource_3b9_c_020002a0.c. Straight-line, no flag test.
 *
 * Its middle is a run of three Func_0808a1b8 calls whose second
 * arguments descend -- 0xd000, 0xb000, 0x8000, each built as
 * `movs r1,#<n> / lsls r1,#8` from 208, 176 and 128 -- while the third
 * argument steps 20, 20, 40. A descending sequence with a longer last
 * step reads like a settling or approach, but that is a guess from
 * shape and the values are transcribed, not interpreted.
 *
 * Complete owner: `push {lr}` at 0x02000648 through `pop {r0} / bx r0`
 * at 0x020006b4-0x020006b6, then the one-word literal pool at
 * 0x020006b8 (line id 0x2122) and a zero alignment halfword at
 * 0x020006ba; the next owner's prologue is at 0x020006bc.
 *
 * All `bl` targets resolved through the import-veneer table under the
 * +2 rule; the Func_02001c48 and Func_02001c5c calls are direct
 * intra-image branches.
 *
 * Uncertainty: Func_0808a1b8's arguments are unread beyond the actor id.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a150(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_02001c48(s32 arg0);
extern void Func_02001c5c(s32 arg0, s32 arg1);

void Func_02000648(void)
{
    Func_0808a018();
    Func_0808a150(18, 0, 20);
    Func_0808a170(0x2122);
    Func_02001c48(18);
    Func_0808a1b8(18, 0xd000, 20);
    Func_0808a1b8(18, 0xb000, 20);
    Func_0808a1b8(18, 0x8000, 40);
    Func_0808a150(18, 0, 20);
    Func_02001c48(18);
    Func_0808a110(18, 3);
    Func_02001c48(18);
    Func_02001c5c(18, 0x5000);
    Func_0808a020();
}
