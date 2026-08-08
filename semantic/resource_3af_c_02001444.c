#include "types.h"

/*
 * resource_3af owner at 0x02001444, 576 bytes: THE OVERLAY'S TOP-LEVEL
 * ENTRY DRIVER. It is the row every other owner in resource_3af hangs
 * off -- a 16-entry jump table that dispatches to fifteen of them, plus
 * a story-flag chain that picks a scene when the table falls through.
 *
 * TWO SPAN TRAPS, both resolved here, both worth reading before
 * trusting any measurement in this overlay:
 *
 *   1. The queue estimated 528 bytes because the next `push` after the
 *      body sits at 0x02001654. It is NOT a prologue: `push {r0, r2, r3,
 *      r4, r5, r7}` has no `lr`, so nothing could ever return through
 *      it -- and in fact those two halfwords are the pool word
 *      0x0200b4bd. The real epilogue is `pop {r5} / pop {r0} / bx r0`
 *      at 0x02001638-0x0200163c, the pool runs 0x0200163e-0x02001683,
 *      and the next real prologue is 0x02001684. True span: 576 bytes.
 *   2. The jump table itself, sixteen words at 0x02001518-0x02001557,
 *      disassembles as plausible `str r5,[sp,#imm]` instructions. Each
 *      word is an image address in the +0x8000 link form: 0x02009558
 *      means the case body at image 0x02001558.
 *
 * Selector: `*(s16 *)(Data_02000240 + 450)`, read SIGNED, then
 * `subs #4 / cmp #15 / bhi` -- an unsigned bound after a signed load,
 * so any negative value falls to the default arm along with 0..3 and
 * anything above 19. Live cases are 4 and 10 through 19; 5 through 9
 * are table entries that point at the default arm.
 *
 * Case 4 is the only arm that FALLS THROUGH into the tail chain rather
 * than returning: it edits two bits of a byte reached through the
 * record for slot 0 (`p = *(u8 **)(Func_0808a080(0) + 80);
 * p[9] = (p[9] & ~12) | 4;` -- the mask comes from `movs r3,#13 / negs
 * r3,r3`, which is -13, i.e. ~12) and then continues at 0x020015d0.
 * Every other live case ends with a branch to the epilogue.
 *
 * The tail chain (0x020015d0, shared by the default arm and case 4) is
 * a six-way story-flag ladder, first match wins: 0x93e writes 0x410000
 * to +236 through the context pointer at 0x03001e70; then 0x8a0 ->
 * Func_020019c0, 0x92b -> Func_02001920, 0x928 -> Func_0200185c,
 * 0x925 -> Func_020017a0, 0x911 -> Func_02001684.
 *
 * WHAT THIS CORROBORATES. The driver names, as dispatch targets or tail
 * arms, every semantic draft recorded in this overlay -- 0x0200185c,
 * 0x02001920, 0x02001684, 0x02001db0, 0x020029d4 -- plus the three
 * remaining queue rows 0x020022c0, 0x02002618, 0x02002c84 and
 * 0x02003a0c. That is independent confirmation that all of them are
 * real function entries at the addresses claimed, and it fixes the
 * three flags this overlay's scenes trade in: 0x92b (set by 0x020029d4,
 * tested here), 0x271 and 0x272 (the once-only reward flags of the
 * 0x02000bb8 / 0x02000bf0 twins, CLEARED here when flag 0x109 is set --
 * so 0x109 is a replay/reset condition, which is why the 0x0200185c and
 * 0x02001920 scene entries branch on 0x109 with the inverted sense).
 *
 * The driver also sets flag 0x144 unconditionally on entry, before
 * anything else.
 *
 * Two indirect installs go through Func_080000d0(callback, 3200), the
 * established per-frame task installer. Both pool words are Thumb
 * function pointers in the +0x8000 link form: 0x0200b4bd is
 * Func_020034bc (already tracked byte-exact) and 0x0200b1a9 is
 * Func_020031a8, a real `push {r5, r6, lr}` prologue that is in no
 * inventory row and has no draft -- an owner for a later pass, recorded
 * here because this is where it is published.
 *
 * A known-zero register reuse, same shape as 0x02001684's: r5 holds the
 * Func_080770c0(0x927) result, and the 0x928 arm is reached only when
 * that result was zero, so its two `str r5,[r3,#4]` stores write 0.
 *
 * Complete owner: `push {r5, lr}` at 0x02001444 through `pop {r5} /
 * pop {r0} / bx r0` at 0x02001638-0x0200163c; the jump table at
 * 0x02001518-0x02001557 is INSIDE the body with code branching over it
 * (the `mov pc, r3` at 0x02001514 enters it, the first case body
 * resumes at 0x02001558); an alignment halfword and seventeen trailing
 * pool words (0x00000109, 0x00000271, 0x00000272, 0x0000093e,
 * 0x00000927, 0x0200b4bd, 0x0200db50, 0x0200db60, 0x00013333,
 * 0x0200b1a9, 0x00000928, 0x02000240, 0x02009518, 0x03001e70,
 * 0x0000092b, 0x00000925, 0x00000911) end at 0x02001683, and the next
 * owner prologue (`push {r5, r6, lr}`, the drafted 0x02001684) begins
 * at 0x02001684.
 *
 * Second read done (over 400 bytes): the corrections it produced were
 * the two span traps above, and the table's stride -- a first pass read
 * the sixteen words as an 8-entry table of pairs because the repeated
 * 0x020095d0 makes five consecutive identical words; they are sixteen
 * separate 4-byte entries, five of which point at the same default arm.
 *
 * Not found by the structural inventory walk: reached only by `bl`
 * (cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3af 1444 1684, the +2
 * rule against the raw image; every dispatch target below was resolved
 * that way, and the table words by subtracting the 0x8000 link base).
 *
 * Uncertainty: the selector's meaning and the identity of the
 * Data_02000240 block are unknown; the case-4 bitfield edit is
 * transcribed, the field's role unknown.
 */

extern u8 Data_02000240[];      /* EWRAM block; +450 holds the scene selector */
extern u8 *Data_03001e70;       /* context pointer cell, IWRAM */
extern s32 Data_0200db50[];
extern s32 Data_0200db60[];

extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_080770d0(s32 flagId);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_080000d0(void *callback, s32 rank);

extern void Func_020017a0(void);
extern void Func_0200185c(void);
extern void Func_02001684(void);
extern void Func_02001920(void);
extern void Func_020019c0(void);
extern void Func_02001a98(void);
extern void Func_02001b58(void);
extern void Func_02001d0c(void);
extern void Func_02001db0(void);
extern void Func_02001f90(void);
extern void Func_020022c0(void);
extern void Func_02002618(void);
extern void Func_020029d4(void);
extern void Func_02002b7c(void);
extern void Func_02002c84(void);
extern void Func_020031a8(void);
extern void Func_02003284(void);
extern void Func_020034bc(void);
extern void Func_02003710(void);
extern void Func_02003950(void);
extern void Func_020039b8(void);
extern void Func_02003a0c(void);

void Func_02001444(void)
{
    s32 replaying;
    s32 selector;
    u8 *slot;
    u8 *context;

    s32 permuted_1;
    Func_080770c8(0x144);

    if (Func_080770c0(0x109) != 0) {
        Func_080770d0(0x271);
        Func_080770d0(0x272);
    }

    if (Func_080770c0(0x93e) != 0) {
        Func_02003950();
        Func_020039b8();
        Func_0808a158(24, 2);
    } else if (Func_080770c0(0x8a0) != 0) {
        Func_02003950();
        Func_020039b8();
    } else {
        replaying = Func_080770c0(0x927);
        if (replaying != 0) {
            Func_02003950();
            Func_02003284();
            Data_0200db50[1] = 128 << 14;
            Data_0200db60[1] = 0x13333;
            Func_080000d0((void *)((s32)&Func_020034bc | 1), 200 << 4);
            Func_080000d0((void *)((s32)&Func_020031a8 | 1), 200 << 4);
        } else if (Func_080770c0(0x928) != 0) {
            Data_0200db50[1] = 0;
            Data_0200db60[1] = 0;
            Func_02003950();
            Func_080000d0((void *)((s32)&Func_020031a8 | 1), 200 << 4);
        }
    }

    if (Func_080770c0(0x927) == 0) {
        Func_02003710();
    }

    selector = *(s16 *)(Data_02000240 + 450);
    switch (selector) {
    case 4:
        permuted_1 = *(u8 **)(Func_0808a080(0) + 80);
        slot[9] = (u8)((slot[9] & ~12) | 4);
        slot  = permuted_1;
        break;                  /* the only case that falls into the tail */
    case 10:
        if (Func_080770c0(0x928) != 0) {
            Func_02001b58();
        } else {
            Func_02001a98();
        }
        return;
    case 11:
        Func_02001d0c();
        return;
    case 12:
        Func_02001db0();
        return;
    case 13:
        Func_02001f90();
        return;
    case 14:
        Func_020022c0();
        return;
    case 15:
        Func_02002618();
        return;
    case 16:
        /* This branch joins the tail's 0x92b arm at its single physical
         * Func_02001920 call site. */
        if (Func_080770c0(0x109) != 0) {
            goto story_92b;
        } else {
            Func_020029d4();
        }
        return;
    case 17:
        Func_02002b7c();
        return;
    case 18:
        if (Func_080770c0(0x109) == 0) {
            Func_02002c84();
        }
        return;
    case 19:
        Func_02003a0c();
        return;
    default:
        break;
    }

    if (Func_080770c0(0x93e) != 0) {
        context = Data_03001e70;
        *(s32 *)(context + 236) = 130 << 15;
    } else if (Func_080770c0(0x8a0) != 0) {
        Func_020019c0();
    } else if (Func_080770c0(0x92b) != 0) {
story_92b:
        Func_02001920();
    } else if (Func_080770c0(0x928) != 0) {
        Func_0200185c();
    } else if (Func_080770c0(0x925) != 0) {
        Func_020017a0();
    } else if (Func_080770c0(0x911) != 0) {
        Func_02001684();
    }
}
