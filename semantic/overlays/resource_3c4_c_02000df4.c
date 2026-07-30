/*
 * resource_3c4 owner at 0x02000df4, 44 bytes: code 0x02000df4-0x02000e1b plus
 * the single pool word 0xffe00000 at 0x02000e1c-0x02000e1f.
 *
 * The -Z sibling of 0x02001348: same 44-byte shape, same pool word, but the
 * -0x00200000 (-2.0 in 12.20) offset is applied to the record's z at +16
 * instead of its x at +8.  The three-word target is built on the stack and
 * handed to the local move routine at 0x02000cd0.
 *
 * Targets resolved with `tools/overlay_call_targets.ts` (overlay `bl` rule
 * `true_target_offset = stored_displacement + 2`); the disassembler's
 * 0x02003f08 and 0x02001ae4 are artefacts.  The real targets are the import
 * veneer at 0x0200310c (-> Func_0808a080) and this overlay's prologue at
 * 0x02000cd0.
 *
 * Previously skipped because 0x02001ae4 looked like an in-image address that
 * is not a function start; it was the decoding artefact.
 *
 * The epilogue is `add sp,#12 ; pop {r0} ; bx r0`, so the owner is void.
 */
typedef signed int s32;
typedef unsigned char u8;

typedef struct Actor_02000df4 {
    u8 pad0[8];
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
} Actor_02000df4;

/* Returns the actor record for the given slot. */
Actor_02000df4 *Func_0808a080();

/* Local: moves the actor to the three-word 12.20 target; 1 = refused. */
s32 Func_02000cd0(const s32 *target);

void Func_02000df4(void)
{
    Actor_02000df4 *actor = Func_0808a080(0);
    s32 target[3];

    target[0] = actor->x;
    target[1] = actor->y;
    target[2] = actor->z + (s32)0xffe00000;

    Func_02000cd0(target);
}
