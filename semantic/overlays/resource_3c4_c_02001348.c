/*
 * resource_3c4 owner at 0x02001348, 44 bytes: code 0x02001348-0x0200136f plus
 * the single pool word 0xffe00000 at 0x02001370-0x02001373.
 *
 * Walks the actor two whole tiles in -X.  It fetches the record with import
 * Func_0808a080(0), builds the three-word 12.20 target on the stack from the
 * record's own position with -0x00200000 added to x, and hands it to the local
 * move routine at 0x02000cd0.
 *
 * 0xffe00000 is the signed constant -0x00200000, i.e. -2.0 in 12.20; it is
 * added, never subtracted, which is why it is pooled rather than materialised.
 *
 * Targets resolved with `tools/overlay_call_targets.ts` (overlay `bl` rule
 * `true_target_offset = stored_displacement + 2`); the disassembler's
 * 0x0200445c and 0x02002038 are artefacts.  The real targets are the import
 * veneer at 0x0200310c (-> Func_0808a080) and this overlay's prologue at
 * 0x02000cd0.
 *
 * Previously skipped because 0x02002038 looked like a mid-function in-image
 * address; it was the decoding artefact.
 *
 * The epilogue is `add sp,#12 ; pop {r0} ; bx r0`, so the owner is void.
 * Func_02000cd0 returns 1 when the move is refused, but this caller drops it.
 */
typedef signed int s32;
typedef unsigned char u8;

typedef struct Actor_02001348 {
    u8 pad0[8];
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
} Actor_02001348;

/* Returns the actor record for the given slot. */
Actor_02001348 *Func_0808a080();

/* Local: moves the actor to the three-word 12.20 target; 1 = refused. */
s32 Func_02000cd0(const s32 *target);

void Func_02001348(void)
{
    Actor_02001348 *actor = Func_0808a080(0);
    s32 target[3];

    target[0] = actor->x + (s32)0xffe00000;
    target[1] = actor->y;
    target[2] = actor->z;

    Func_02000cd0(target);
}
