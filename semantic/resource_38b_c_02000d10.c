#include "types.h"

/*
 * Resource 38b scene entry at 0x02000d10 (112 bytes, 8 distinct callees).
 *
 * Complete owner: `push {r5, lr}` at 0x02000d10 and `pop {r5} / pop {r0} /
 * bx r0` at 0x02000d6e — the popped r0 is the return address, so the owner is
 * `void`.  0x02000d74-0x02000d7f is the literal pool.
 *
 * Reached from the dispatcher at 0x02000890 for scene id 0x20.
 *
 * ADDITIVE displacement/value shape (HANDOVER section 0).  The sequence is
 * `movs r2,#224 / lsls r2,#1` giving 448, `adds r3,r3,r2` forming the
 * address, and only then `adds r2,#68` giving 516, which is the value
 * stored.  The offset that matters is the PRE-arithmetic 448; reading this
 * as a store to workspace + 516 is the documented natural mistake.  The
 * store is *(s32 *)(workspace + 448) = 516.  Cross-check: 0x02000328 and
 * 0x02000890 both write the same field at offset 448 (with value 256), which
 * confirms the offset independently.
 *
 * The scene test uses Data_02000240 at byte offset 450 — index 225, the
 * halfword one past the scene id at [224] that 0x02000890 switches on.  The
 * comparison is `(u16)(scene - 3) << 16` against 0x00010000 with `bhi`
 * (unsigned), so it admits exactly scene == 3 and scene == 4.
 *
 * All eight callees resolved with tools/lib/overlay_call_targets.ts: seven import
 * veneers and this overlay's own prologue at 0x02000fa0.
 */

/* The overlay's scene workspace pointer CELL. */
extern u8 *Data_03001ebc;

/* Cross-overlay scene-state halfword array. */
extern s16 Data_02000240[];

/* Imports.  Func_080770c0 is used in a condition, so it needs a return type;
 * the rest are old-style, arity open. */
extern void Func_02001cc2(void);
extern s32 Func_02001dd6();
extern u8 *Func_02001e18();
extern void Func_02001e06();
extern void Func_02001ed6();
extern void Func_02001db2();
extern void Func_02001e26();
extern void Func_02001d84();

                            

                            

/* This overlay's collision-rectangle repaint. */

/* Progress flag guarding the one-shot warp below. */
#define FLAG_WARP_DONE 0x109

void Func_02000d10(void)
{
    u8 *workspace = Data_03001ebc;
    u16 sub_scene;
    u8 *actor;

    *(s32 *)(workspace + 448) = 516;

    Func_02001cc2();

    sub_scene = *(u16 *)((u8 *)Data_02000240 + 450);
    if ((u16)(sub_scene - 3) > 1) {
        return;
    }
    if (Func_02001dd6(FLAG_WARP_DONE) != 0) {
        return;
    }

    /* Slot 0 is the player.  Not null-checked here, unlike 0x02000fa0. */
    actor = Func_02001e18(0);
    Func_02001e06();

    /* 128 << 13 = 0x00100000.  The height is both written into the record's
     * +0x0c field and passed as the second argument of the reposition call,
     * whose x and z come from the record's existing +0x08 / +0x10 words. */
    *(s32 *)(actor + 0x0c) = 0x00100000;
    Func_02001ed6(*(s32 *)(actor + 0x08), 0x00100000,
                  *(s32 *)(actor + 0x10), 0);

    /* Neither of the next two calls sets any argument register.  r0 happens
     * to carry the previous callee's result across each of them; the same
     * shape appears for the Func_0808a018 / Func_0808a020 begin-end pair in
     * 0x02000240 and 0x02000328, where the pair is plainly argument-less, so
     * both are read as no-argument calls here.  Recorded as an uncertainty. */
    Func_02001db2();
    Func_02001e26();

    Func_02001d84(1);
}
