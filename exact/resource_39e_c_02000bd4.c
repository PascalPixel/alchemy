#include "types.h"

/*
 * resource_39e owner at 0x02000bd4, 32 bytes: a one-line scripted beat --
 * open the scene bracket, speak line 0x182d, run Func_020050a2 on slot
 * 15, close the bracket.
 *
 * Complete owner: `push {lr}` at 0x02000bd4 through `pop {r0} / bx r0` at
 * 0x02000bec-0x02000bee, then the single pool word at 0x02000bf0
 * (0x0000182d). The next owner's prologue is exactly at 0x02000bf4 --
 * that is the already-drafted Func_02000bf4. **32 bytes**, measured to
 * the epilogue.
 *
 * This is the row whose size the published sweep's residue list would
 * have got right by accident; it is recorded here as measured anyway,
 * because a residue-list bound bounds against the next UNDRAFTED row and
 * is not a span source.
 *
 * Published population (sweep B). All 4 `bl` sites resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_39e bd4 bf4`; sites=4 and
 * four bl lines were transcribed.
 *
 * SHARED IDIOMS, cited: the Func_02004fb4/Func_02004fce scripted-scene
 * bracket and dialogue-by-id Func_0200507a, both from
 * resource_39e_c_02001494.c.
 *
 * Uncertainty: Func_020050a2(15, 0) is transcribed, not identified. The
 * dialogue id 0x182d sits immediately below 0x182a, the default line in
 * this overlay's 0x02000afc exit chain, so the two probably belong to one
 * text block -- that is an observation about id adjacency, not evidence.
 */

extern void Func_02004fb4(void);
extern void Func_02004fce(void);
extern void Func_0200507a(s32 dialogueId);
extern s32 Func_020050a2(s32 slot, s32 arg1);

void Func_02000bd4(void)
{
    Func_02004fb4();
    Func_0200507a(0x182d);
    Func_020050a2(15, 0);
    Func_02004fce();
}
