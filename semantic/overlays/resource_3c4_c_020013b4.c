/*
 * resource_3c4 owner at 0x020013b4, 42 bytes: code 0x020013b4-0x020013dd,
 * ending on the alignment halfword at 0x020013de before the prologue at
 * 0x020013e0.  No pool word - the offset is materialised as
 * `movs r1,#128 ; lsls r1,r1,#14` = 0x00200000.
 *
 * The +X sibling of 0x02001348 and 0x02000df4: it fetches the record with
 * import Func_0808a080(0), adds +2.0 in 12.20 to the x word at +8, and hands
 * the resulting three-word target to the local move routine at 0x02000cd0.
 *
 * Targets resolved with `tools/overlay_call_targets.ts` (overlay `bl` rule
 * `true_target_offset = stored_displacement + 2`); the disassembler's
 * 0x020044c8 and 0x020020a6 are artefacts.  The real targets are the import
 * veneer at 0x0200310c (-> Func_0808a080) and this overlay's prologue at
 * 0x02000cd0.
 *
 * Previously skipped because 0x020020a6 looked like an in-image address that
 * is not a function start (it is two bytes into the prologue at 0x020020a4);
 * it was the decoding artefact.
 *
 * The epilogue is `add sp,#12 ; pop {r0} ; bx r0`, so the owner is void.
 */
typedef signed int s32;
typedef unsigned char u8;

typedef struct Actor_020013b4 {
    u8 pad0[8];
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
} Actor_020013b4;

/* Returns the actor record for the given slot. */
Actor_020013b4 *Func_0808a080();

/* Local: moves the actor to the three-word 12.20 target; 1 = refused. */
s32 Func_02000cd0(const s32 *target);

void Func_020013b4(void)
{
    Actor_020013b4 *actor = Func_0808a080(0);
    s32 target[3];

    target[0] = actor->x + 0x00200000;
    target[1] = actor->y;
    target[2] = actor->z;

    Func_02000cd0(target);
}
