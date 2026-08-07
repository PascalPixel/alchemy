#include "types.h"

/*
 * resource_3b9 owner at 0x02000468, 96 bytes: the fifth published
 * handler in the bank at 0x020002a0-0x020006bc, for actor 13, and the
 * only member that writes to the actor's object directly.
 *
 * PUBLISHED, NOT CALLED. Found by
 * `bun tools/overlay_published.ts resource_3b9`. The bank is described
 * in resource_3b9_c_020002a0.c.
 *
 * NO FLAG TEST. Every member before this one branches on the bank's
 * shared story flag 0x962; this one is straight-line. So the shared
 * flag is not a bank-wide contract either -- it is a majority habit.
 *
 * It fetches the actor's object once with Func_0808a080(13), keeps it
 * in r5 across the body, and near the end stores two u16 fields:
 * +100 = 720 (built as `movs r3,#180 / lsls r3,#2`, so a computed
 * constant, not a pool word) and +102 = 112. The pointer for the second
 * store is produced by `adds r5,#102` -- the compiler consumed r5 for
 * the address rather than keeping the base, which is why the two stores
 * look asymmetric in the listing.
 *
 * Complete owner: `push {r5, lr}` at 0x02000468 through
 * `pop {r5} / pop {r0} / bx r0` at 0x020004be-0x020004c2, then the
 * one-word literal pool at 0x020004c4 (line id 0x2114) and a zero
 * alignment halfword at 0x020004c6; the next owner's prologue is at
 * 0x020004c8. The r5 spill exists for the object pointer.
 *
 * All `bl` targets resolved through the overlay's import-veneer table
 * under the +2 rule (tools/overlay_call_targets.ts); the call to this
 * overlay's own Func_02001c48 is a direct intra-image branch.
 *
 * Uncertainty: object offsets +100 and +102 are u16 by the `strh`
 * width; their meaning is unread, and 720/112 are transcribed.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern s32 Func_0808a080(s32 id);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0a8(s32 id);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a150(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a180(s32 id, s32 arg1);
extern void Func_02001c48(s32 arg0);

void Func_02000468(void)
{
    s32 object = Func_0808a080(13);

    Func_0808a018();
    Func_0808a0a8(13);
    Func_0808a150(13, 0, 20);
    Func_0808a170(0x2114);
    Func_02001c48(13);
    Func_0808a138(13, 1);
    Func_0808a180(13, 0);
    *(u16 *)(object + 100) = 720;
    *(u16 *)(object + 102) = 112;
    Func_0808a098(13, 2);
    Func_0808a020();
}
