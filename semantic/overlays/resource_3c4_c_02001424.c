/*
 * resource_3c4 owner at 0x02001424, 50 bytes: code 0x02001424-0x02001455,
 * ending on the alignment halfword at 0x02001456 before the prologue at
 * 0x02001458.  No pool word - the +Z offset is materialised as
 * `movs r1,#128 ; lsls r1,r1,#14` = 0x00200000.
 *
 * The +Z sibling of 0x020013b4/0x02001348/0x02000df4, with one addition: it
 * inspects the move routine's result.  Func_02000cd0 returns 1 when the move
 * was refused, and only then does this owner run the local cutscene step at
 * 0x020013e0.
 *
 * Targets resolved with `tools/overlay_call_targets.ts` (overlay `bl` rule
 * `true_target_offset = stored_displacement + 2`); the disassembler's
 * 0x02004538, 0x02002116 and 0x0200282e are artefacts.  The real targets are
 * the import veneer at 0x0200310c (-> Func_0808a080) and this overlay's
 * prologues at 0x02000cd0 and 0x020013e0.
 *
 * Previously skipped because 0x02002116 and 0x0200282e both looked like
 * in-image addresses that are not function starts; both were decoding
 * artefacts.
 *
 * The epilogue is `add sp,#12 ; pop {r0} ; bx r0`, so the owner is void.
 */
typedef signed int s32;
typedef unsigned char u8;

typedef struct Actor_02001424 {
    u8 pad0[8];
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
} Actor_02001424;

/* Returns the actor record for the given slot. */
Actor_02001424 *Func_0808a080();

/* Local: moves the actor to the three-word 12.20 target; 1 = refused. */
s32 Func_02000cd0(const s32 *target);

void Func_020013e0(void);

void Func_02001424(void)
{
    Actor_02001424 *actor = Func_0808a080(0);
    s32 target[3];

    target[0] = actor->x;
    target[1] = actor->y;
    target[2] = actor->z + 0x00200000;

    if (Func_02000cd0(target) != 0) {
        Func_020013e0();
    }
}
