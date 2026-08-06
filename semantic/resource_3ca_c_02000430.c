#include "types.h"

/*
 * Resource 3ca cutscene body at 0x02000430 (1796 bytes,
 * 0x02000430-0x02000b33).
 *
 * Complete owner: `push {r5, r6, lr}` / `sub sp, #8` at 0x02000430, matching
 * `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0` at 0x02000af8.  The popped
 * register is r0, so it holds the return address and the owner is `void`.
 * The 8-byte frame exists only to carry the fifth and sixth arguments of the
 * single Func_080091c0 call at 0x0200045a.
 *
 * Two literal pools sit inside the span.  The map comes from walking the
 * owner's control flow, which is the only method immune to both pool traps:
 *   0x02000808-0x02000813, reached only by falling past the `b.n 0x02000840`
 *     at 0x02000806 (an arm-join branch, not a dedicated pool hop), and
 *   0x02000b00-0x02000b33, after the epilogue.
 * Pool contents: 0x000028fe, 0x03001ebc, 0x00000101 in the first; 0x03001ebc,
 * 0x00013333, 0x00009999, 0x02009820, 0x020098e0, 0x0200998c, 0x02009a4c,
 * 0x03001e70, 0xffffd000, 0xffffa000, 0x00023333, 0x0000028f, 0x00000000 in
 * the second.  The four 0x02009xxx words are even, so under the 0x02008000
 * link base they are in-image DATA addresses (file offsets 0x1820, 0x18e0,
 * 0x198c, 0x1a4c), not callbacks — the parity test.  Each is the second
 * argument of a Func_0808a098 call, which is exactly the shape that test was
 * derived from.
 *
 * Calls.  188 `bl` sites, all resolved with tools/overlay_call_targets.ts;
 * the row's `calls=174` is the usual undercount.  The completeness proof is
 * per target, not a total — a matching total can hide a permutation:
 *
 *   Func_0808a010 x70   Func_0808a1b8 x28   Func_0808a180 x18
 *   Func_0808a110 x12   Func_0808a100  x7   Func_0808a138  x6
 *   Func_0808a1e8  x6   Func_0808a210  x4   Func_0808a090  x4
 *   Func_0808a098  x4   Func_0808a070  x3   Func_0808a178  x3
 *   Func_0808a570  x3   Func_0808a580  x3   Func_0808a0a0  x2
 *   Func_080f9010  x2   Func_080091c0  x1   Func_080770c8  x1
 *   Func_0808a018  x1   Func_0808a080  x1   Func_0808a0e8  x1
 *   Func_0808a170  x1   Func_0808a208  x1   Func_0808a218  x1
 *   Func_0808a238  x1   Func_0808a368  x1   Func_0808a370  x1
 *   Func_080f9020  x1   Func_020012ac  x1
 *   Total 188.
 *
 * Every call site here is straight-line except three, and all three are the
 * same shape: `Func_0808a070(0, 0)` returns a flag, and the two arms it
 * selects contain the *same* calls with the same arguments, differing only in
 * where the counter bump sits relative to the last call.  That is the
 * documented skip-beat counter, and it means the arms are behaviourally
 * identical.  They are nevertheless written out separately, because they are
 * genuinely distinct call sites in the image: collapsing them would drop six
 * calls from the multiset above, which is the mirror of the phantom-call trap.
 *
 * Two arithmetic idioms that are easy to misread and are spelled for their
 * effect here:
 *  - at 0x02000abe the displacement 224*2 = 448 is loaded, the address is
 *    formed, and only *then* is `subs r3, #192` applied to make the stored
 *    value 32.  The store is workspace + 448 = 32, not workspace + 256.
 *    Compare 0x02000da4, which writes 256 to the same field.
 *  - the counter bump is `ldr r2, [0x03001ebc] / adds r2, #472 / ldrh / adds
 *    #1 / strh`: a u16 at workspace + 472, where 472 is 236 << 1.
 *
 * UNCERTAINTIES.  (1) Func_0808a010 is read as "wait n frames" from the way
 * every site separates two visible state changes; the name is not otherwise
 * pinned down.  (2) Func_0808a018, Func_0808a218, Func_0808a368 and
 * Func_0808a370 are reached with no register deliberately set and are spelled
 * with no arguments.  (3) Func_0808a010's first argument is the only one this
 * owner ever varies for it, so its remaining registers are not asserted.
 */

/* The scene workspace pointer cell in IWRAM, and the record block. */
#define WORKSPACE   (*(u8 **)0x03001EBC)
#define RECORDS     (*(u8 **)0x03001E70)

/* The skip-beat counter: a u16 at workspace + 472 (236 << 1). */
#define BUMP_BEAT() (*(u16 *)(WORKSPACE + 472) += 1)

/* In-image data handed to Func_0808a098; even pool words under the 0x02008000
 * base, so data rather than callbacks. */
extern u8 Data_02009820[];
extern u8 Data_020098e0[];
extern u8 Data_0200998c[];
extern u8 Data_02009a4c[];

/* Main-image imports reached through this overlay's veneer table.  Old-style
 * declarations are mandatory: import arity varies between call sites, and two
 * names can be one callee. */
void Func_080091c0();
void Func_080770c8();
void Func_0808a010();   /* wait n frames */
void Func_0808a018();
s32  Func_0808a070();   /* returns the branch flag */
u8  *Func_0808a080();   /* scene-record accessor: slot id -> record */
void Func_0808a090();
void Func_0808a098();
void Func_0808a0a0();
void Func_0808a0e8();
void Func_0808a100();
void Func_0808a110();
void Func_0808a138();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a238();
void Func_0808a368();
void Func_0808a370();
void Func_0808a570();
void Func_0808a580();
void Func_080f9010();
void Func_080f9020();

/* In-image callee: the scene entry point that installs the curtain task. */
void Func_020012ac(void);

void Func_02000430(void)
{
    u8 *record;
    u8 *workspace;
    s32 *swing;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    /* Six arguments; the last two, 18 and 12, travel in the frame. */
    { s32 f1 = 18; s32 g1 = 12; Func_080091c0(18, 0, 3, 1,  f1, g1); }
    Func_0808a170(0x28fe);

    Func_0808a570(1, -10, 16, 0xc000);
    Func_0808a570(3, 0, 24, 0xc000);
    Func_0808a570(2, 10, 16, 0xc000);
    Func_0808a0e8(1);

    Func_0808a010(50);
    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a180(1, 0);
    Func_0808a010(10);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_0808a180(2, 0);
    Func_0808a010(10);
    Func_0808a1b8(3, 0x4000, 0);
    Func_0808a010(40);
    Func_0808a180(3, 0);
    Func_0808a010(10);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(2, 0x4000, 0);
    Func_0808a010(60);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a110(2, 3);
    Func_0808a010(60);
    Func_0808a138(1, 2);
    Func_0808a010(30);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a010(20);
    Func_0808a178(1, 0);

    /* First skip-beat pair: the same three calls either way. */
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(30);
        Func_0808a1e8(1, 258, 50);      /* 129 << 1 */
        Func_0808a180(1, 0);
        BUMP_BEAT();
    } else {
        Func_0808a010(30);
        Func_0808a1e8(1, 258, 50);
        BUMP_BEAT();
        Func_0808a180(1, 0);
    }

    Func_0808a010(10);
    Func_0808a1b8(2, 0x8000, 0);
    Func_0808a010(30);
    Func_0808a1e8(2, 0x101, 50);
    Func_0808a180(2, 0);
    Func_0808a010(10);
    Func_0808a1b8(1, 0, 0);
    Func_0808a010(30);
    Func_0808a1e8(1, 0x101, 60);
    Func_0808a138(3, 2);
    Func_0808a010(30);
    Func_0808a1b8(3, 0xa000, 0);
    Func_0808a010(30);
    Func_0808a180(3, 0);
    Func_0808a010(10);
    Func_0808a1e8(1, 256, 50);          /* 128 << 1 */
    Func_0808a180(1, 0);
    Func_0808a010(10);
    Func_0808a138(2, 2);
    Func_0808a010(20);
    Func_0808a180(2, 0);
    Func_0808a010(10);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a010(20);
    Func_0808a110(3, 4);
    Func_0808a010(20);
    Func_0808a178(3, 0);

    /* Second skip-beat pair. */
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        Func_0808a138(1, 2);
        Func_0808a010(20);
        Func_0808a1b8(1, 0xe000, 0);
        Func_0808a010(20);
        Func_0808a180(1, 0);
        BUMP_BEAT();
    } else {
        Func_0808a010(20);
        Func_0808a138(1, 2);
        Func_0808a010(20);
        Func_0808a1b8(1, 0xe000, 0);
        Func_0808a010(20);
        BUMP_BEAT();
        Func_0808a180(1, 0);
    }

    Func_0808a010(10);
    Func_0808a1e8(0, 258, 50);
    Func_0808a010(10);
    Func_0808a1b8(2, 0xa000, 0);
    Func_0808a010(20);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_0808a180(2, 0);
    Func_0808a010(20);
    Func_0808a180(1, 0);
    Func_0808a010(20);
    Func_0808a110(0, 3);
    Func_0808a010(30);
    Func_0808a010(10);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_0808a178(3, 0);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a1b8(2, 0xa000, 0);
    Func_0808a010(20);

    /* Third skip-beat pair. */
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(30);
        Func_0808a110(3, 3);
        Func_0808a010(20);
        Func_0808a180(3, 0);
        BUMP_BEAT();
    } else {
        Func_0808a010(30);
        Func_0808a110(3, 3);
        Func_0808a010(20);
        BUMP_BEAT();
        Func_0808a180(3, 0);
    }

    Func_0808a010(10);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a010(30);
    Func_0808a180(1, 0);
    Func_0808a010(10);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a010(30);
    Func_0808a010(10);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a010(30);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_0808a180(2, 0);
    Func_0808a010(10);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_0808a180(3, 0);

    Func_080f9010(17);
    Func_0808a010(10);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a110(2, 3);
    Func_0808a010(40);

    Func_0808a090(0, 0x13333, 0x9999);
    Func_0808a090(1, 0x13333, 0x9999);
    Func_0808a090(2, 0x13333, 0x9999);
    Func_0808a090(3, 0x13333, 0x9999);

    Func_0808a098(0, Data_02009820);
    Func_0808a010(50);
    Func_0808a210(0x1080000, -1, 0xc80000, 1);   /* 132 << 17, 200 << 16 */
    Func_0808a098(1, Data_020098e0);
    Func_0808a010(50);
    Func_0808a098(2, Data_0200998c);
    Func_0808a0a0(2);

    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(2, 0x4000, 0);
    Func_0808a580(3, 0, -32);
    Func_0808a010(30);
    Func_0808a1b8(3, 0x4000, 0);
    Func_0808a010(60);
    Func_0808a1b8(3, 0x8000, 0);
    Func_0808a010(20);
    Func_0808a098(3, Data_02009a4c);
    Func_0808a0a0(3);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a010(20);
    Func_0808a210(0xd80000, -1, 0xa80000, 1);    /* 216 << 16, 168 << 16 */
    Func_0808a218();
    Func_0808a010(20);

    Func_0808a1b8(0, 0x8000, 0);
    Func_0808a1b8(1, 0x8000, 0);
    Func_0808a1b8(2, 0x8000, 0);
    Func_0808a1b8(3, 0x8000, 0);
    Func_0808a010(40);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(3, 3);
    Func_0808a110(2, 3);
    Func_0808a010(30);

    Func_080f9010(67);
    Func_080f9020(240);
    Func_020012ac();
    Func_0808a010(80);

    /*
     * Slot 8's record is retuned, and a 16.16 field 284 bytes into the record
     * block (142 << 1) is stepped from -12288.0/65536 to -8192.0/65536 between
     * two 60-frame moves.  The record-block pointer is read once, before the
     * accessor call, and both stores go through it.
     */
    swing = (s32 *)(RECORDS + 284);   /* read before the accessor call */
    record = Func_0808a080(8);
    *(s32 *)(record + 52) = 131;
    *(s32 *)(record + 48) = 0x20000;        /* 128 << 10 */
    *swing = (s32)0xffffd000;
    Func_0808a580(8, 60, 0);
    *swing = (s32)0xffffa000;
    Func_0808a580(8, 60, 0);

    Func_0808a010(80);
    Func_0808a010(100);
    Func_0808a208(0x23333, 0x28f);
    Func_0808a210(0x3280000, -1, 0xa80000, 1);   /* 202 << 18, 168 << 16 */
    Func_0808a010(300);                          /* 150 << 1 */

    /*
     * Tear-down.  The 448 displacement is formed first and only then reduced
     * to the stored value 32 — see the header note.
     */
    workspace = WORKSPACE;
    *(s32 *)(workspace + 448) = 32;
    *(volatile u16 *)0x05000000 = 0;
    *(s32 *)(workspace + 456) = 96;

    Func_0808a368();
    Func_0808a370();
    Func_0808a010(30);
    Func_080770c8(282);                          /* 141 << 1 */
    Func_0808a238(0, 9);
}
