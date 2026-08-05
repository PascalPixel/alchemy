#include "types.h"

/*
 * Resource 3ad, cutscene at 0x020004a8 (896 bytes, 95 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x020004a8 and `pop {r5} / pop {r0} /
 * bx r0` at 0x02000800.  The popped branch register is r0, so it holds the
 * return address and the owner is `void`.  The entry guard reaches that
 * epilogue through a long `b.n 0x2000800` (the `bne` over it exists only
 * because the short branch could not span the body), so it is an early return,
 * not a call.
 *
 * An eight-word literal pool occupies 0x02000808-0x02000827, past the return:
 *   0x941, 0x94d (story flags), 0x00014ccc / 0x0000a666 (a coordinate pair),
 *   0x250d, 0x2512 (dialogue line bases), 0x00006666 / 0x00003333 (a second
 *   coordinate pair).
 * Two of those are the documented trap: 0x00014ccc disassembles as a plausible
 * `ldr r4,[pc,#816]` and 0x0000a666 as `add r6,pc,#408`.  They are data - the
 * control flow never reaches 0x02000810, and both words are loaded by the four
 * `Func_0808a090` call sites in the body.
 *
 * r5 is the dialogue line cursor and is reloaded twice: 0x250d for the first
 * conversation, 0x2512 for the second, then `adds r5, #3` to 0x2515.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; 94 sites are
 * import veneers and one is this overlay's own prologue at 0x02000828.  The
 * per-target multiset is reproduced exactly by the C, target for target:
 *   Func_0808a1b8 x10, Func_0808a170 x9, Func_0808a010 x8, Func_0808a180 x8,
 *   Func_0808a0e8 x7,  Func_0808a090 x6, Func_0808a0c8 x6, Func_0808a0f0 x5,
 *   Func_0808a080 x4,  Func_0808a130 x4, Func_0808a148 x4, Func_0808a1e8 x4,
 *   Func_0808a100 x3,  Func_0808a110 x2, Func_0808a138 x2, Func_0808a578 x2,
 *   and one each of Func_080770c0, Func_080770c8, Func_0808a018,
 *   Func_0808a020, Func_0808a070, Func_0808a178, Func_0808a208,
 *   Func_0808a210, Func_0808a368, Func_0808a370, Func_02000828.
 * (The inventory row says calls=86, a known lower bound.)
 *
 * The four `Func_0808a080(0)` blocks read the actor record's 16.16 words at +8
 * and +16 directly (`ldr`), not the s16 views at +10/+18 that 0x02001760 and
 * 0x02001448 use.
 *
 * Shift-built constants, all spelled as their values below: 0x90 << 16,
 * 0xc8 << 17, 0xc0 << 9, 0xc0 << 8, 0xc8 << 1, 0xc0 << 6, 0x80 << 8, 0x80 << 5,
 * 0xc0 << 16, 0xd8 << 17, 0xe8 << 1, 0xf4 << 1, 0x81 << 1, 0x80 << 1,
 * 0x80 << 7, 0xd8 << 1.  -1, -16 and -13 are `movs`/`negs` pairs.
 */

/* Old-style declarations: overlay import arities vary per call site. */
s32 Func_080770c0();    /* test a story flag (used in a condition) */
void Func_080770c8();   /* set a story flag */
void Func_0808a010();   /* wait n frames */
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();    /* dialogue prompt; result selects the branch */
u8 *Func_0808a080();    /* scene entity record by selector */
void Func_0808a090();
void Func_0808a0c8();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();   /* scene-presentation request */
void Func_0808a130();
void Func_0808a138();
void Func_0808a148();   /* scene request */
void Func_0808a170();   /* show a dialogue line by id */
void Func_0808a178();   /* dialogue-line variant with a mode word */
void Func_0808a180();   /* wait for the slot's action to finish */
void Func_0808a1b8();   /* face the slot along a heading */
void Func_0808a1e8();   /* play animation id for n frames */
void Func_0808a208();
void Func_0808a210();
void Func_0808a368();
void Func_0808a370();
void Func_0808a578();
void Func_02000828();   /* this overlay */

void Func_020004a8(void)
{
    u8 *actor;
    int line;

    if (Func_080770c0(0x941) == 0) {
        return;
    }
    Func_080770c8(0x94d);

    Func_0808a018();

    Func_0808a0f0(12, 0x900000, 0x1900000);
    Func_0808a090(12, 0x18000, 0xc000);
    Func_0808a0c8(12, 184, 400);
    Func_0808a0e8(12);
    Func_0808a100(12, 1);
    Func_0808a1b8(12, 0x3000, 0);

    Func_0808a138(0, 1);
    Func_0808a010(30);

    Func_0808a208(0x8000, 0x1000);
    Func_0808a210(0xc00000, -1, 0x1b00000, 1);

    /*
     * Four near-identical blocks: place slot N at actor 0's 16.16 position,
     * scale it, position it on screen, commit and face it.
     */
    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0f0(13, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    }
    Func_0808a090(13, 0x14ccc, 0xa666);
    Func_0808a0c8(13, 168, 464);
    Func_0808a0e8(13);
    Func_0808a1b8(13, 0xc000, 0);

    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0f0(2, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    }
    Func_0808a090(2, 0x14ccc, 0xa666);
    Func_0808a0c8(2, 152, 488);
    Func_0808a0e8(2);
    Func_0808a1b8(2, 0xc000, 0);

    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0f0(3, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    }
    Func_0808a090(3, 0x14ccc, 0xa666);
    Func_0808a0c8(3, 168, 488);
    Func_0808a0e8(3);
    Func_0808a1b8(3, 0xc000, 0);

    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0f0(1, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    }
    Func_0808a090(1, 0x14ccc, 0xa666);
    Func_0808a0c8(1, 184, 488);
    Func_0808a0e8(1);
    Func_0808a1b8(1, 0xc000, 0);

    Func_0808a010(30);
    Func_0808a138(1, 1);

    line = 0x250d;
    Func_0808a170(line);
    Func_0808a180(1, 0);
    Func_0808a010(30);

    Func_0808a110(3, 3);
    Func_0808a010(10);
    Func_0808a170(line + 1);
    Func_0808a180(3, 0);

    Func_0808a1e8(2, 0x102, 70);
    Func_0808a148(2, 0, 0);
    Func_0808a148(0, 2, 0);
    Func_0808a170(line + 2);
    Func_0808a178(2, 0);

    Func_0808a148(3, 0, 0);
    Func_0808a148(1, 0, 0);
    Func_0808a010(30);

    /* Two arms, each with its own line id and its own pair of call sites. */
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a170(line + 3);
        Func_0808a180(1, 0);
    } else {
        Func_0808a170(line + 4);
        Func_0808a180(1, 0);
    }

    Func_0808a1e8(13, 0x100, 70);

    line = 0x2512;
    Func_0808a170(line);
    Func_0808a180(13, 0);

    Func_0808a130(0, 2);
    Func_0808a130(1, 2);
    Func_0808a130(2, 2);
    Func_0808a130(3, 2);

    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a1b8(3, 0xc000, 0);

    Func_0808a100(13, 2);
    Func_0808a578(13, 0, -16);
    Func_0808a0e8(13);
    Func_0808a100(13, 1);
    Func_0808a170(line + 1);
    Func_0808a180(13, 0);

    Func_0808a1e8(12, 0x100, 65);
    Func_0808a170(line + 2);
    Func_0808a180(12, 0);

    Func_0808a110(13, 3);
    Func_0808a010(80);

    Func_0808a090(12, 0x6666, 0x3333);
    Func_0808a578(12, -13, 0);
    Func_0808a0e8(12);
    Func_0808a1b8(12, 0x4000, 0);

    Func_0808a1e8(12, 0x102, 70);
    Func_0808a170(line);
    line += 3;
    Func_0808a180(12, 0);
    Func_0808a0c8(12, 168, 432);
    Func_0808a010(40);

    Func_0808a368();
    Func_0808a370();
    Func_0808a010(20);
    Func_0808a020();

    Func_02000828();
}
