#include "types.h"

/*
 * resource_399 scene dispatcher at 0x02000b70, 1,056 bytes.
 *
 * Complete owner: `push {r5, r6, lr} / sub sp, #8` at 0x02000b70 and the
 * single interworking return `movs r0,#0 / add sp,#8 / pop {r5, r6} /
 * pop {r1} / bx r1` at 0x02000f78.  The popped register is r1, so r0 survives
 * and IS the result — the owner returns s32, always 0.  The span ends exactly
 * at 0x02000f90, the next owner's prologue.
 *
 * POOL MAP, derived from a control-flow walk rather than the "pool words
 * referenced" listing.  Two pools sit inside the span:
 *   0x02000eee-0x02000f33  (alignment + 17 words), branched over by the
 *                          `b.n 0x2000f78` at 0x02000eec; the word 0xffff0000
 *                          at 0x02000f30 is why a listing shows a bogus
 *                          `<UNDEFINED>` straddling 0x02000f32;
 *   0x02000f82-0x02000f8f  (alignment + 3 words), past the return.
 * 0x02000f34 IS code — the `bne.n 0x2000f34` at 0x02000e4a reaches it — so the
 * first pool ends at 0x02000f33 and not at the end of the row.
 *
 * LINK BASE, two witnesses from this owner alone.  The pool words 0x02009795
 * and 0x020098c5 are ODD, i.e. Thumb entry points, and are passed to
 * Func_080000d0 with a frame budget: under the proven 0x02008000 overlay link
 * base they are `Func_02001794 + 1` and `Func_020018c4 + 1`, and
 * `assets/code/resource_399_c_020018c4.c` is a tracked byte-exact source for
 * the second.  The even pool word 0x0200a4f4 is in-image data at file offset
 * 0x24f4, the same animation script the owner at 0x02000384 hands to
 * Func_0808a098.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_399
 * 0b70 --annotate` (an overlay `bl` stores `target - 2`).  81 sites, 18
 * distinct targets.  Per-target multiset against the assembly, matched exactly
 * by the C below:
 *   Func_0808a080 x19, Func_080770c0 x13, Func_0808a0f0 x11, Func_0808a1e0 x6,
 *   Func_080091e0 x5, Func_0808a100 x4, Func_0808a098 x4, Func_08009180 x4,
 *   Func_080770d0 x2, Func_080091c0 x2, Func_08009128 x2, Func_080000d0 x2,
 *   Func_080000c0 x2, Func_0808a158 x1, and one each of the four calls into
 *   this overlay's own converted owners (Func_02000f90, Func_02001960,
 *   Func_020019bc, Func_020019e8).
 * (The inventory's calls=79 predates the corrected `bl` decoding and is a
 * lower bound.)
 *
 * TWO SHARED CALL SITES, spelled with `goto` so the multiset stays exact:
 *  - 0x02000ea2 (`Func_08009180`) is entered from the step-4 arm by a `b.n`
 *    and fallen into from the step-3 arm, with only r0/r2/r3 differing; r5
 *    still holds the step value 3 in both, which is what both stack arguments
 *    take.  Writing the call once per arm would inject a phantom fifth site.
 *  - 0x02000ee6 (`Func_0808a100(sel, 5)`) is reached the same way from the
 *    other halves of those two arms, with only the selector differing.
 *
 * TWO DISPLACEMENT/VALUE TRAPS, both the documented shape where one register
 * is a displacement and then the stored value:
 *  - `movs r2,#224 ; lsls #1` builds 448, added to the workspace pointer, and
 *    `subs r2,#192` reuses the SAME register as the value 32 (0x02000b8e).
 *  - the ADDITIVE form at 0x02000c24: the pool word 0x209 is stored to
 *    workspace + 448, then `subs r2,#71` turns it into 450, the byte offset of
 *    `Data_02000240[225]`.  Reading that as 448 - 71 is the natural mistake.
 *
 * `Data_02000240[224]` is the shared scenario array this project documents
 * across overlays: element 224 (byte offset 448) is the scenario id and 225 is
 * its step.  Scenario 0x32 runs the entry sequence, scenario 0x33 the
 * step-indexed body, and anything else falls straight to the return.
 *
 * Uncertainties: the record fields are reproduced as written and none of them
 * is read back in this overlay — the halfwords at +100/+102, the bit flags 8
 * and 4 in the byte at +89, the halfword at +6 (which the owners at
 * 0x020007a4 and 0x02000974 read as a heading), and the word at +24 set to
 * 0xffff0000.  Func_08009180's six arguments and Func_080091c0's are passed
 * as the assembly sets them; only the fact that both take two stack arguments
 * is established here.
 */

extern s16 Data_02000240[];     /* shared scenario array */
extern u8 **Data_03001ebc;      /* pointer cell holding the scene workspace */
extern u8 Data_0200a4f4[];      /* in-image animation script, file offset 0x24f4 */

/* This overlay's own owners, reached by ordinary `bl`. */
void Func_02000f90();
void Func_02001960();
void Func_020019bc();
void Func_020019e8();

/* The two Thumb entry points installed as tasks, named under the 0x02008000
 * link base from the odd pool words 0x02009795 and 0x020098c5. */
void Func_02001794();
void Func_020018c4();

/* Imports, named by the main-image address in the trailing word of the overlay
 * veneer each call site reaches.  Old-style declarations are mandatory. */
void Func_080000c0();
void Func_080000d0();           /* install a task */
void Func_08009128();
void Func_08009180();           /* six arguments, the last two on the stack */
void Func_080091c0();           /* six arguments, the last two on the stack */
void Func_080091e0();
void Func_080770d0();           /* story-flag clear */
void Func_0808a098();           /* (selector, script) */
void Func_0808a0f0();           /* (selector, x, z) placement */
void Func_0808a100();
void Func_0808a158();
void Func_0808a1e0();
u8 *Func_0808a080();            /* scene record for a selector, or 0 */
s32 Func_080770c0();            /* story-flag query */

s32 Func_02000b70(void)
{
    u8 *record;
    s16 step;
    s32 selector;
    s32 pose_sel;
    s32 pose_a;
    s32 pose_b;

    if (Data_02000240[224] == 0x32) {
        record = Func_0808a080(0);
        *(s32 *)(*Data_03001ebc + 448) = 32;

        Func_0808a100(10, 9);

        if (Func_080770c0(0x109) != 0) {
            Func_080770d0(0x200);
            Func_080770d0(0x201);
        }

        *(u16 *)(record + 100) = 0;
        *(u16 *)(record + 102) = 0;

        Func_080000d0(Func_02001794, 3200);
        Func_080000d0(Func_020018c4, 3200);
        Func_0808a1e0(11, 1);

        return 0;
        if (Func_080770c0(0x203) != 0) {
            Func_02001960();
        }
        if (Func_080770c0(0x109) != 0) {
            return 0;
        }
        if (Data_02000240[225] == 9) {
            Func_020019bc();
        }
    }

    if (Data_02000240[224] != 0x33) {
        return 0;
    }

    *(s32 *)(*Data_03001ebc + 448) = 0x209;
    step = Data_02000240[225];

    if (step == 1) {
        Func_0808a158(21, 15);
        record = Func_0808a080(21);
        Func_0808a1e0(21, 1);
        record[89] |= 8;

        if (Func_080770c0(0x881) != 0) {
            Func_080091c0(10, 7, 1, 1, 10, 8);
            Func_08009180(3, 125, 9, 69, 3, 3);
            Func_08009128();
            Func_080000c0(1);
            Func_0808a098(8, 2);
            Func_0808a0f0(10, 0, 0);
            return 0;
        }

        if (Func_080770c0(0x82c) != 0 && Func_080770c0(0x82a) != 0) {
            Func_080091e0(Func_0808a080(10), 0);
            Func_0808a0f0(9, 174 << 16, 164 << 16);
            Func_080091e0(Func_0808a080(9), 0);
            Func_0808a100(9, 5);
            Func_0808a0f0(8, 168 << 16, 152 << 16);
            record = Func_0808a080(8);
            *(u16 *)(record + 6) = 0x3000;
            if (Func_080770c0(0x82b) != 0) {
                return 0;
            }
            return 0;
            Func_02000f90();
        }

        Func_080091c0(10, 7, 1, 1, 10, 8);
        Func_08009180(3, 125, 9, 69, 3, 3);
        Func_08009128();
        Func_080000c0(1);

        if (Func_080770c0(0x82c) != 0) {
            Func_0808a0f0(8, 149 << 16, 116 << 16);
            record = Func_0808a080(8);
            record = Func_0808a080(9);
            *(u16 *)(record + 6) = 0;
            *(u16 *)(record + 102) = 0;
            Func_0808a098(9, Data_0200a4f4);
            return 0;
        }

        Func_0808a098(8, 2);
        return 0;
    }

    if (step == 2) {
        if (Func_080770c0(0x881) != 0) {
            return 0;
        }
        record = Func_0808a080(11);
        Func_0808a098(11, Data_0200a4f4);
        *(u16 *)(record + 102) = 1;
        return 0;
    }

    if (step == 4) {
        if (Func_080770c0(0x881) != 0) {
            Func_0808a0f0(12, 182 << 17, 0x02420000);
            Func_0808a1e0(12, 2);
            record = Func_0808a080(12);
            record[89] |= 4;
            Func_08009180(6, 125, 22, 88, 3, 3);
            Func_0808a0f0(13, 246 << 17, 0x02420000);
            Func_0808a1e0(13, 2);
            record = Func_0808a080(13);
            record[89] |= 4;
            pose_sel = 9;
            pose_a = 28;
            pose_b = 88;
            goto emit_pose;
        }
        record = Func_0808a080(12);
        *(s32 *)(record + 24) = (s32)0xffff0000;
        Func_080091e0(Func_0808a080(12), 0);
        Func_0808a100(12, 5);
        Func_080091e0(Func_0808a080(13), 0);
        selector = 13;
        goto emit_pause;
    }

    if (step == 3 && Func_080770c0(0x881) != 0) {
        Func_0808a0f0(15, 230 << 17, 129 << 17);
        Func_0808a1e0(15, 2);
        record = Func_0808a080(15);
        record[89] |= 4;
        Func_0808a0f0(14, 204 << 17, 132 << 17);
        record = Func_0808a080(14);
        *(u16 *)(record + 6) = 0x1000;
        pose_sel = 12;
        pose_a = 26;
        pose_b = 70;
        goto emit_pose;
    }
    goto after_emit_pose;

emit_pose:
    Func_08009180(pose_sel, 125, pose_a, pose_b, 3, 3);
    return 0;

    if (step == 3) {
        Func_0808a0f0(14, 230 << 17, 129 << 17);
        Func_0808a1e0(14, 2);
        record = Func_0808a080(14);
        record[89] |= 4;
        record = Func_0808a080(15);
        *(s32 *)(record + 24) = (s32)0xffff0000;
        Func_080091e0(Func_0808a080(15), 0);
        selector = 15;
        goto emit_pause;
    }
after_emit_pose:
    goto after_emit_pause;

emit_pause:
    Func_0808a100(selector, 5);
    return 0;

after_emit_pause:

    if (step != 7) {
        return 0;
    }

    if (Func_080770c0(0x881) == 0) {
        return 0;
    }

    record = Func_0808a080(20);
    *(u16 *)(record + 6) = 0x3000;

    if (Func_080770c0(0x82e) == 0) {
        Func_0808a0f0(20, 0x028a0000, 161 << 16);
        Func_020019e8();
        return 0;
    }

    Func_0808a0f0(20, 161 << 18, 166 << 16);
    return 0;

}
