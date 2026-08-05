#include "types.h"

/*
 * resource_3a8 owner at 0x02001930, 1,340 bytes: the room's re-entry fix-up -
 * restore whatever state the two followers were left in, then replay either
 * follower's cutscene if its story flag says it already happened.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02001930 with an 8-byte frame,
 * through the single interworking epilogue at 0x02001e5a (`add sp,#8 /
 * pop {r5,r6} / pop {r0} / bx r0`).  Two literal pools sit in the span and are
 * data, not code: an INTERIOR one at 0x02001c86-0x02001ca7 (an alignment
 * halfword plus eight words, jumped over by the `b 0x02001cc2` at 0x02001c84
 * while 0x02001ca8 is reached only by the `b` at 0x02001a8e), and the trailing
 * one at 0x02001e62-0x02001e6b.  Both are excluded.  The 8-byte frame carries
 * the fifth and sixth arguments of the six-argument calls and nothing else.
 * Nothing is live past the return.
 *
 * Signature.  The epilogue is `pop {r0} ; bx r0`, so r0 holds the popped
 * return address and the owner returns nothing: `void`.  r0 is written with 0
 * before any read and r1-r3 are never read before being written, so it takes
 * no arguments.
 *
 * Call accounting: 82 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_3a8 1930`; 12 distinct import
 * veneers, no intra-overlay call.  Every per-target count was reproduced by
 * hand from `printed_target - site - 2` and matches the tool exactly:
 *   0x02003b9c -> Func_08009180 x49    0x02003ba4 -> Func_080091c0  x7
 *   0x02003bb4 -> Func_080091e0  x2    0x02003bd4 -> Func_080770c0  x7
 *   0x02003bdc -> Func_080770c8  x1    0x02003be4 -> Func_080770d0  x1
 *   0x02003c1c -> Func_0808a080  x6    0x02003c74 -> Func_0808a0f0  x4
 *   0x02003c7c -> Func_0808a100  x1    0x02003d4c -> Func_0808a408  x1
 *   0x02003d64 -> Func_0808a5e0  x2    0x02003b1c -> Func_080000d0  x1
 * The inventory row advertises `calls=80`; the difference is sites the row
 * folds.  The disassembler's own annotations are wrong in the usual overlay
 * way and were not used.
 *
 * LINK BASE 0x02008000, and this owner supplies the strongest witness in the
 * overlay: the pool word 0x0200b769 handed to Func_080000d0 as the per-frame
 * task is 0x3768 plus the Thumb bit, and file offset 0x3768 is Func_02003768 -
 * the follower-stepping routine reconstructed in this same reconstruction, whose
 * `push {r5, r6, lr}` prologue is right there.  Installing exactly that
 * routine here is also what the two owners mean together.  The remaining pool
 * words are the IWRAM pointers 0x03001ebc and (via `[r6,#36]`) 0x03001ee0 -
 * both already named by BYTE-EXACT siblings in assets/code
 * (resource_3a8_c_02003184.c and resource_3a8_c_020015b4.c) with these exact
 * types - the flag ids 0x109/0x200/0x201/0x302/0x303/0x915, the table base
 * 0x02000240 (again from a byte-exact sibling), and the coordinate 0x02da0000.
 *
 * The two scripted sequences at the end are the same two the sibling owner
 * 0x020032a4 plays; every literal in both blocks matches that reconstruction
 * argument for argument.  The difference is the entry condition: 0x020032a4
 * plays a sequence when the flag is still UNSET and latches it, this owner
 * replays it when the flag is already SET.  That is what makes this the
 * re-entry path.
 *
 * UNCERTAINTIES.
 *  - At 0x020019b4 the Func_0808a080(11) result is discarded; the very next
 *    instruction reloads r0 with 11 for Func_0808a100.  The call is kept for
 *    its side effect, which is what the code asserts.
 *  - `Data_03001ee0[6]` is set either to the selector-0 entity pointer or to 0.
 *    The byte-exact sibling for 0x020015b4 writes the same slot from
 *    Func_020051e6(0), so the slot holds a pointer; the s32 spelling is that
 *    sibling's and is kept.
 *  - `*(s32 *)(Data_03001ebc + 448) = 516` is a fixed slot write with no other
 *    witness in the tree; both numbers are literal.
 *  - Func_0808a408, Func_0808a100, Func_0808a5e0 and Func_080770d0 have no
 *    established interfaces here and are spelled old-style.
 *  - The six-argument Func_08009180 / Func_080091c0 literals are reproduced
 *    positionally; their parameter meanings are not established.
 */

struct SceneEntity {
    u8 unknown_00[6];
    u16 facing;                     /* 0x06, high nibble selects 1 of 16 */
    u8 unknown_08[0x1b];
    u8 flags23;                     /* 0x23 */
};

/* Old-style declarations: overlay imports vary in arity between call sites. */
struct SceneEntity *Func_0808a080();    /* scene entity by selector */
void Func_0808a0f0();                   /* place an entity at (x, z) */
void Func_0808a100();                   /* interface not established */
void Func_0808a408();                   /* interface not established */
void Func_0808a5e0();                   /* interface not established */
s32 Func_080770c0();                    /* read a story flag */
void Func_080770c8();                   /* set a story flag */
void Func_080770d0();                   /* clear a story flag */
void Func_080091e0();                   /* set presentation mode */
void Func_08009180();                   /* six-argument scripted-actor call */
void Func_080091c0();                   /* six-argument scripted-actor call */
void Func_080000d0();                   /* install a per-frame task */

/* IWRAM workspace pointers, both named by byte-exact siblings. */
extern u8 *Data_03001ebc;
extern s32 *Data_03001ee0;

/* Room state table, from the byte-exact sibling for 0x0200164c. */
extern s16 Data_02000240[];

void Func_02001930(void)
{
    struct SceneEntity *entity;
    s32 present;
    s32 room;

    *(s32 *)(Data_03001ebc + 448) = 516;

    Func_0808a408(0);

    present = Func_080770c0(0x109);
    if (present != 0) {
        s32 value = Func_080770c0(0x200);
        s32 *slots = Data_03001ee0;

        slots[6] = value;
        if (value != 0) {
            value = (s32)Func_0808a080(0);
        }
    } else {
        Func_080770c8(0x200);
        if (Data_02000240[225] == 4) {
            Data_03001ee0[6] = present;     /* present is 0 here */
            Func_080770d0(0x200);
        }
    }

    if (Func_080770c0(0x302) != 0) {
        Func_0808a0f0(11, 0x00960000, 0x02d80000);
        if (Func_080770c0(0x201) != 0) {
            Func_0808a080(11);              /* result discarded */
            Func_0808a100(11, 5);
            Func_080091c0(0, 0, 1, 1, 9, 14);
            Func_080091c0(0, 0, 1, 1, 9, 45);
            entity->flags23 = (u8)(entity->flags23 | 2);
            entity = Func_0808a080(11);
        }
    }

    Func_080091e0(Func_0808a080(8), 0);
    Func_080091e0(Func_0808a080(9), 0);

    /* 0x0200b769 = file offset 0x3768 + the Thumb bit = Func_02003768. */
    Func_080000d0((void (*)(void))0x0200b769, 3200);

    if (Func_080770c0(0x915) != 0) {
        Func_0808a0f0(10, 0x01aa0000, 0x02da0000);
        entity = Func_0808a080(10);
        Func_08009180(88, 48, 88, 45, 2, 3);
        Func_08009180(24, 49, 24, 48, 2, 1);
        entity->facing = 0x5000;
        Func_08009180(25, 42, 25, 47, 1, 1);
        Func_080091c0(22, 50, 2, 1, 24, 49);
    }

    if (Func_080770c0(0x302) != 0) {
        Func_0808a0f0(8, 0x00e80000, 0x02dc0000);
        Func_080091c0(7, 44, 1, 1, 0, 1);
        Func_08009180(74, 58, 78, 41, 1, 5);
        Func_08009180(16, 109, 13, 109, 3, 2);

        Func_08009180(67, 64, 71, 44, 1, 2);
        Func_08009180(67, 64, 72, 44, 1, 2);
        Func_08009180(67, 68, 73, 43, 1, 2);
        Func_08009180(67, 68, 74, 43, 1, 2);
        Func_08009180(67, 64, 75, 44, 1, 2);
        Func_08009180(67, 66, 76, 44, 1, 2);
        Func_08009180(67, 64, 77, 44, 1, 2);
        Func_08009180(67, 64, 78, 44, 1, 2);
        Func_08009180(67, 64, 79, 44, 1, 2);
        Func_08009180(67, 66, 80, 44, 1, 2);
        Func_08009180(2, 0, 9, 42, 2, 2);

        Func_08009180(68, 64, 71, 44, 1, 2);
        Func_08009180(68, 64, 72, 44, 1, 2);
        Func_08009180(68, 68, 73, 43, 1, 2);
        Func_08009180(68, 68, 74, 43, 1, 2);
        Func_08009180(68, 64, 75, 44, 1, 2);
        Func_08009180(68, 66, 76, 44, 1, 2);
        Func_08009180(68, 64, 77, 44, 1, 2);
        Func_08009180(68, 64, 78, 44, 1, 2);
        Func_08009180(68, 64, 79, 44, 1, 2);
        Func_08009180(68, 66, 80, 44, 1, 2);
        Func_08009180(4, 0, 9, 42, 2, 2);

        Func_08009180(7, 11, 7, 42, 10, 8);
        Func_08009180(71, 12, 71, 43, 10, 13);
        Func_080091c0(6, 13, 12, 12, 6, 44);
        Func_080091c0(0, 1, 1, 1, 7, 44);
    } else {
        room = Data_02000240[225];
        if (room >= 1 && room <= 2) {
            Func_0808a5e0(170);
        }
    }

    if (Func_080770c0(0x303) != 0) {
        Func_0808a0f0(9, 0x02b80000, 0x02dc0000);
        Func_08009180(74, 58, 107, 41, 1, 5);
        Func_08009180(45, 109, 42, 109, 3, 2);

        Func_08009180(67, 64, 102, 44, 1, 2);
        Func_08009180(67, 64, 103, 44, 1, 2);
        Func_08009180(67, 64, 104, 44, 1, 2);
        Func_08009180(67, 66, 105, 44, 1, 2);
        Func_08009180(67, 64, 106, 44, 1, 2);
        Func_08009180(67, 64, 107, 44, 1, 2);
        Func_08009180(67, 64, 108, 44, 1, 2);
        Func_08009180(67, 66, 109, 44, 1, 2);

        Func_08009180(68, 64, 102, 44, 1, 2);
        Func_08009180(68, 64, 103, 44, 1, 2);
        Func_08009180(68, 64, 104, 44, 1, 2);
        Func_08009180(68, 66, 105, 44, 1, 2);
        Func_08009180(68, 64, 106, 44, 1, 2);
        Func_08009180(68, 64, 107, 44, 1, 2);
        Func_08009180(68, 64, 108, 44, 1, 2);
        Func_08009180(68, 66, 109, 44, 1, 2);

        Func_08009180(38, 14, 38, 44, 8, 4);
        Func_08009180(102, 14, 102, 44, 8, 12);
        Func_080091c0(37, 13, 10, 12, 37, 43);
    } else {
        room = Data_02000240[225];
        if (room >= 3 && room <= 4) {
            Func_0808a5e0(170);
        }
    }
}
