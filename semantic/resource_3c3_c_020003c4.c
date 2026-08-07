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
void Func_02000c66();
void Func_02000c90();
void Func_02000cb4();
void Func_02000d30();
u8 *Func_02000ca6();
void Func_02000cec();
void Func_02000c62();
void Func_02000ccc();
void Func_02000cee();
void Func_02000d6a();
void Func_02000d7e();
void Func_02000d82();
void Func_02000d8c();
void Func_02000d98();
void Func_02000da2();
void Func_02000d88();
void Func_02000daa();
void Func_02000dce();
void Func_02000d96();
void Func_02000dc8();
void Func_02000d74();
void Func_02000df0();
void Func_02000dfc();
void Func_02000e14();
void Func_02000ddc();
void Func_02000e16();
void Func_02000df6();
void Func_02000e28();
void Func_02000e4c();
void Func_02000e46();
void Func_02000e62();
void Func_02000dfe();
void Func_02000dc4();
void Func_02000e44();
void Func_02000e76();
void Func_02000ea0();
void Func_02000e58();
void Func_02000e92();
void Func_02000eae();
s32 Func_02000df6_b();
void Func_02000ece();
void Func_02000ee8();
void Func_02000e2e();
void Func_02000ed8();
void Func_02000f06();
void Func_02000f00();
void Func_02000f24();
void Func_02000f28();
void Func_02000f22();
void Func_02000f4a();
void Func_02000f12();
void Func_02000f4c();
void Func_02000f76();
void Func_02000f36();
void Func_02000ec4();
void Func_02000f6e();
void Func_02000f4e();
void Func_02000edc();
void Func_02000f98();
void Func_02000f68();
void Func_02000ef6();
void Func_02000fa0();
void Func_02000f80();
void Func_02000fb2();
void Func_02000f92();
void Func_02000f20();
void Func_02000fca();
void Func_02000fe6();
void Func_02000fe0();
void Func_02000fa8();
u8 *Func_02000f6e_b();
void Func_02000f90();
void Func_02000fb6();
void Func_02000fc8();
void Func_02000f5c();
void Func_02000f8c();
void Func_02000fb0();
                        /* wait n frames */
                        /* six-argument presentation request, last two on the stack */
                        /* test a story flag (used in a condition) */
                        /* set a story flag */
                        /* wait n frames (scripted) */
                        /* open a scripted scene */
                        /* close a scripted scene */
                        /* scene entity record by selector */
                        /* set a slot's x/y scale in 16.16 */
                        /* place a slot at an integer x/z */
                        /* place an entity */
                        /* commit a slot's placement */
                        /* place a slot at a 16.16 position */
                        /* set a slot's presentation mode */
                        /* select a slot's pose */
                        /* select a slot's walk mode */
                        /* show a dialogue line by id */
                        /* move the slot along a heading */
                        /* per-slot state reset */
                        /* face the slot along a heading */
                        /* play animation id for n frames */
                        /* timed per-slot request */

extern u8 *Data_03001ebc;

void Func_020003c4(void)
{
    u8 *workspace;
    u8 *actor;
    int mode;

    Func_02000c66();

    Func_02000c90(0, 0x19999, 0xcccc);
    Func_02000cb4(0, 120, 438);
    Func_02000d30(0, 0xc000, 0);

    actor = Func_02000ca6(0);
    if (actor != 0) {
        Func_02000cec(11, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    }
    Func_02000c62(1);

    Func_02000ccc(11, 0x19999, 0xcccc);
    Func_02000cee(11, 108, 0x1af);
    Func_02000d6a(11, 0xd000, 10);
    Func_02000d7e(11, 256, 20);
    Func_02000d82(11, 0xd000, 20);
    Func_02000d8c(11, 0, 40);
    Func_02000d98(11, 0xd000, 40);
    Func_02000da2(11, 0, 20);
    Func_02000d6a(11, 2);

    Func_02000d88(0x2654);

    Func_02000daa(11, 0, 40);
    Func_02000dce(8, 256, 0);
    Func_02000d96(8, 2);
    Func_02000dc8(8, 0, 10);

    Func_02000d74(11, 132, 420);
    Func_02000df0(11, 0xd000, 0);
    Func_02000dfc(0, 0xe000, 0);
    Func_02000d98(11, 138, 416);
    Func_02000e14(11, 0xb000, 10);
    Func_02000ddc(11, 2);
    Func_02000e16(11, 0, 40);
    Func_02000df6(8, 2);
    Func_02000e28(8, 0, 40);

    Func_02000e4c(9, 256, 20);
    Func_02000e14(9, 2);
    Func_02000e46(9, 0, 20);

    Func_02000e62(0, 0xc000, 0);
    Func_02000dfe(11, 144, 420);
    Func_02000dc4(20);

    Func_02000e44(9, 2);
    Func_02000e76(9, 0, 20);
    Func_02000ea0(9, 258);
    Func_02000e58(9, 3);
    Func_02000e92(9, 0, 20);

    Func_02000eae(11, 0x5000, 20);
    Func_02000eae(11, 0);

    if (Func_02000df6_b(0x9b0) != 0) {
        Func_02000ece(11, 0xd000, 40);
        Func_02000ee8(11, 258);
        Func_02000e2e(40);
        Func_02000ed8(11, 0, 10);
    } else {
        /* Skip-beat counter: the scene variant that plays no beat here still
         * advances the workspace beat cursor. */
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) += 1;
    }

    Func_02000f06(11, 0x5000, 10);
    Func_02000f00(11, 0, 40);
    Func_02000f24(11, 256, 40);
    Func_02000f28(11, 0xb000, 10);
    Func_02000f22(11, 0, 10);
    Func_02000ece(11, 138, 416);
    Func_02000f4a(11, 0xb000, 20);
    Func_02000f12(8, 2);
    Func_02000f4c(8, 0, 10);
    Func_02000f76(11, 258);
    Func_02000f36(11, 1);
    Func_02000ec4(20);
    Func_02000f6e(11, 0, 20);

    Func_02000f4e(9, 2);
    Func_02000edc(20);
    Func_02000f98(0, 0xe000, 10);
    Func_02000f68(9, 1);
    Func_02000ef6(20);
    Func_02000fa0(9, 0, 10);

    Func_02000f80(8, 2);
    Func_02000fb2(8, 0, 20);

    Func_02000f92(11, 1);
    Func_02000f20(20);
    Func_02000fca(11, 0, 20);
    Func_02000fe6(11, 0x5000, 10);
    Func_02000fe0(11, 0, 10);
    Func_02000fa8(11, 2);

    actor = Func_02000f6e_b(0);
    if (actor != 0) {
        Func_02000f90(11, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    }
    Func_02000fb6(11);
    Func_02000fc8(11, 0, 0);

    mode = 7;
    Func_02000f5c(6, 27, 1, 1, mode, 27);
    Func_02000f6e(9, 26, 2, 1, mode, 26);

    Func_02000f8c(0x89f);

    Func_02000fb0();
}
