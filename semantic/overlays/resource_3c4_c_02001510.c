/*
 * resource_3c4 owner at 0x02001510, 32 bytes: code 0x02001510-0x0200152f, no
 * pool and no alignment (0x02001530 is the next prologue).
 *
 * A position guard.  It fetches the actor record with import Func_0808a080(0)
 * and runs the local move step at 0x02001348 unless the actor is standing on
 * tile (32, 50).  The two `asrs #20` make both comparisons signed whole-tile
 * tests on the 12.20 words at +8 and +16.
 *
 * The branch shape is `bne -> call` then `beq -> skip`, i.e. the call is taken
 * on every path except x == 32 && z == 50.
 *
 * Targets resolved with `tools/overlay_call_targets.ts` (overlay `bl` rule
 * `true_target_offset = stored_displacement + 2`); the disassembler's
 * 0x02004622 and 0x02002872 are artefacts.  The real targets are the import
 * veneer at 0x0200310c (-> Func_0808a080) and this overlay's prologue at
 * 0x02001348.
 *
 * Previously skipped because 0x02002872 looked like an in-image address that
 * is not a function start; that address never existed.
 *
 * The epilogue is `pop {r0} ; bx r0`, so the owner is void.  Func_02001348's
 * result is not inspected here, and r0 is overwritten by the popped return
 * address before the `bx`.
 */
typedef signed int s32;
typedef unsigned char u8;

typedef struct Actor_02001510 {
    u8 pad0[8];
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
} Actor_02001510;

/* Returns the actor record for the given slot. */
Actor_02001510 *Func_0808a080();

void Func_02001348(void);

void Func_02001510(void)
{
    Actor_02001510 *actor = Func_0808a080(0);

    if ((actor->x >> 20) == 32 && (actor->z >> 20) == 50) {
        return;
    }
    Func_02001348();
}
