#include "types.h"

/*
 * resource_399 cutscene at 0x02000f90, 1,468 bytes.
 *
 * Complete owner: `push {r5, lr}` at 0x02000f90 and the single interworking
 * return `pop {r5} / pop {r0} / bx r0` at 0x02001526.  The popped register is
 * r0, so it holds the return address — the owner is `void`.  The span ends at
 * 0x0200154c.
 *
 * POOL MAP, from a control-flow walk rather than the "pool words referenced"
 * listing.  Two pools:
 *   0x0200124c-0x02001283  branched over by the `b.n 0x200128a` at
 *                          0x0200124a.  NOTE that 0x02001284 is NOT part of
 *                          it: it is the body of the first spin-wait, reached
 *                          by the `beq.n 0x2001284` at 0x02001298, so the pool
 *                          ends six bytes before the obvious place.
 *   0x0200152c-0x0200154b  past the return.
 *
 * Call targets resolved with `bun tools/lib/overlay_call_targets.ts resource_399
 * 0f90 --annotate` (an overlay `bl` stores `target - 2`).  142 sites over 35
 * distinct targets; the inventory's calls=130 predates the corrected decoding
 * and is a lower bound.  The per-target multiset the assembly gives, which the
 * C below reproduces exactly:
 *   Func_0808a010 x15, Func_0808a188 x12, Func_0808a090 x12,
 *   Func_0808a080 x12, Func_0808a1b8 x11, Func_0808a110 x8,
 *   Func_0808a098 x6, Func_080000c0 x6, Func_0808a1e8 x5, Func_0808a138 x5,
 *   Func_0808a100 x4, Func_080f9010 x3, Func_0808a210 x3, Func_0808a1f0 x3,
 *   Func_0808a180 x3, Func_0808a168 x3, Func_0808a0f0 x3, Func_0808a0d0 x3,
 *   Func_0808a0b8 x3, Func_0808a0a8 x3, Func_0808a348 x2, Func_0808a330 x2,
 *   Func_0808a0a0 x2, Func_08009178 x2, and one each of Func_020016c8,
 *   Func_0808a360, Func_0808a338, Func_0808a218, Func_0808a208,
 *   Func_0808a170, Func_0808a0c8, Func_0808a0b0, Func_0808a018,
 *   Func_0808a020, Func_080770c8.
 *
 * TWO SPIN-WAITS, at 0x02001284 and 0x020013e0.  Each is entered by a forward
 * `b.n` to its test, polls the s16 at +100 of channel 3's record and calls
 * Func_080000c0(1) once per iteration until it becomes non-zero.  Written as
 * as explicit bottom-tested loops so the source walk follows the machine's
 * wait-then-test layout while keeping each Func_0808a080/Func_080000c0 site
 * at one — the shared-call-site trap in its loop form.
 * The flag they wait on is the halfword this owner itself clears just before
 * each wait and hands to Func_0808a098 with a script, so the script is what
 * sets it: the pair reads as "play this animation and block until it ends".
 *
 * BOTH DISPLACEMENT/VALUE TRAPS APPEAR HERE, and this owner is the cleanest
 * witness for each:
 *  - subtractive (0x02000fda): `movs r3,#224 ; lsls #1` builds 448, used as
 *    the displacement; `subs r3,#192` makes the SAME register the stored value
 *    32; then `adds r3,#200` makes it 232, the displacement of the NEXT store,
 *    whose value is 40.  So it is workspace+448 = 32 and workspace+232 = 40,
 *    not any arithmetic on 448.
 *  - additive (0x02001506): 448 as displacement, `adds r3,#73` gives the
 *    stored value 521 (= 0x209, the same scenario word the owner at
 *    0x02000b70 writes), then `subs r3,#65` gives 456 as the next
 *    displacement, storing 24.
 *
 * LINK BASE.  Every pointer-shaped pool word is even and in the
 * 0x0200axxx/0x0200bxxx band, so under the proven 0x02008000 base they are
 * in-image data at `value - 0x8000`: scripts at 0x274c, 0x25ec, 0x2670,
 * 0x26e0, 0x2760 and camera targets at 0x31c0/0x31d6 — the same two targets
 * the owner at 0x02000384 passes to Func_08009178, which cross-checks both
 * files.
 *
 * Constants are built by shifts throughout, as this compiler spells 16.16
 * values and 16-bit angles: `movs #128 ; lsls #8` = 0x8000 is a half turn,
 * `lsls #7` = 0x4000 a quarter, `movs #192 ; lsls #6` = 0x3000 three
 * sixteenths.  `movs #129 ; lsls #1` = 258 is the pose id shared with the
 * owners at 0x020002b8 and 0x02000384.
 *
 * Uncertainties: the record fields are reproduced as written.  +8 and +16 are
 * the 16.16 x and z of channel 0's record, copied onto channels 1 and 2;
 * bit 0 of the byte at +90 is cleared on all three channels and set again
 * after the Func_0808a0b8 placement, which reads as a visibility or
 * follow-suppression flag but is not confirmed anywhere in this overlay.  The
 * word 0x00207e9f handed to Func_0808a330 is a packed constant of unknown
 * layout, not an address (it is not in the image's address range).
 */

extern u8 **Data_03001ebc;      /* pointer cell holding the scene workspace */
extern u8 Data_0200a5ec[];      /* in-image animation script, file offset 0x25ec */
extern u8 Data_0200a670[];      /* in-image animation script, file offset 0x2670 */
extern u8 Data_0200a6e0[];      /* in-image animation script, file offset 0x26e0 */
extern u8 Data_0200a74c[];      /* in-image data block, file offset 0x274c */
extern u8 Data_0200a760[];      /* in-image animation script, file offset 0x2760 */
extern u8 Data_0200b1c0[];      /* in-image camera target, file offset 0x31c0 */
extern u8 Data_0200b1d6[];      /* in-image camera target, file offset 0x31d6 */

/* This overlay's own owner, reached by an ordinary `bl`. */
void Func_020016c8();

/* Imports, named by the main-image address in the trailing word of the overlay
 * veneer each call site reaches.  Old-style declarations are mandatory. */
void Func_080000c0();           /* frame advance */
void Func_08009178();           /* camera placement */
void Func_080770c8();           /* story-flag set */
void Func_080f9010();
void Func_0808a010();           /* frame wait */
void Func_0808a018();           /* scripted-section open */
void Func_0808a020();           /* scripted-section close */
void Func_0808a090();           /* 16.16 pair setter */
void Func_0808a098();           /* (selector, script) */
void Func_0808a0a0();
void Func_0808a0a8();
void Func_0808a0b0();           /* (selector, script) */
void Func_0808a0b8();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0f0();           /* (selector, x, z) placement */
void Func_0808a100();
void Func_0808a110();
void Func_0808a138();
void Func_0808a168();
void Func_0808a170();           /* message line */
void Func_0808a180();
void Func_0808a188();
void Func_0808a1b8();           /* orientation over frames */
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a208();
void Func_0808a210();           /* camera target */
void Func_0808a218();
void Func_0808a330();
void Func_0808a338();
void Func_0808a348();
void Func_0808a360();
u8 *Func_0808a080();            /* scene record for a selector, or 0 */

void Func_02000f90(void)
{
    u8 *workspace;
    u8 *record;

    Func_0808a018();
    Func_0808a0f0(3, 182 << 16, 150 << 16);
    Func_0808a210(141 << 16, -1, 221 << 16, 0);
    Func_080000c0(1);
    Func_0808a208(0x4ccc, 0x999);
    Func_0808a210(140 << 16, -1, 164 << 16, 1);

    *(s32 *)(workspace + 232) = 40;
    *(s32 *)(workspace + 448) = 32;
    workspace = *Data_03001ebc;

    Func_0808a360();

    Func_0808a090(0, 0x6666, 0x3333);
    Func_0808a090(1, 0x6666, 0x3333);
    Func_0808a090(2, 0x6666, 0x3333);

    Func_0808a0d0(0, 142, 221);
    Func_0808a1b8(0, 0xd000, 0);

    /* Channels 1 and 2 are placed on channel 0's coordinates.  The record
     * pointer is fetched again for each, and each fetch has its own null
     * guard, so these are two independent call sites. */
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }

    Func_0808a0c8(1, 150, 234);
    Func_0808a0d0(2, 134, 234);
    Func_0808a100(1, 1);

    Func_0808a168(0, 0x10003, Data_0200a74c);
    Func_0808a168(1, 0x10003, Data_0200a74c);
    Func_0808a168(2, 0x10003, Data_0200a74c);
    Func_0808a218();

    Func_0808a098(9, Data_0200a5ec);
    Func_0808a010(40);
    Func_0808a1f0(3, 258);
    Func_0808a010(40);
    Func_0808a138(3, 1);
    Func_0808a170(0x155c);
    Func_0808a188(3, 0, 20);
    Func_0808a098(9, Data_0200a5ec);
    Func_0808a188(9, 0, 20);
    Func_0808a1b8(3, 0x8000, 20);
    Func_0808a1b8(8, 0, 10);
    Func_0808a110(8, 4);
    Func_0808a188(8, 0, 40);
    Func_0808a110(3, 3);
    Func_0808a010(10);
    Func_0808a1b8(3, 0x4000, 0);
    Func_0808a1b8(8, 0x3000, 20);
    Func_0808a188(3, 0, 10);
    Func_0808a098(9, Data_0200a5ec);

    Func_020016c8();

    Func_0808a1e8(3, 0x101, 60);
    Func_0808a188(3, 0, 40);
    Func_0808a188(9, 0, 20);
    Func_0808a1e8(8, 0x105, 60);
    Func_0808a100(9, 7);
    Func_08009178(Data_0200b1c0, 10, 69);
    Func_0808a010(10);
    Func_0808a138(3, 2);
    Func_0808a110(3, 4);
    Func_0808a188(3, 0, 20);
    Func_0808a138(9, 1);
    Func_0808a010(40);
    Func_0808a100(9, 8);
    Func_08009178(Data_0200b1d6, 10, 69);
    Func_0808a010(40);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_0808a1b8(8, 0, 20);
    Func_0808a110(8, 3);
    Func_0808a188(8, 0, 10);
    Func_0808a1e8(3, 0x101, 30);
    Func_0808a1b8(3, 0x8000, 10);
    Func_0808a100(3, 4);
    Func_0808a188(3, 0, 10);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a110(3, 3);
    Func_0808a010(40);
    Func_0808a090(3, 0x10000, 0x8000);

    record = Func_0808a080(3);
    *(u16 *)(record + 100) = 0;
    Func_0808a098(3, Data_0200a670);

waitFirstScript:
    goto checkFirstScript;
        Func_080000c0(1);
checkFirstScript:
    if (*(s16 *)(Func_0808a080(3) + 100) == 0) {
        goto waitFirstScript;
    }

    Func_0808a210(140 << 16, -1, 198 << 16, 1);
    Func_0808a0a0(3);
    Func_0808a1e8(3, 0x101, 80);
    Func_0808a188(3, 0, 40);
    Func_0808a138(3, 1);
    Func_0808a010(10);
    Func_0808a180(3, 0);

    Func_080f9010(131);
    Func_0808a338(0x10000, 0);
    Func_0808a330(0x00207e9f, 0);
    Func_0808a348(10);
    Func_080000c0(1);
    Func_080f9010(220);
    Func_080000c0(40);
    Func_0808a330(0x10000, 0);
    Func_0808a348(60);
    Func_080000c0(60);

    Func_0808a1f0(3, 258);
    Func_0808a010(20);
    Func_0808a1b8(3, 0, 10);
    Func_0808a090(3, 0x20000, 0x10000);
    Func_0808a0d0(3, 202, 198);
    Func_0808a010(40);
    Func_0808a138(3, 2);
    Func_0808a180(3, 0);
    Func_0808a110(3, 4);
    Func_0808a188(3, 0, 20);
    Func_0808a1f0(3, 258);
    Func_0808a010(40);
    Func_0808a188(3, 0, 40);
    Func_0808a1e8(3, 256, 40);
    Func_0808a180(3, 0);

    Func_0808a0a8(0);
    Func_0808a0a8(1);
    Func_0808a0a8(2);

    Func_0808a090(3, 0x30000, 0x18000);

    *(u16 *)(record + 100) = 0;
    record = Func_0808a080(3);
    Func_0808a098(3, Data_0200a6e0);

    goto checkSecondScript;
waitSecondScript:
        Func_080000c0(1);
checkSecondScript:
    if (*(s16 *)(Func_0808a080(3) + 100) == 0) {
        goto waitSecondScript;
    }

    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(2, 0x4000, 10);

    Func_0808a090(0, 0x40000, 0x20000);
    Func_0808a090(1, 0x40000, 0x20000);
    Func_0808a090(2, 0x40000, 0x20000);

    Func_080f9010(152);

    record = Func_0808a080(0);
    record = Func_0808a080(1);
    record[90] &= 0xfe;
    record[90] &= 0xfe;
    record = Func_0808a080(2);
    record[90] &= 0xfe;

    Func_0808a0b8(0, 132, 206);
    Func_0808a0b8(1, 136, 221);
    Func_0808a0b8(2, 122, 238);

    Func_0808a0a0(3);
    Func_0808a010(80);

    record = Func_0808a080(0);
    record[90] |= 1;
    record = Func_0808a080(1);
    record[90] |= 1;
    record = Func_0808a080(2);
    record[90] |= 1;

    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a090(2, 0xcccc, 0x6666);

    Func_0808a098(1, Data_0200a760);
    Func_0808a0b0(2, Data_0200a760);
    Func_0808a010(20);

    workspace = *Data_03001ebc;
    *(s32 *)(workspace + 448) = 521;
    *(s32 *)(workspace + 456) = 24;

    Func_080770c8(0x82b);
    Func_0808a020();
}
