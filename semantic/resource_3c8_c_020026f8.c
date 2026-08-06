#include "types.h"

/*
 * Resource 3c8 at 0x020026f8: the 320-frame sequence that drops a shower of
 * particles down a shaft while a workspace value creeps sideways.
 *
 * The owner is complete: `push {r5, r6, r7, lr}` plus the high-register saves
 * `mov r7, fp ; mov r6, sl ; mov r5, r9 ; push {r5, r6, r7}` and
 * `mov r7, r8 ; push {r7}` at 0x020026f8, a 56-byte frame, and the matching
 * unwind at 0x020028c6..0x020028d6, followed by a 13-word literal pool at
 * 0x020028d8-0x0200290b.  No live frame or register state escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.  All four guard failures branch to that same epilogue.
 *
 * The entry guard is the standard rectangle test on the actor's integer
 * coordinate halfwords at +0x0a and +0x12 (the high halves of the 16.16 words
 * at +0x08 and +0x10; see the two struct views in the byte-exact sibling
 * 0x020014f4): -532 is added to the first and the sum compared unsigned
 * against 7, so it must lie in [532, 539], and the second must satisfy
 * 324 <= v < 332.  `actor->y` is cleared before the test and set to the
 * pooled 0xfffe0000 after it - the same constant the byte-exact sibling
 * 0x02001644 writes into the same record.
 *
 * The 40-byte parameter block at sp+16 and the eight-argument spawn call are
 * this package's standard effect ABI, reconstructed in
 * semantic/overlays/resource_3c8_c_02000118.c.  The flag word 0x008a0000
 * selects exactly the three fields this loop fills: 0x00020000 takes
 * `unk00 & 3`, 0x00080000 takes `color1`/`color2`, 0x00800000 takes the
 * halfword at +0x22.  Nothing else in the block is written, and nothing else
 * is read.
 *
 * The three random results are scaled with the shift/add chains
 * `(u32)(r * 3) >> 16` then `* 3 * 17 * 257` (= * 0x3333) and
 * `(u32)(r << 12) >> 16`.  Their imports are declared `(void)`, following the
 * byte-exact sibling 0x02000b98's random family.
 *
 * Loop shape: `counter` runs the unsigned range 0..0x13f (320 iterations),
 * `height` starts at 0x02680000 and drops by 0x3333 per spawn until it
 * reaches 0x023fffff, and `countdown` starts at 60 and is decremented every
 * iteration but only *tested* inside the spawn block, so it can run negative
 * when spawning is suppressed.  That is what the code does and it is
 * preserved rather than normalised.
 *
 * The tail rounds the workspace word at +8 to a whole 16.16 unit: it stores
 * `v + 0x8000`, then stores `(that / 0x10000) * 0x10000`, the truncating
 * division spelled with the pooled 0x00017fff bias for the negative case.
 * Both stores are in the original.
 *
 * All 22 `bl` sites reach 21 distinct targets - 0x020075e8 is called twice,
 * once with no argument register written and once with three, which is why
 * every import is declared old-style.  The full list: 0x020075c2,
 * 0x020075be, 0x020075e8, 0x02007582, 0x02007768, 0x0200759a, 0x02007610,
 * 0x02007796, 0x0200762c, 0x02007588, 0x020075a2, 0x020075bc, 0x02002956,
 * 0x0200767a, 0x02007608, 0x02007890, 0x02007896, 0x020076fc, 0x02007708,
 * 0x02007864, 0x02007760.  Import naming follows the note in
 * resource_3c8_c_020002f0.c.
 */

struct Actor_020026f8 {
    u8 unknown_00[10];
    s16 gridX;                  /* 0x0a - integer part of the 16.16 x at +8 */
    s32 y;                      /* 0x0c */
    u8 unknown_10[2];
    s16 gridZ;                  /* 0x12 - integer part of the 16.16 z at +16 */
};

struct Workspace_020026f8 {
    u8 unknown_00[8];
    s32 drift;                  /* 0x08 */
};

struct EffectParams_020026f8 {
    s32 unk00;                  /* 0x00 */
    s32 mode;                   /* 0x04 - not written; flag bit not set */
    s32 color1;                 /* 0x08 */
    s32 color2;                 /* 0x0c */
    u8 unknown_10[0x12];
    u16 unk22;                  /* 0x22 */
    u8 unknown_24[4];
};

/*
 * CALL SYMBOLS ARE PER-SITE: every one of the 22 `bl`s in the raw assembly
 * is a direct relocation against an absolute .set symbol (verified via
 * `arm-none-eabi-objdump -dr -M force-thumb` on the assembled .o) except
 * Func_02000118, whose call is the one genuine indirect call routed through
 * this overlay's own `_call_via_rN` bank. This header's own comment already
 * had the correct per-site address list (0x020075c2, 0x020075be, ...); the
 * bug was that the C body below called them by their veneer-math resolved
 * names (Func_0808a080 etc) instead. 0x020075e8 is one physical routine
 * shared by two call sites (call3, no args, and call8, three args) -- one
 * declaration, two call sites, matching the raw assembly exactly.
 */
void Func_02000118();
struct Actor_020026f8 *Func_020075c2();
s32 Func_020075be();
void Func_020075e8();
void Func_02007582();
void Func_02007768();
void Func_0200759a();
void Func_02007610();
void Func_02007796();
void Func_0200762c();
s32 Func_02007588();
s32 Func_020075a2();
s32 Func_020075bc();
void Func_0200767a();
void Func_02007608();
void Func_02007890();
void Func_02007896();
void Func_020076fc();
void Func_02007708();
void Func_02007864();
void Func_02007760();

void Func_020026f8(void)
{
    struct Workspace_020026f8 *workspace;
    struct Actor_020026f8 *actor;
    struct EffectParams_020026f8 params;
    s32 gridX;
    s32 gridZ;
    s32 height;
    s32 countdown;
    s32 index;
    u32 counter;
    s32 rounded;

    workspace = (struct Workspace_020026f8 *)(*(u8 **)0x03001e70 + 356);

    actor = Func_020075c2(0);
    gridX = actor->gridX;
    gridZ = actor->gridZ;
    actor->y = 0;
    if ((u32)(gridX - 532) > 7 || gridZ < 324 || gridZ >= 332) {
        return;
    }
    actor->y = 0xfffe0000;

    if (Func_020075be(0x306) != 0) {
        return;
    }

    /* No argument register is written here; r0 still holds the 0 just
     * returned above, and that dataflow is preserved as written. */
    Func_020075e8(0);

    { s32 f1 = 1; s32 g1 = 1; Func_02007582(63, 29, 33, 20,  f1, g1); }
    Func_02007768(161);
    { s32 f2 = 3; s32 g2 = 3; Func_0200759a(44, 83, 44, 80,  f2, g2); }
    Func_02007610(30);
    Func_020075e8(0x10000, 0x10000, 0x10000);
    Func_02007796(239);
    Func_0200762c(20);

    height = 0x02680000;
    index = 0;
    countdown = 60;
    counter = 0;
    do {
        workspace->drift += 0x3333;

        if (height > 0x023fffff && counter > 40) {
            height += 0xffffcccd;       /* -0x3333 */
            params.unk00 = 2;
            params.color1 =
                (s32)((u32)(Func_02007588() * 3) >> 16) * 0x3333 + 0xcccc;
            params.color2 =
                (s32)((u32)(Func_020075a2() * 3) >> 16) * 0x3333 + 0xcccc;
            params.unk22 =
                (u16)(((u32)(Func_020075bc() * 0x1000) >> 16) + 0xf800);

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
                index += 4;
                { s32 f3 = 3; s32 g3 = 4; Func_0200767a(index, 56, 36, 17,  f3, g3); }
            }
        }

        Func_02007608(1);
        counter++;
        countdown--;
    } while (counter <= 0x13f);

    rounded = workspace->drift + 0x8000;
    workspace->drift = rounded;
    workspace->drift = (rounded / 0x10000) << 16;

    Func_02007890(288);
    Func_02007896(188);
    Func_020076fc(-1, -1, 0xe666);
    Func_02007708();

    *(s32 *)(*(u8 **)0x03001ebc + 448) = 514;

    Func_02007864(18);
    Func_02007760();
}
