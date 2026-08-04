#include "types.h"

/*
 * Resource 3c8 at 0x0200290c: the mirror of 0x020026f8 - the same 320-frame
 * particle shower, entered from the opposite side of the map and running the
 * sweep in the other direction.
 *
 * The owner is complete: `push {r5, r6, r7, lr}` plus the high-register saves
 * `mov r7, fp ; mov r6, sl ; mov r5, r9 ; push {r5, r6, r7}` and
 * `mov r7, r8 ; push {r7}` at 0x0200290c, a 56-byte frame, and the matching
 * unwind at 0x02002aca..0x02002ada, followed by a 14-word literal pool at
 * 0x02002adc-0x02002b13.  No live frame or register state escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.  All four guard failures branch to that same epilogue.
 *
 * Differences from 0x020026f8, all of them constants or signs:
 *   - the entry rectangle is [788, 795] x [324, 332) instead of [532, 539];
 *   - the one-shot key is 0x307 instead of 0x306;
 *   - the workspace word at +8 drifts by -0x3333 per frame, not +0x3333;
 *   - the emitter height starts at 0x02c00000 and *rises* by 0x3333 every
 *     frame (unconditionally, at the top of the loop), and the spawn window
 *     is the unsigned range test `(u32)(height - 0x02c80000) <= 0x0027ffff`
 *     spelled as `0xfd380000 + height` compared `bhi 0x0027ffff`;
 *   - the sub-sequence index counts *down* from 61 by 4;
 *   - one placement call fewer in the opening block.
 *
 * The entry guard reads the actor's integer coordinate halfwords at +0x0a and
 * +0x12 - the high halves of the 16.16 words at +0x08 and +0x10, the two
 * struct views of the byte-exact sibling 0x020014f4.  `actor->y` is cleared
 * before the test and set to the pooled 0xfffe0000 after it, the same
 * constant the byte-exact sibling 0x02001644 writes into the same record.
 *
 * The 40-byte parameter block at sp+16 and the eight-argument spawn call are
 * this package's effect ABI, reconstructed in
 * semantic/overlays/resource_3c8_c_02000118.c.  Flag word 0x008a0000 selects
 * `unk00 & 3` (0x00020000), `color1`/`color2` (0x00080000) and the halfword
 * at +0x22 (0x00800000) - exactly the fields this loop fills.
 *
 * `countdown` starts at 60 and is decremented every iteration but only
 * *tested* inside the spawn block, so it can run negative when spawning is
 * suppressed.  Preserved rather than normalised, as in 0x020026f8.
 *
 * The tail rounds the workspace word at +8 to a whole 16.16 unit with two
 * stores: `v + 0x8000`, then `(that / 0x10000) * 0x10000` using the pooled
 * 0x00017fff bias for the negative case.  Both stores are in the original.
 *
 * All 21 `bl` sites reach 21 distinct targets, all accounted for:
 * 0x020077d6, 0x020077d2, 0x020077fc, 0x02007796, 0x0200797c, 0x02007812,
 * 0x020077ea, 0x02007998, 0x0200782e, 0x0200778a, 0x020077a4, 0x020077be,
 * 0x02002b5a, 0x0200787e, 0x0200780c, 0x02007a94, 0x02007a9a, 0x02007900,
 * 0x0200790c, 0x02007a68, 0x02007964.  Import naming follows the note in
 * resource_3c8_c_020002f0.c.
 */

struct Actor_0200290c {
    u8 unknown_00[10];
    s16 gridX;                  /* 0x0a - integer part of the 16.16 x at +8 */
    s32 y;                      /* 0x0c */
    u8 unknown_10[2];
    s16 gridZ;                  /* 0x12 - integer part of the 16.16 z at +16 */
};

struct Workspace_0200290c {
    u8 unknown_00[8];
    s32 drift;                  /* 0x08 */
};

struct EffectParams_0200290c {
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
void Func_080091f0();
void Func_080091f8();
s32 Func_080770c0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
struct Actor_0200290c *Func_0808a080();
void Func_0808a248();
void Func_080f9010();

/* Random source; the byte-exact siblings declare this family `(void)`. */

/* Old-style declarations: the imports' real interfaces are not known here. */

void Func_0200290c(void)
{
    struct Workspace_0200290c *workspace;
    struct Actor_0200290c *actor;
    struct EffectParams_0200290c params;
    s32 gridX;
    s32 gridZ;
    s32 height;
    s32 countdown;
    s32 index;
    u32 counter;
    s32 rounded;

    workspace = (struct Workspace_0200290c *)(*(u8 **)0x03001e70 + 356);

    actor = Func_0808a080(0);
    gridX = actor->gridX;
    gridZ = actor->gridZ;
    actor->y = 0;
    if ((u32)(gridX - 788) > 7 || gridZ < 324 || gridZ >= 332) {
        return;
    }
    actor->y = 0xfffe0000;

    if (Func_080770c0(0x307) != 0) {
        return;
    }

    /* No argument register is written here; r0 still holds the 0 just
     * returned above, and that dataflow is preserved as written. */
    Func_0808a018(0);

    Func_08009180(63, 29, 49, 20, 1, 1);
    Func_080f9010(161);
    Func_0808a010(30);
    Func_080091f0(0x10000, 0x10000, 0x10000);
    Func_080f9010(239);
    Func_0808a010(20);

    height = 0x02c00000;
    countdown = 60;
    index = 61;
    counter = 0;
    do {
        workspace->drift += 0xffffcccd;     /* -0x3333 */
        height += 0x3333;

        if ((u32)(height - 0x02c80000) <= 0x0027ffff) {
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
                0x01200000,
                0,
                -(s32)(((*(u32 *)0x03001e40 & 1) * 3) << 16),
                0,
                0x008a0000,
                &params);

            if (countdown == 0) {
                countdown = 40;
                index -= 4;
                Func_08009180(index, 56, 44, 17, 3, 4);
            }
        }

        Func_080000c0(1);
        counter++;
        countdown--;
    } while (counter <= 0x13f);

    rounded = workspace->drift + 0x8000;
    workspace->drift = rounded;
    workspace->drift = (rounded / 0x10000) << 16;

    Func_080f9010(288);
    Func_080f9010(188);
    Func_080091f0(-1, -1, 0xe666);
    Func_080091f8();

    *(s32 *)(*(u8 **)0x03001ebc + 448) = 514;

    Func_0808a248(19);
    Func_0808a020();
}
