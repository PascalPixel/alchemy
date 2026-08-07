#include "types.h"

/*
 * Resource 3c8 at 0x02001af0: a 480-frame particle shower, the longest of the
 * three in this package (compare 0x020026f8 and 0x0200290c).
 *
 * The owner is complete: `push {r5, r6, r7, lr}` plus the high-register saves
 * `mov r7, fp ; mov r6, sl ; mov r5, r9 ; push {r5, r6, r7}` and
 * `mov r7, r8 ; push {r7}` at 0x02001af0, a 56-byte frame, and the matching
 * unwind at 0x02001d00..0x02001d10, followed by an alignment halfword and a
 * 13-word literal pool at 0x02001d12-0x02001d47.  No live frame or register
 * state escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.  All four guard failures branch to that same epilogue.
 *
 * ONE IMPORT, TWO ARGUMENT COUNTS.  0x0200698c is reached twice: at
 * 0x02001b6e with six arguments (r0-r3 plus [sp,#0] and [sp,#4], the
 * placement ABI proven by the byte-exact sibling 0x020010cc), and at
 * 0x02001bd6 with none, as the second random source of the loop.  Both `bl`
 * encodings were decoded by hand and do resolve to the same address
 * (0x02001b6e + 4 + 0x4e1a and 0x02001bd6 + 4 + 0x4db2).  This is the
 * clearest instance in the package of the rule that an overlay `Func_` name
 * is an import identity that may take different argument counts at different
 * sites, and it is why every import here is declared old-style.
 *
 * The entry guard is the usual rectangle test on the actor's integer
 * coordinate halfwords at +0x0a and +0x12 (high halves of the 16.16 words at
 * +0x08 and +0x10): -308 added to the first and compared unsigned against 7,
 * so [308, 315]; the second must satisfy 532 <= v < 540.  `actor->y` is
 * cleared before the test and set to the pooled 0xfffe0000 after it.
 *
 * The 40-byte parameter block at sp+16 and the eight-argument spawn call are
 * this package's effect ABI, reconstructed in
 * semantic/overlays/resource_3c8_c_02000118.c.  Flag word 0x008a0000 selects
 * `unk00 & 3`, `color1`/`color2` and the halfword at +0x22 - exactly the
 * fields this loop fills.
 *
 * Loop shape: `counter` runs the unsigned range 0..0x1df (480 iterations);
 * the emitter height starts at 0x01200000 and drops 0x3333 every iteration,
 * with one extra 0x00300000 drop on the single iteration where the counter is
 * exactly 240; `countdown` starts at 40, is decremented every iteration and
 * reloaded to 40 whenever it reaches 0, choosing one of two sub-sequence
 * calls according to whether the counter has passed 240 - the index walks
 * down from 29 before that point and back up after it.
 *
 * The tail rounds the workspace word at +8 to a whole 16.16 unit with two
 * stores (`v + 0x8000`, then `(that / 0x10000) * 0x10000`, the pooled
 * 0x00017fff bias covering the negative case) and finally writes 0x202 - 514,
 * the same value the sibling showers write - into the workspace slot at +448.
 *
 * Twenty-five `bl` sites reach 24 distinct targets, all accounted for:
 * 0x020069bc, 0x020069ba, 0x020069e4, 0x02006b52, 0x020069da, 0x0200698c,
 * 0x02006a02, 0x02006b78, 0x020069e0, 0x02006a1e, 0x02006970, 0x020069a8,
 * 0x02001d42, 0x02006a8a, 0x02006a72, 0x02006a18, 0x02006ae8, 0x02006afa,
 * 0x02006cca, 0x02006cd0, 0x02006b36, 0x02006b42, 0x02006c9e, 0x02006b9a.
 * Import naming follows the note in resource_3c8_c_020002f0.c.
 */

struct Actor_02001af0 {
    u8 unknown_00[10];
    s16 gridX;                  /* 0x0a - integer part of the 16.16 x at +8 */
    s32 y;                      /* 0x0c */
    u8 unknown_10[2];
    s16 gridZ;                  /* 0x12 - integer part of the 16.16 z at +16 */
};

struct Workspace_02001af0 {
    u8 unknown_00[8];
    s32 drift;                  /* 0x08 */
};

struct EffectParams_02001af0 {
    s32 unk00;                  /* 0x00 */
    s32 mode;                   /* 0x04 - not written; flag bit not set */
    s32 color1;                 /* 0x08 */
    s32 color2;                 /* 0x0c */
    u8 unknown_10[0x12];
    u16 unk22;                  /* 0x22 */
    u8 unknown_24[4];
};

/* Used for their return values. */
void Func_02000118();
void Func_080000c0();
s32 Func_080000f8();
void Func_08009180();
void Func_080091c0();
void Func_080091f0();
void Func_080091f8();
s32 Func_080770c0();
void Func_080770c8();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
struct Actor_02001af0 *Func_0808a080();
void Func_0808a248();
void Func_080f9010();

/*
 * Old-style declarations throughout: 0x0200698c in particular is called both
 * as a six-argument placement service and as a zero-argument random source.
 */


void Func_02001af0(void)
{
    struct Workspace_02001af0 *workspace;
    struct Actor_02001af0 *actor;
    struct EffectParams_02001af0 params;
    s32 gridX;
    s32 gridZ;
    s32 height;
    s32 countdown;
    s32 index;
    u32 counter;
    s32 rounded;

    workspace = (struct Workspace_02001af0 *)(*(u8 **)0x03001e70 + 356);

    actor = Func_0808a080(0);
    gridX = actor->gridX;
    gridZ = actor->gridZ;
    actor->y = 0;
    if ((u32)(gridX - 308) > 7 || gridZ < 532 || gridZ >= 540) {
        return;
    }
    actor->y = 0xfffe0000;

    if (Func_080770c0(768) != 0) {
        return;
    }

    /* No argument register is written here; r0 still holds the 0 just
     * returned above, and that dataflow is preserved as written. */
    Func_0808a018(0);

    Func_080f9010(161);
    Func_080770c8(768);
    Func_08009180(26, 33, 19, 33, 1, 1);
    Func_0808a010(30);
    Func_080f9010(239);
    Func_080091f0(0x10000, 0x10000, 0x10000);
    Func_0808a010(20);

    height = 0x01200000;
    countdown = 40;
    index = 29;
    counter = 0;
    do {
        workspace->drift += 0x3333;
        height += 0xffffcccd;           /* -0x3333 */

        params.unk00 = 2;
        params.color1 =
            (s32)((u32)(Func_080000f8() * 3) >> 16) * 0x3333 + 0xcccc;
        params.color2 =
            (s32)((u32)(Func_080000f8() * 3) >> 16) * 0x3333 + 0xcccc;
        params.unk22 =
            (u16)(((u32)(Func_080000f8() * 0x1000) >> 16) + 0xf800);

        Func_02000118(
            height,
            0,
            0x02100000,
            0,
            -(s32)(((*(u32 *)0x03001e40 & 1) * 3) << 16),
            0,
            0x008a0000,
            &params);

        if (counter == 240) {
            height += 0xffd00000;       /* -0x00300000 */
        }

        if (countdown == 0) {
            countdown = 40;
            if (counter > 240) {
                index += 4;
                Func_08009180(index, 45, 9, 32, 3, 4);
            } else {
                index -= 4;
                Func_08009180(index, 50, 15, 32, 3, 4);
            }
        }

        Func_080000c0(1);
        counter++;
        countdown--;
    } while (counter <= 0x1df);

    rounded = workspace->drift + 0x8000;
    workspace->drift = rounded;
    workspace->drift = (rounded / 0x10000) << 16;

    Func_080091c0(15, 32, 3, 1, 9, 32);
    Func_080091c0(12, 32, 3, 1, 15, 32);
    Func_080f9010(288);
    Func_080f9010(188);
    Func_080091f0(-1, -1, 0xe666);
    Func_080091f8();

    *(s32 *)(*(u8 **)0x03001ebc + 448) = 0x202;

    Func_0808a248(11);
    Func_0808a020();
}
