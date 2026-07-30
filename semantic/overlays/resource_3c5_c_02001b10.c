typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3c5 owner at 0x02001b10, 2396 bytes: 0x02001b10-0x0200246b.  The
 * overlay's second large cutscene script, the sibling of the one at
 * 0x02001238: the same straight-line actor/camera command sequence, the same
 * two dialogue branches and the same three-actor cleanup tail.
 *
 * FRAME AND SIGNATURE.  `push {lr}` only -- no locals, no stack arguments, no
 * saved high registers.  The single epilogue at 0x0200245a is
 * `pop {r0} ; bx r0`, so r0 holds the popped return address and the owner is
 * **void**.  It takes no arguments (r0 is written before any read).
 *
 * POOL MAP, derived from the owner's own control flow -- what the code branches
 * over -- and NOT from any referenced-words listing.  Three data regions, none
 * decoded as instructions, with register state carried straight across them:
 *   0x02001f0c-0x02001f1b   4 words, hopped by the unconditional `b.n` at
 *                           0x02001f0a (0x989, 0x272f, 0x101, 0x301);
 *   0x020021f4-0x020021ff   3 words, hopped by the `b.n` at 0x020021f2, which
 *                           ends the fall-through arm of the second dialogue
 *                           branch; the `bne` that opens that branch targets
 *                           0x02002200, the halfword immediately after the
 *                           pool (0x103, 0x03001ebc, 0x101);
 *   0x02002460-0x0200246b   3 trailing words past the return, filling the row
 *                           to its 0x0200246c end (0x03001ebc, 0x13333,
 *                           0x9999).
 * The first of these is the plain case the guard exists for: an unconditional
 * branch over a pool in the middle of the script, with nothing else to mark it.
 *
 * CALL ACCOUNTING.  `bun tools/overlay_call_targets.ts resource_3c5 1b10`
 * reports sites=263 / distinct_targets=30 (29 import veneers plus one real
 * prologue, this overlay's own 0x02002548).  The inventory's `calls=249` is
 * the usual lower bound and is not used as a completeness proof.  The multiset
 * comparison is: this file contains 263 `Func_...(` occurrences with per-name
 * multiplicities
 *   0808a010 x88  0808a1b8 x49  0808a180 x27  0808a1e8 x18  0808a110 x13
 *   0808a138 x12  0808a100 x7   0808a090 x5   0808a0e8 x4   0808a570 x3
 *   0808a210 x3   0808a580 x3   0808a080 x3   0808a0b8 x3   0808a0f0 x3
 *   0808a018 x2   0808a0d0 x2   0808a218 x2   0808a128 x2   0808a178 x2
 *   0808a070 x2   0808a150 x2   080770c8 x1   0808a460 x1   0808a170 x1
 *   0808a208 x1   0808a200 x1   080770d0 x1   0808a020 x1   02002548 x1
 * which is exactly the tool's per-target table, entry for entry.
 *
 * THE SCRIPT'S IDIOM is identical to the 0x02001238 owner's: Func_0808a010(n)
 * is the frame wait between beats (88 of the 263 sites), Func_0808a180(slot, 0)
 * waits for a slot's queued action, Func_0808a1b8(slot, heading, 0) faces a
 * slot on the overlay's 16-bit angle scale, Func_0808a1e8(slot, id, frames)
 * plays a numbered animation, and Func_0808a070(0, 0) is the dialogue call
 * whose result selects the branch.  Func_02002548 is the overlay's own routine,
 * called once with no argument register set.
 *
 * THE DIALOGUE COUNTER.  Both branches, on both arms, bump the halfword at byte
 * 472 of the workspace, the same `ldr =0x03001ebc / ldr / movs #236 / lsls #1 /
 * ldrh / adds #1 / strh` sequence the sibling script uses.  The workspace
 * pointer variable is the Data_03001ebc that the byte-exact
 * assets/code/resource_3c5_c_0200006c.c reads, and the overlay image is
 * writable EWRAM used as save state, so this is not const data.
 *
 * Uncertainties, recorded rather than guessed:
 *  - The three tail probes at 0x0200237c/0x020023ac/0x020023dc read the
 *    record's s16 at +10 and +18: the integer parts of the 16.16 words at +8
 *    and +16, the halfword coordinate view the byte-exact resource_3c8:14f4
 *    already models twice.
 *  - At each of those sites r3 still holds 18 at the `bl`, because
 *    `movs r3,#18` supplied the load's offset.  That is the offset register,
 *    not a fourth argument; Func_0808a0b8 is spelled with three.  Recorded
 *    because an argument-window simulator reports `(slot, x, z, 18)`.
 *  - The progress ids 0x989 (Func_080770c8) and 0x301 (Func_080770d0), and the
 *    0x272f argument to Func_0808a170, are pooled constants passed straight
 *    through; their meaning is not established here.
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
void Func_02002548();
void Func_080770c8();
void Func_080770d0();
void Func_0808a010();           /* wait n frames */
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();            /* dialogue; result selects the branch */
u8 *Func_0808a080();            /* record by slot index, or 0 */
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a138();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();           /* wait for the slot's action to finish */
void Func_0808a1b8();           /* face slot along heading */
void Func_0808a1e8();           /* play animation id for n frames */
void Func_0808a200();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a460();
void Func_0808a570();
void Func_0808a580();

void Func_02001b10(void)
{
    u8 *record;

    Func_080770c8(0x989);
    Func_0808a018();
    Func_0808a460();
    Func_0808a170(0x272f);
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a0d0(0, 0x128, 0x138);
    Func_0808a1b8(0, 0, 0);
    Func_0808a010(0xa);
    Func_0808a570(1, 0, 0x10, 0);
    Func_0808a570(2, 0xfffffff0, 0xfffffff8, 0);
    Func_0808a570(3, 0xfffffff0, 0x18, 0);
    Func_0808a0e8(3);
    Func_0808a010(0x14);
    Func_0808a208(0x30000, 0x6000);
    Func_0808a210(0x1180000, -1, 0x1480000, 1);
    Func_0808a218();
    Func_0808a010(0xa);
    Func_0808a010(0xa);
    Func_0808a1b8(0xa, 0xb000, 0);
    Func_0808a010(0xa);
    Func_0808a1e8(0xa, 0x100, 0x28);
    Func_0808a180(0xa, 0);
    Func_0808a128(0xa, 4, 0xd);
    Func_0808a128(0xa, 4, 0x1e);
    Func_0808a010(0xa);
    Func_0808a1e8(0xb, 0x100, 0);
    Func_0808a1e8(0xc, 0x100, 0);
    Func_0808a1e8(0xd, 0x100, 0);
    Func_0808a1e8(0xe, 0x100, 0x28);
    Func_0808a010(0xa);
    Func_0808a1b8(0xe, 0xb000, 0);
    Func_0808a1b8(0xb, 0xb000, 0);
    Func_0808a1b8(0xc, 0xb000, 0);
    Func_0808a1b8(0xd, 0xb000, 0);
    Func_0808a010(0x1e);
    Func_0808a138(0xe, 2);
    Func_0808a010(0x14);
    Func_0808a180(0xe, 0);
    Func_0808a010(0x14);
    Func_0808a1e8(0xd, 0x102, 0x28);
    Func_0808a180(0xd, 0);
    Func_0808a010(0x14);
    Func_0808a1e8(0xc, 0x101, 0x32);
    Func_0808a1b8(0xc, 0x8000, 0);
    Func_0808a010(0x19);
    Func_0808a180(0xc, 0);
    Func_0808a010(0xa);
    Func_0808a1b8(0xe, 0x8000, 0);
    Func_0808a1b8(0xb, 0x8000, 0);
    Func_0808a1b8(0xd, 0x8000, 0);
    Func_0808a010(0x1e);
    Func_0808a010(0xa);
    Func_0808a138(3, 2);
    Func_0808a010(0x14);
    Func_0808a180(3, 0);
    Func_0808a010(0xa);
    Func_0808a138(2, 2);
    Func_0808a010(0x14);
    Func_0808a180(2, 0);
    Func_0808a010(0xa);
    Func_0808a1e8(1, 0x102, 0x28);
    Func_0808a180(1, 0);
    Func_0808a010(0x14);
    Func_0808a110(0xa, 4);
    Func_0808a010(0x14);
    Func_0808a180(0xa, 0);
    Func_0808a010(0xa);
    Func_0808a138(0xe, 2);
    Func_0808a010(0x19);
    Func_0808a180(0xe, 0);
    Func_0808a010(0xa);
    Func_0808a138(0xa, 2);
    Func_0808a010(0x14);
    Func_0808a090(0xa, 0x10000, 0x8000);
    Func_0808a580(0xa, 0, 0xffffffd8);
    Func_0808a1b8(0xa, 0, 0);
    Func_0808a010(0x14);
    Func_0808a180(0xa, 0);
    Func_0808a010(0xa);
    Func_0808a110(0xe, 3);
    Func_0808a010(0x1e);
    Func_0808a010(0xa);
    Func_0808a1b8(0xd, 0x4000, 0);
    Func_0808a1b8(0xc, 0xc000, 0);
    Func_0808a010(0x1e);
    Func_0808a100(0xc, 3);
    Func_0808a110(0xd, 3);
    Func_0808a010(0x1e);
    Func_0808a1b8(0xd, 0x8000, 0);
    Func_0808a1b8(0xc, 0x8000, 0);
    Func_0808a010(0x14);
    Func_0808a110(0xa, 3);
    Func_0808a010(0x1e);
    Func_0808a200(0xa, 1);
    Func_0808a580(0xa, 0, 0xffffffe0);
    Func_02002548();
    Func_080770d0(0x301);
    Func_0808a210(-1, -1, -1, 0);
    Func_0808a018();
    Func_0808a1e8(0xa, 0x102, 0x28);
    Func_0808a180(0xa, 0);
    Func_0808a010(0xa);
    Func_0808a1b8(0xe, 0xb000, 0);
    Func_0808a1b8(0xb, 0xb000, 0);
    Func_0808a1b8(0xc, 0xb000, 0);
    Func_0808a1b8(0xd, 0xb000, 0);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a010(0x1e);
    Func_0808a1e8(0xb, 0x100, 0);
    Func_0808a1e8(0xc, 0x100, 0);
    Func_0808a1e8(0xd, 0x100, 0);
    Func_0808a1e8(0xe, 0x100, 0x46);
    Func_0808a210(0x1180000, -1, 0x1380000, 1);
    Func_0808a218();
    Func_0808a010(0xa);
    Func_0808a1e8(0xc, 0x102, 0x28);
    Func_0808a180(0xc, 0);
    Func_0808a010(0xa);
    Func_0808a110(0xd, 4);
    Func_0808a010(0x14);
    Func_0808a180(0xd, 0);
    Func_0808a010(0xa);
    Func_0808a138(2, 2);
    Func_0808a010(0x1e);
    Func_0808a1b8(2, 0x2000, 0);
    Func_0808a010(0x14);
    Func_0808a180(2, 0);
    Func_0808a010(0xa);
    Func_0808a1b8(0xa, 0x4000, 0);
    Func_0808a010(0x1e);
    Func_0808a1b8(3, 0xe000, 0);
    Func_0808a010(0x1e);
    Func_0808a1e8(3, 0x102, 0x28);
    Func_0808a1b8(0, 0x3000, 0);
    Func_0808a010(0x14);
    Func_0808a180(3, 0);
    Func_0808a010(0xa);
    Func_0808a110(1, 4);
    Func_0808a010(0x14);
    Func_0808a180(1, 0);
    Func_0808a010(0x14);
    Func_0808a1e8(0xe, 0x103, 0x32);
    Func_0808a1b8(0xe, 0x8000, 0);
    Func_0808a010(0x14);
    Func_0808a178(0xe, 0);
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(0xb, 0x8000, 0);
    Func_0808a1b8(0xc, 0x8000, 0);
    Func_0808a1b8(0xd, 0x8000, 0);
    Func_0808a1b8(2, 0x1000, 0);
    Func_0808a1b8(3, 0xe000, 0);

    if (Func_0808a070(0, 0) != 0) {
        Func_0808a010(0x1e);
        Func_0808a138(0xe, 2);
        Func_0808a010(0x14);
        DIALOGUE_COUNTER++;
        Func_0808a180(0xe, 0);
    } else {
        Func_0808a010(0x1e);
        Func_0808a138(0xe, 2);
        Func_0808a010(0x14);
        Func_0808a180(0xe, 0);
        DIALOGUE_COUNTER++;
    }

    Func_0808a010(0xa);
    Func_0808a138(0xa, 2);
    Func_0808a010(0x14);
    Func_0808a580(0xa, 0, 0x10);
    Func_0808a1b8(0xa, 0x2000, 0);
    Func_0808a010(0x14);
    Func_0808a180(0xa, 0);
    Func_0808a010(0xa);
    Func_0808a1b8(0xe, 0xa000, 0);
    Func_0808a010(0x14);
    Func_0808a110(0xe, 4);
    Func_0808a010(0x14);
    Func_0808a180(0xe, 0);
    Func_0808a010(0xa);
    Func_0808a110(0xa, 3);
    Func_0808a010(0x1e);
    Func_0808a180(0xa, 0);
    Func_0808a010(0xa);
    Func_0808a138(0xe, 2);
    Func_0808a010(0x14);
    Func_0808a178(0xe, 0);
    Func_0808a010(0x28);
    Func_0808a1b8(0xa, 0x5000, 0);
    Func_0808a010(0x14);
    Func_0808a1e8(0xa, 0x101, 0x3c);

    if (Func_0808a070(0, 0) != 0) {
        Func_0808a010(0x1e);
        Func_0808a1b8(0xa, 0x2000, 0);
        Func_0808a010(0x14);
        Func_0808a110(0xa, 4);
        Func_0808a010(0x14);
        DIALOGUE_COUNTER++;
        Func_0808a180(0xa, 0);
    } else {
        Func_0808a010(0x1e);
        Func_0808a1b8(0xa, 0x2000, 0);
        Func_0808a010(0x14);
        Func_0808a110(0xa, 3);
        Func_0808a010(0x1e);
        Func_0808a180(0xa, 0);
        DIALOGUE_COUNTER++;
    }

    Func_0808a010(0xa);
    Func_0808a1e8(0xe, 0x102, 0x32);
    Func_0808a180(0xe, 0);
    Func_0808a010(0xf);
    Func_0808a110(0xa, 3);
    Func_0808a010(0x1e);
    Func_0808a010(0x14);
    Func_0808a138(0xe, 2);
    Func_0808a010(0x28);
    Func_0808a1b8(0xe, 0x8000, 0);
    Func_0808a010(0x14);
    Func_0808a180(0xe, 0);
    Func_0808a010(0xa);
    Func_0808a110(0xe, 3);
    Func_0808a010(0x1e);
    Func_0808a180(0xe, 0);
    Func_0808a010(0xa);
    Func_0808a1b8(0xa, 0x5000, 0);
    Func_0808a010(0x19);
    Func_0808a138(0xa, 2);
    Func_0808a010(0x14);
    Func_0808a0d0(0xa, 0x138, 0x138);
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(0xa, 0x8000, 0);
    Func_0808a010(0x19);
    Func_0808a180(0xa, 0);
    Func_0808a010(0xa);
    Func_0808a150(0, 1, 0);
    Func_0808a150(3, 2, 0);
    Func_0808a010(0x1e);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(3, 3);
    Func_0808a110(2, 3);
    Func_0808a010(0x1e);
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
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(0xa, 0xb000, 0);
    Func_0808a1b8(0xe, 0xb000, 0);
    Func_0808a1b8(0xb, 0xb000, 0);
    Func_0808a1b8(0xc, 0xb000, 0);
    Func_0808a1b8(0xd, 0xb000, 0);
    Func_0808a010(0x1e);
    Func_0808a020();
}
