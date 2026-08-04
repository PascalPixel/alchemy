#include "types.h"

/*
 * resource_3c5 owner at 0x02001238, 2264 bytes: 0x02001238-0x02001b0f.  One of
 * the overlay's two large cutscene scripts -- a straight-line sequence of
 * actor/camera commands separated by frame waits, with three dialogue
 * branches and a three-actor cleanup tail.
 *
 * FRAME AND SIGNATURE.  `push {lr}` only: no locals, no stack arguments, no
 * saved high registers.  The single epilogue at 0x02001b00 is
 * `pop {r0} ; bx r0`, so r0 holds the popped return address and the owner is
 * **void**.  It takes no arguments (r0 is written before any read).
 *
 * POOL MAP, derived from the owner's own control flow -- what the code branches
 * over -- and NOT from any referenced-words listing.  Three data regions, none
 * of them decoded as instructions, and register state is carried straight
 * across each of them:
 *   0x020015b4-0x020015cf   7 words, jumped over by the `b.n` at 0x020015b2
 *                           (0x988, 0x98a, 0x2702, 0x107, 0x101, 0x105,
 *                           0x03001ebc);
 *   0x0200186c-0x0200187f   5 words, jumped over by the `b.n` at 0x02001868
 *                           (0x03001ebc, 0x101, 0x14ccc, 0xa666, 0x107);
 *   0x02001b04-0x02001b0f   3 trailing words past the return, filling the row
 *                           to its 0x02001b10 end (0x03001ebc, 0x13333,
 *                           0x9999).
 * Both interior pools sit in the middle of a dialogue branch: the `bne` that
 * opens each branch targets the instruction immediately AFTER the pool, while
 * the fall-through arm ends in the `b.n` that hops it.  Nothing is dropped by
 * skipping them and nothing live crosses them unmodelled.
 *
 * CALL ACCOUNTING.  `bun tools/overlay_call_targets.ts resource_3c5 1238`
 * reports sites=251 / distinct_targets=29, all 29 import veneers, no prologue
 * and no `call_via`.  The inventory's `calls=230` is the usual lower bound and
 * is not used as a completeness proof.  The multiset comparison is: this file
 * contains 251 `Func_...(` occurrences with per-name multiplicities
 *   0808a010 x91  0808a180 x32  0808a1b8 x22  0808a110 x17  0808a1e8 x11
 *   0808a138 x11  0808a090 x11  0808a100 x6   0808a580 x5   0808a570 x4
 *   0808a0e8 x4   0808a0d0 x4   0808a178 x3   0808a130 x3   0808a0f0 x3
 *   0808a0b8 x3   0808a080 x3   0808a070 x3   0808a578 x2   0808a218 x2
 *   0808a210 x2   080770c8 x2   0808a460 x1   0808a208 x1   0808a170 x1
 *   0808a150 x1   0808a0c8 x1   0808a020 x1   0808a018 x1
 * which is exactly the tool's per-target table, entry for entry.
 *
 * THE SCRIPT'S IDIOM.  Func_0808a010(n) is the frame wait that separates
 * beats; it is 91 of the 251 sites and its argument is a frame count (10, 20,
 * 25, 30, 35, 40).  Func_0808a180(slot, 0) waits for the slot's queued action
 * to finish.  Func_0808a1b8(slot, heading, 0) faces a slot, and the headings
 * used here (0, 0x4000, 0x8000, 0xb000, 0xc000) are the same 16-bit angle
 * scale the rest of the overlay uses.  Func_0808a1e8(slot, id, frames) plays a
 * numbered animation, Func_0808a090/0808a0d0/0808a0f0 set positions, and
 * Func_0808a070(0, 0) is the dialogue call whose result selects the branch.
 *
 * THE DIALOGUE COUNTER.  Each of the three branches, on both arms, bumps the
 * halfword at byte 472 of the workspace: `ldr r3,=0x03001ebc / ldr r2,[r3] /
 * movs r3,#236 / lsls r3,#1 / adds r2,r2,r3 / ldrh / adds #1 / strh`.  The
 * workspace pointer variable is the same Data_03001ebc the byte-exact sibling
 * assets/code/resource_3c5_c_0200006c.c reads, and the overlay image is
 * writable EWRAM used as save state, so this is not const data.
 *
 * Uncertainties, recorded rather than guessed:
 *  - The three tail probes at 0x02001a70/0x02001aa0/0x02001ad0 read the record's
 *    s16 at +10 and +18.  Those are the integer parts of the 16.16 words at +8
 *    and +16, the halfword coordinate view the byte-exact resource_3c8:14f4
 *    already models twice for the same reason.
 *  - At each of those sites the register r3 still holds 18 when the `bl` is
 *    taken, because `movs r3,#18` supplied the load's offset.  That is the
 *    offset register, not a fourth argument; Func_0808a0b8 is spelled with
 *    three.  Recorded because an argument-window simulator reports it as
 *    `(slot, x, z, 18)` and the wrong reading is plausible.
 *  - The identity of the two 0x080770c8 progress ids 0x988 and 0x98a, and of
 *    the 0x2702 argument to Func_0808a170, is not established here; they are
 *    pooled constants passed straight through.
 *  - Every import is declared old-style: several are reached with different
 *    argument counts elsewhere in this overlay.
 */

extern u8 *Data_03001ebc;       /* workspace pointer variable */

/* The dialogue-progress counter each branch bumps. */
#define DIALOGUE_COUNTER (*(u16 *)(Data_03001ebc + 472))

/* Halfword coordinate view of a record: the integer parts of the 16.16 words
 * at +8 and +16. */
#define POS_X(rec) (*(s16 *)((u8 *)(rec) + 10))
#define POS_Z(rec) (*(s16 *)((u8 *)(rec) + 18))

/* Old-style declarations are mandatory in overlay sources. */
void Func_080770c8();
void Func_0808a010();           /* wait n frames */
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();            /* dialogue; result selects the branch */
u8 *Func_0808a080();            /* record by slot index, or 0 */
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a130();
void Func_0808a138();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();           /* wait for the slot's action to finish */
void Func_0808a1b8();           /* face slot along heading */
void Func_0808a1e8();           /* play animation id for n frames */
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a460();
void Func_0808a570();
void Func_0808a578();
void Func_0808a580();

void Func_02001238(void)
{
    u8 *record;

    Func_080770c8(0x988);
    Func_080770c8(0x98a);
    Func_0808a018();
    Func_0808a460();
    Func_0808a170(0x2702);
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a0d0(0, 0x128, 0x160);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a010(0xa);
    Func_0808a570(0xa, 0x10, 0, 0xc000);
    Func_0808a570(1, -8, 0x10, 0xc000);
    Func_0808a570(2, 8, 0x10, 0xc000);
    Func_0808a570(3, 0x18, 0x10, 0xc000);
    Func_0808a0e8(3);
    Func_0808a010(0x14);
    Func_0808a180(0xe, 0);
    Func_0808a010(0xa);
    Func_0808a208(0x30000, 0x6000);
    Func_0808a210(0x1180000, -1, 0x1200000, 1);
    Func_0808a218();
    Func_0808a010(0x14);
    Func_0808a110(0xb, 3);
    Func_0808a010(0x1e);
    Func_0808a180(0xb, 0);
    Func_0808a010(0xa);
    Func_0808a1b8(0xe, 0xc000, 0);
    Func_0808a1b8(0xb, 0xc000, 0);
    Func_0808a010(0x1e);
    Func_0808a010(0xa);
    Func_0808a138(0xe, 2);
    Func_0808a010(0x14);
    Func_0808a180(0xe, 0);
    Func_0808a010(0xa);
    Func_0808a130(0xd, 2);
    Func_0808a138(0xc, 2);
    Func_0808a010(0x28);
    Func_0808a130(0xd, 2);
    Func_0808a138(0xc, 2);
    Func_0808a010(0x28);
    Func_0808a130(0xd, 2);
    Func_0808a138(0xc, 2);
    Func_0808a010(0x28);
    Func_0808a1e8(0xc, 0x102, 0x32);
    Func_0808a180(0xc, 0);
    Func_0808a010(0xa);
    Func_0808a1b8(0xd, 0, 0);
    Func_0808a010(0x19);
    Func_0808a138(0xd, 2);
    Func_0808a010(0x14);
    Func_0808a180(0xd, 0);
    Func_0808a010(0xa);
    Func_0808a1b8(0xc, 0x8000, 0);
    Func_0808a010(0x14);
    Func_0808a110(0xd, 4);
    Func_0808a010(0x14);
    Func_0808a180(0xd, 0);
    Func_0808a010(0xa);
    Func_0808a110(0xb, 4);
    Func_0808a010(0x14);
    Func_0808a180(0xb, 0);
    Func_0808a010(0x14);
    Func_0808a1e8(0xd, 0x107, 0x28);
    Func_0808a010(0xa);
    Func_0808a1b8(0xd, 0x4000, 0);
    Func_0808a010(0x14);
    Func_0808a180(0xd, 0);
    Func_0808a010(0x14);
    Func_0808a1e8(0xd, 0x101, 0x4b);
    Func_0808a1e8(0xe, 0x101, 0x3c);
    Func_0808a1b8(0xc, 0x4000, 0);
    Func_0808a010(0x14);
    Func_0808a1b8(0xb, 0x4000, 0);
    Func_0808a010(0x14);
    Func_0808a1b8(0xe, 0x4000, 0);
    Func_0808a010(0x1e);
    Func_0808a180(0xe, 0);
    Func_0808a210(0x1180000, -1, 0x1400000, 1);
    Func_0808a218();
    Func_0808a010(0x14);
    Func_0808a110(0xa, 3);
    Func_0808a010(0x1e);
    Func_0808a180(0xa, 0);
    Func_0808a010(0xa);
    Func_0808a110(0xa, 4);
    Func_0808a010(0x14);
    Func_0808a180(0xa, 0);
    Func_0808a010(0x14);
    Func_0808a1e8(0xe, 0x105, 0x3c);
    Func_0808a180(0xe, 0);
    Func_0808a010(0xa);
    Func_0808a110(3, 4);
    Func_0808a010(0x14);
    Func_0808a180(3, 0);
    Func_0808a010(0xa);
    Func_0808a138(2, 2);
    Func_0808a010(0x14);
    Func_0808a180(2, 0);
    Func_0808a010(0xa);
    Func_0808a138(1, 2);
    Func_0808a010(0x14);
    Func_0808a178(1, 0);

    if (Func_0808a070(0, 0) != 0) {
        Func_0808a010(0x1e);
        Func_0808a1b8(0xa, 0x8000, 0);
        Func_0808a010(0x1e);
        Func_0808a110(0xa, 4);
        Func_0808a010(0x14);
        DIALOGUE_COUNTER++;
        Func_0808a180(0xa, 0);
    } else {
        Func_0808a010(0x1e);
        Func_0808a1b8(0xa, 0x8000, 0);
        Func_0808a010(0x1e);
        Func_0808a110(0xa, 3);
        Func_0808a010(0x1e);
        Func_0808a180(0xa, 0);
        DIALOGUE_COUNTER++;
    }

    Func_0808a010(0xa);
    Func_0808a1e8(0xe, 0x101, 0x3c);
    Func_0808a090(0xe, 0x10000, 0x8000);
    Func_0808a580(0xe, 0, 0x10);
    Func_0808a010(0x14);
    Func_0808a180(0xe, 0);
    Func_0808a010(0xa);
    Func_0808a1b8(0xa, 0xc000, 0);
    Func_0808a010(0x23);
    Func_0808a180(0xa, 0);
    Func_0808a010(0xa);
    Func_0808a138(0xe, 2);
    Func_0808a010(0x14);
    Func_0808a180(0xe, 0);
    Func_0808a010(0x14);
    Func_0808a1e8(3, 0x102, 0x28);
    Func_0808a180(3, 0);
    Func_0808a010(0xa);
    Func_0808a138(2, 2);
    Func_0808a010(0x14);
    Func_0808a180(2, 0);
    Func_0808a010(0xa);
    Func_0808a110(1, 3);
    Func_0808a010(0x1e);
    Func_0808a180(1, 0);
    Func_0808a010(0x14);
    Func_0808a1e8(0xe, 0x100, 0x28);
    Func_0808a178(0xe, 0);

    if (Func_0808a070(0, 0) != 0) {
        Func_0808a010(0x1e);
        Func_0808a110(0xe, 4);
        Func_0808a010(0x14);
        DIALOGUE_COUNTER++;
        Func_0808a180(0xe, 0);
    } else {
        Func_0808a010(0x1e);
        Func_0808a110(0xe, 4);
        Func_0808a010(0x14);
        Func_0808a180(0xe, 0);
        DIALOGUE_COUNTER++;
    }

    Func_0808a010(0xa);
    Func_0808a1e8(0xa, 0x102, 0x32);
    Func_0808a180(0xa, 0);
    Func_0808a010(0xa);
    Func_0808a138(0xd, 2);
    Func_0808a010(0x14);
    Func_0808a090(0xd, 0x14ccc, 0xa666);
    Func_0808a580(0xd, 0, 0x10);
    Func_0808a010(0x14);
    Func_0808a180(0xd, 0);
    Func_0808a010(0xa);
    Func_0808a090(0xc, 0x14ccc, 0xa666);
    Func_0808a580(0xc, 0, 0x10);
    Func_0808a010(0x14);
    Func_0808a1e8(0xc, 0x107, 0x32);
    Func_0808a180(0xc, 0);
    Func_0808a010(0xa);
    Func_0808a110(0xe, 3);
    Func_0808a010(0x1e);
    Func_0808a010(0xa);
    Func_0808a1e8(0xa, 0x102, 0x3c);
    Func_0808a1b8(0xa, 0x8000, 0);
    Func_0808a010(0x19);
    Func_0808a180(0xa, 0);
    Func_0808a010(0xa);
    Func_0808a138(0xa, 2);
    Func_0808a010(0x14);
    Func_0808a178(0xa, 0);

    if (Func_0808a070(0, 0) != 0) {
        Func_0808a010(0x1e);
        Func_0808a110(0xa, 4);
        Func_0808a010(0x14);
        DIALOGUE_COUNTER++;
        Func_0808a180(0xa, 0);
    } else {
        Func_0808a010(0x1e);
        Func_0808a110(0xa, 3);
        Func_0808a010(0x1e);
        Func_0808a180(0xa, 0);
        DIALOGUE_COUNTER++;
    }

    Func_0808a010(0xa);
    Func_0808a1b8(0xa, 0xc000, 0);
    Func_0808a010(0x23);
    Func_0808a110(0xe, 3);
    Func_0808a010(0x1e);
    Func_0808a1b8(0xe, 0xb000, 0);
    Func_0808a010(0x28);
    Func_0808a180(0xe, 0);
    Func_0808a010(0xa);
    Func_0808a150(0xc, 0xd, 0x32);
    Func_0808a100(0xc, 3);
    Func_0808a110(0xd, 3);
    Func_0808a010(0x1e);
    Func_0808a1b8(0xc, 0x4000, 0);
    Func_0808a1b8(0xd, 0x4000, 0);
    Func_0808a010(0x14);
    Func_0808a100(0xc, 3);
    Func_0808a110(0xd, 3);
    Func_0808a010(0x1e);
    Func_0808a090(0xc, 0x10000, 0x8000);
    Func_0808a090(0xd, 0x10000, 0x8000);
    Func_0808a578(0xc, 0x20, 0);
    Func_0808a580(0xd, 0x20, 0);
    Func_0808a578(0xc, 0, 0x10);
    Func_0808a580(0xd, 0x10, 0);
    Func_0808a0c8(0xd, 0x158, 0x138);
    Func_0808a0d0(0xc, 0x158, 0x150);
    Func_0808a100(0xd, 1);
    Func_0808a1b8(0xc, 0x8000, 0);
    Func_0808a1b8(0xd, 0x8000, 0);
    Func_0808a010(0xa);
    Func_0808a1b8(0xe, 0x4000, 0);
    Func_0808a010(0x14);
    Func_0808a180(0xe, 0);
    Func_0808a010(0xa);
    Func_0808a090(0xe, 0x10000, 0x8000);
    Func_0808a0d0(0xe, 0x148, 0x138);
    Func_0808a1b8(0xe, 0x8000, 0);
    Func_0808a010(0x1e);
    Func_0808a090(0xb, 0x10000, 0x8000);
    Func_0808a0d0(0xb, 0x148, 0x148);
    Func_0808a1b8(0xb, 0x8000, 0);
    Func_0808a010(0x14);
    Func_0808a090(1, 0x13333, 0x9999);
    Func_0808a090(2, 0x13333, 0x9999);
    Func_0808a090(3, 0x13333, 0x9999);
    Func_0808a100(1, 2);

    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0b8(1, POS_X(record), POS_Z(record));
    }
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);
    Func_0808a100(2, 2);

    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0b8(2, POS_X(record), POS_Z(record));
    }
    Func_0808a0e8(2);
    Func_0808a0f0(2, 0, 0);
    Func_0808a100(3, 2);

    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0b8(3, POS_X(record), POS_Z(record));
    }
    Func_0808a0e8(3);
    Func_0808a0f0(3, 0, 0);
    Func_0808a010(0xa);
    Func_0808a020();
}
