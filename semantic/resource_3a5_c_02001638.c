#include "types.h"

/*
 * resource_3a5 owner at 0x02001638, 572 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools-rs/overlay-driver resource_3a5), so the loader enters
 * here and no bl inside the image reaches it, which is why it sat in
 * no structural inventory row.
 *
 * Shape: three halfwords into the loader block first, unconditionally,
 * BEFORE any scene test -- [278] = 600, [279] = 0, [280] = 281.  Then
 * the scene id at [224] is read once into a register that is kept live
 * for the whole body: if it is 0x5c the driver returns 0 immediately,
 * having done nothing but those three writes.
 *
 * Otherwise: 256 into the workspace's +448 s32, Func_02001c1c (an
 * in-image setup owner at 0x02001c1c), and callback 0x02008cd1
 * installed with 3200.  Then the same scene id selects one of three
 * arms, each a straight run of six-argument Func_080091b8 calls of the
 * form (a, b, 4, kind, slot, limit) with slot stepping by 4 from 0x40:
 * - scene 0x59, kind 2, limit 126, six entries:
 *   (22,7) (8,10) (23,21) (16,42) (36,44) (14,55);
 * - scene 0x5a, kind 2, limit 126, fourteen entries:
 *   (42,5) (20,11) (14,12) (56,18) (7,22) (44,23) (38,24) (26,28)
 *   (17,35) (50,36) (34,43) (6,46) (27,55) (43,56);
 * - scene 0x5b: Func_0808a5e0(169) first, then kind 4, limit 124,
 *   three entries: (8,14) (6,18) (10,21).
 * Any other scene id falls through all three arms and does nothing.
 * All arms then converge on Func_020004e4 (a second in-image owner)
 * and return 0.
 *
 * Note on the compiled form, kept out of the C because it is a codegen
 * detail and not a fact about the program: the 0x59 arm's last entry
 * does not emit its own call.  It loads slot 0x54 and (14, 55) and
 * branches to 0x020017f2, the tail of the 0x5a arm's last entry, which
 * supplies the shared `4, 2, limit` and the bl.  Both arms therefore
 * end at the same call site at 0x020017f8; the draft writes the two
 * calls out separately because that is what each path performs.
 *
 * Complete owner: `push {r5, lr}` at 0x02001638 with `sub sp, #8` at
 * 0x02001664, through the epilogue at 0x02001846-0x0200184e, then the
 * NINE-WORD literal pool at 0x02001850-0x02001873; the next function
 * begins at 0x02001874 (`push {lr}`).  Here the pool does trail the
 * body -- unlike resource_3a0, whose pool sits inside the function --
 * which is exactly why the pool is not usable as an end marker either
 * way and the span was read from the epilogue instead.
 *
 * Cross-checked per the 2026-08-01 ruling, hand-drafted from the bytes
 * first: tools/m2c_guard.ts clears this row of the call-through-
 * register hazard and finds no jump table, and reports full call
 * coverage against this draft.  Every bl target here was resolved as
 * an ABSOLUTE image offset (stored displacement + 2), not
 * pc-relative -- a plain objdump listing annotates these calls with
 * twenty different targets that are all in fact the one veneer at
 * 0x02001d14.
 *
 * Uncertainty: the three head halfwords are recorded by index with
 * their roles open, as is the meaning of the 0x59/0x5a/0x5b/0x5c scene
 * ids; Func_080091b8's six arguments are positional only, following
 * the resource_3a7 and resource_3ba precedent for the same import.
 * 0x02008cd1 is an in-image code pointer (image offset 0xcd0 | 1)
 * installed as a callback and never called here -- a data-installed
 * pointer of the kind neither a BL sweep nor the unindexed check can
 * see.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;

extern void Func_080000d0(s32 callback, s32 arg1);
extern void Func_080091b8();     /* six-argument scene request, last two on the stack */
extern void Func_0808a5e0(s32 arg0);
extern void Func_02001c1c(void);
extern void Func_020004e4(void);

s32 Func_02001638(void)
{
    s32 scene;
    s32 finalA;
    s32 finalB;
    s32 finalSlot;

    Data_02000240[278] = 600;
    Data_02000240[279] = 0;
    Data_02000240[280] = 281;

    if (scene == 0x5c) {
        return 0;
    }
    scene = Data_02000240[224];

    *(s32 *)(Data_03001ebc + 448) = 256;
    Func_02001c1c();
    Func_080000d0(0x02008cd1, 3200);

    if (scene == 0x59) {
        Func_080091b8(22, 7, 4, 2, 0x40, 126);
        Func_080091b8(8, 10, 4, 2, 0x44, 126);
        Func_080091b8(23, 21, 4, 2, 0x48, 126);
        Func_080091b8(16, 42, 4, 2, 0x4c, 126);
        Func_080091b8(36, 44, 4, 2, 0x50, 126);
        finalSlot = 0x54;
        finalA = 14;
        finalB = 55;
        goto kind2_tail;
    } else if (scene == 0x5a) {
        Func_080091b8(42, 5, 4, 2, 0x40, 126);
        Func_080091b8(20, 11, 4, 2, 0x44, 126);
        Func_080091b8(14, 12, 4, 2, 0x48, 126);
        Func_080091b8(56, 18, 4, 2, 0x4c, 126);
        Func_080091b8(7, 22, 4, 2, 0x50, 126);
        Func_080091b8(44, 23, 4, 2, 0x54, 126);
        Func_080091b8(38, 24, 4, 2, 0x58, 126);
        Func_080091b8(26, 28, 4, 2, 0x5c, 126);
        Func_080091b8(17, 35, 4, 2, 0x60, 126);
        Func_080091b8(50, 36, 4, 2, 0x64, 126);
        Func_080091b8(34, 43, 4, 2, 0x68, 126);
        Func_080091b8(6, 46, 4, 2, 0x6c, 126);
        Func_080091b8(27, 55, 4, 2, 0x70, 126);
        finalSlot = 0x74;
        finalB = 56;
        finalA = 43;
        goto kind2_tail;
    }
    goto maybe_scene_5b;

kind2_tail:
    Func_080091b8(finalA, finalB, 4, 2, finalSlot, 126);
    goto scenes_done;

maybe_scene_5b:
    if (scene == 0x5b) {
        Func_0808a5e0(169);
        Func_080091b8(8, 14, 4, 4, 0x40, 124);
        Func_080091b8(6, 18, 4, 4, 0x44, 124);
        Func_080091b8(10, 21, 4, 4, 0x48, 124);
    }

scenes_done:
    Func_020004e4();
    return 0;
}
