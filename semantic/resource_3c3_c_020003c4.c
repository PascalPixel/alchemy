#include "types.h"

/*
 * Resource 3c3, the overlay's main cutscene at 0x020003c4
 * (876 bytes, 83 call sites).
 *
 * Complete owner: `push {r5, lr} / sub sp, #8` at 0x020003c4 and
 * `add sp, #8 / pop {r5} / pop {r0} / bx r0` at 0x0200070e.  The popped branch
 * register IS r0, so it holds the return address and the owner is `void`.
 * A two-byte zero alignment word at 0x02000716 is followed by the six-word
 * literal pool 0x02000718-0x0200072f:
 *   0x00019999, 0x0000cccc (a 16.16 scale pair, 1.6 / 0.8 against 0x10000),
 *   0x000001af (a row), 0x00002654 (a dialogue line id), 0x03001ebc (the
 *   workspace pointer cell) and 0x0000089f (a story flag).
 * The pool is past the final `bx`, and every word in it is loaded by a
 * `ldr rN,[pc,#imm]` in the body, so the range is data by control-flow walk.
 *
 * Shift-built constants, spelled as their values below: 0xdb << 1 = 438,
 * 0xc0 << 8 = 0xc000, 0xd0 << 8 = 0xd000, 0x80 << 1 = 256, 0xd2 << 1 = 420,
 * 0xe0 << 8 = 0xe000, 0xd0 << 1 = 416, 0xb0 << 8 = 0xb000, 0x81 << 1 = 258,
 * 0xa0 << 7 = 0x5000, 0x9b << 4 = 0x9b0.
 *
 * The `else` of the 0x9b0 story-flag test is the documented SKIP-BEAT COUNTER
 * — `movs r3,#236 / lsls r3,#1` (byte offset 472) off the 0x03001ebc workspace
 * pointer, incrementing a u16 — not an empty arm.  Sibling 0x02000730 carries
 * the same counter with a `+= 2` bump.  `Data_03001ebc` is a pointer CELL, so
 * `ldr r3,[pc] / ldr r2,[r3]` is one dereference, not two.
 *
 * The two guarded Func_0808a080(0) blocks read the actor record two different
 * ways, and both spellings are deliberate: the first takes the 16.16 words at
 * +8 and +16 (`ldr`), the second the s16 integer views of the same coordinates
 * at +10 and +18 (`movs r3,#N / ldrsh`).  In the second, the offset register r3
 * is the load offset, NOT a trailing argument to the following Func_0808a0b8 —
 * that is the documented phantom-argument trap.
 *
 * The two closing Func_080091c0 requests share r5 = 7 as their first stack
 * argument, set once at 0x020006e2 and reused; the second stack argument
 * differs (27 then 26) and is spelled per call.
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts`; all 83 sites are
 * import veneers.  Per-target multiset, reproduced exactly by the C below:
 *   Func_0808a1b8 x17, Func_0808a188 x16, Func_0808a138 x9,
 *   Func_0808a010 x6,  Func_0808a0d0 x6,  Func_0808a130 x4,
 *   Func_0808a1e8 x4,  Func_0808a1f0 x3,  Func_080091c0 x2,
 *   Func_0808a080 x2,  Func_0808a090 x2,  Func_0808a0f0 x2,
 *   and one each of Func_080000c0, Func_080770c0, Func_080770c8,
 *   Func_0808a018, Func_0808a020, Func_0808a0b8, Func_0808a0e8,
 *   Func_0808a100, Func_0808a170, Func_0808a190.
 * The inventory row advertises calls=75 against 83 real sites — the field is a
 * FLOOR, as HANDOVER records, and is not used as an equality test here.
 */

/* Old-style declarations: overlay import arities vary per call site. */
void Func_080000c0();   /* wait n frames */
void Func_080091c0();   /* six-argument presentation request, last two on the stack */
s32 Func_080770c0();    /* test a story flag (used in a condition) */
void Func_080770c8();   /* set a story flag */
void Func_0808a010();   /* wait n frames (scripted) */
void Func_0808a018();   /* open a scripted scene */
void Func_0808a020();   /* close a scripted scene */
u8 *Func_0808a080();    /* scene entity record by selector */
void Func_0808a090();   /* set a slot's x/y scale in 16.16 */
void Func_0808a0b8();   /* place a slot at an integer x/z */
void Func_0808a0d0();   /* place an entity */
void Func_0808a0e8();   /* commit a slot's placement */
void Func_0808a0f0();   /* place a slot at a 16.16 position */
void Func_0808a100();   /* set a slot's presentation mode */
void Func_0808a130();   /* select a slot's pose */
void Func_0808a138();   /* select a slot's walk mode */
void Func_0808a170();   /* show a dialogue line by id */
void Func_0808a188();   /* move the slot along a heading */
void Func_0808a190();   /* per-slot state reset */
void Func_0808a1b8();   /* face the slot along a heading */
void Func_0808a1e8();   /* play animation id for n frames */
void Func_0808a1f0();   /* timed per-slot request */

extern u8 *Data_03001ebc;

void Func_020003c4(void)
{
    u8 *workspace;
    u8 *actor;
    int mode;

    Func_0808a018();

    Func_0808a090(0, 0x19999, 0xcccc);
    Func_0808a0d0(0, 120, 438);
    Func_0808a1b8(0, 0xc000, 0);

    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0f0(11, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    }
    Func_080000c0(1);

    Func_0808a090(11, 0x19999, 0xcccc);
    Func_0808a0d0(11, 108, 0x1af);
    Func_0808a1b8(11, 0xd000, 10);
    Func_0808a1e8(11, 256, 20);
    Func_0808a1b8(11, 0xd000, 20);
    Func_0808a1b8(11, 0, 40);
    Func_0808a1b8(11, 0xd000, 40);
    Func_0808a1b8(11, 0, 20);
    Func_0808a130(11, 2);

    Func_0808a170(0x2654);

    Func_0808a188(11, 0, 40);
    Func_0808a1e8(8, 256, 0);
    Func_0808a138(8, 2);
    Func_0808a188(8, 0, 10);

    Func_0808a0d0(11, 132, 420);
    Func_0808a1b8(11, 0xd000, 0);
    Func_0808a1b8(0, 0xe000, 0);
    Func_0808a0d0(11, 138, 416);
    Func_0808a1b8(11, 0xb000, 10);
    Func_0808a130(11, 2);
    Func_0808a188(11, 0, 40);
    Func_0808a138(8, 2);
    Func_0808a188(8, 0, 40);

    Func_0808a1e8(9, 256, 20);
    Func_0808a138(9, 2);
    Func_0808a188(9, 0, 20);

    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a0d0(11, 144, 420);
    Func_0808a010(20);

    Func_0808a138(9, 2);
    Func_0808a188(9, 0, 20);
    Func_0808a1f0(9, 258);
    Func_0808a130(9, 3);
    Func_0808a188(9, 0, 20);

    Func_0808a1b8(11, 0x5000, 20);
    Func_0808a190(11, 0);

    if (Func_080770c0(0x9b0) != 0) {
        Func_0808a1b8(11, 0xd000, 40);
        Func_0808a1f0(11, 258);
        Func_0808a010(40);
        Func_0808a188(11, 0, 10);
    } else {
        /* Skip-beat counter: the scene variant that plays no beat here still
         * advances the workspace beat cursor. */
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) += 1;
    }

    Func_0808a1b8(11, 0x5000, 10);
    Func_0808a188(11, 0, 40);
    Func_0808a1e8(11, 256, 40);
    Func_0808a1b8(11, 0xb000, 10);
    Func_0808a188(11, 0, 10);
    Func_0808a0d0(11, 138, 416);
    Func_0808a1b8(11, 0xb000, 20);
    Func_0808a130(8, 2);
    Func_0808a188(8, 0, 10);
    Func_0808a1f0(11, 258);
    Func_0808a138(11, 1);
    Func_0808a010(20);
    Func_0808a188(11, 0, 20);

    Func_0808a138(9, 2);
    Func_0808a010(20);
    Func_0808a1b8(0, 0xe000, 10);
    Func_0808a138(9, 1);
    Func_0808a010(20);
    Func_0808a188(9, 0, 10);

    Func_0808a138(8, 2);
    Func_0808a188(8, 0, 20);

    Func_0808a138(11, 1);
    Func_0808a010(20);
    Func_0808a188(11, 0, 20);
    Func_0808a1b8(11, 0x5000, 10);
    Func_0808a188(11, 0, 10);
    Func_0808a100(11, 2);

    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0b8(11, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    Func_0808a0e8(11);
    Func_0808a0f0(11, 0, 0);

    mode = 7;
    Func_080091c0(6, 27, 1, 1, mode, 27);
    Func_080091c0(9, 26, 2, 1, mode, 26);

    Func_080770c8(0x89f);

    Func_0808a020();
}
