#include "types.h"

/*
 * Resource 3c8 at 0x02001f60: the "party members take their marks" step -
 * for each of the three followers, find which of four marked tiles it is
 * standing on, refuse if another follower shares the tile, then seat it and,
 * once all three are seated, run the payoff and pick one of two endings.
 *
 * The owner is complete: `push {r5, r6, r7, lr}` plus the high-register saves
 * `mov r7, fp ; mov r6, sl ; mov r5, r9 ; push {r5, r6, r7}` and
 * `mov r7, r8 ; push {r7}` at 0x02001f60, a 20-byte frame, and the matching
 * unwind at 0x02002236..0x02002246, followed by a nine-word literal pool at
 * 0x02002248-0x0200226b that ends the row.  No live frame or register state
 * escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.  The early exit at 0x02002148 skips the loop's own tail
 * call at 0x020070d0 and lands on the same unwind.
 *
 * `Data_0200d128` is the four-entry `(x >> 20, z >> 20)` position table this
 * reconstruction's 0x02004bd8 also reads for scene 0xb9; the match index is scaled by 8
 * because each entry is a pair of words.  A match additionally requires the
 * actor's `y` to be non-negative.  Index 5 is the sentinel for "no tile", and
 * it is also written back over a real index when a second follower is found
 * on the same tile.
 *
 * The eight bytes at [sp,#0]/[sp,#4] carry arguments five and six of this
 * package's six-argument placement ABI, the order fixed by the byte-exact
 * sibling 0x020010cc.  [sp,#12] holds the leader record fetched once at the
 * top; [sp,#16] holds an optional handle that is only produced when the
 * leader is not past the target tile, and is only released when non-zero.
 *
 * Field offsets are this package's actor record: 0x08/0x0c/0x10 the 16.16
 * x/y/z triple, 0x22 `flags22`, 0x23 `flags23`, 0x44/0x48 the
 * `state44`/`state48` pair (here 0 and 0x1999 - the same 0x1999 that
 * semantic/overlays/resource_3c8_c_02001d48.c writes), 0x50 the sprite, 0x55
 * `mode55`, 0x59 `state59`.  `(sprite->flags9 << 28) >> 30` extracts bits 2-3
 * as an unsigned 2-bit field - the same two bits the spawner at 0x02000118
 * writes from `params->unk00 & 3`.
 *
 * The final wait loop polls two records returned by Func_02000c5c
 * (their words at +0 are the "still running" counters and their bytes at +99
 * the "finished" flags), stepping one frame per pass through 0x02006fbc.
 *
 * Calls use their resolved resident-service and local-helper identities.  The
 * two sites previously sharing one overlay-relative alias are deliberately
 * distinct here: the first changes actor mode, while the second fetches an
 * actor record.
 */

struct Sprite_02001f60 {
    u8 unknown_00[9];
    u8 flags9;                  /* 0x09 */
};

struct Actor_02001f60 {
    u8 unknown_00[8];
    s32 x;                      /* 0x08 */
    s32 y;                      /* 0x0c */
    s32 z;                      /* 0x10 */
    u8 unknown_14[0x0e];
    u8 flags22;                 /* 0x22 */
    u8 flags23;                 /* 0x23 */
    u8 unknown_24[0x20];
    s32 state44;                /* 0x44 */
    s32 state48;                /* 0x48 */
    u8 unknown_4c[4];
    struct Sprite_02001f60 *sprite;  /* 0x50 */
    u8 unknown_54[1];
    u8 mode55;                  /* 0x55 */
    u8 unknown_56[3];
    u8 state59;                 /* 0x59 */
};

struct Progress_02001f60 {
    s32 running;                /* 0x00 */
    u8 unknown_04[95];
    u8 finished;                /* 0x63 = 99 */
};

/* Four `(x >> 20, z >> 20)` pairs. */
extern s32 Data_0200d128[];

extern u8 Data_0200d488[];
extern u8 Data_0200d508[];
extern u8 Data_0200dd12[];

struct Actor_02001f60 *Func_0808a080();
struct Progress_02001f60 *Func_02000c5c();
s32 Func_02000098();
s32 Func_080770c0();

void Func_0808a018();
void Func_080091c0();
void Func_0808a1e0();
void Func_080091e0();
void Func_0200094c();
void Func_080f9010();
void Func_080090d0();
void Func_0808a020();
void Func_0808a010();
void Func_08009178();
void Func_080770c8();
void Func_080000c0();

void Func_02001f60(void)
{
    struct Actor_02001f60 *leader;
    struct Actor_02001f60 *actor;
    struct Actor_02001f60 *neighbour;
    struct Progress_02001f60 *progressA;
    struct Progress_02001f60 *progressB;
    s32 handle;
    s32 shade;
    s32 match;
    s32 other;
    s32 slot;

    handle = 0;
    leader = Func_0808a080();

    /* No argument register is written here; r0 still holds the pointer just
     * returned above, and that dataflow is preserved as written. */
    Func_0808a018(leader);

    { s32 f1 = 44; s32 g1 = 39; Func_080091c0(108, 39, 13, 7,  f1, g1); }

    for (slot = 9; slot <= 11; slot++) {
        actor = Func_0808a080(slot);

        if (actor->flags23 == 2) {
            Func_080091c0(46, 39, 1, 1, actor->x >> 20, actor->z >> 20);
        } else {
            Func_080091c0(47, 39, 1, 1, actor->x >> 20, actor->z >> 20);
        }

        match = 5;
        if ((actor->x >> 20) == Data_0200d128[0]
            && (actor->z >> 20) == Data_0200d128[1]
            && actor->y >= 0) {
            match = 0;
        } else {
            for (other = 1; other <= 3; other++) {
                if ((actor->x >> 20) == Data_0200d128[other * 2]
                    && (actor->z >> 20) == Data_0200d128[other * 2 + 1]
                    && actor->y >= 0) {
                    match = other;
                    break;
                }
            }
        }
        if (match == 5) {
            continue;
        }

        for (other = 9; other <= 11; other++) {
            neighbour = Func_0808a080(other);
            if (other != slot
                && (actor->x >> 20) == (neighbour->x >> 20)
                && (actor->z >> 20) == (neighbour->z >> 20)) {
                match = 5;
                break;
            }
        }
        if (match == 5) {
            continue;
        }

        shade = (leader->sprite->flags9 << 28) >> 30;

        if ((u32)(leader->z >> 20) <= (u32)Data_0200d128[match * 2 + 1]) {
            handle = Func_02000098(actor->x, actor->y,
                                   actor->z + 0xfffc0000, 20);
            Func_0808a1e0(0, 3);
        }

        /* Only r1 is written at the second site; r0 still carries the
         * record the first call returned. */
        Func_080091e0(Func_0808a080(slot), 0);

        actor->flags22 = 0;
        actor->state48 = 0x1999;
        Func_080091c0(42, 41, 1, 1,
                      Data_0200d128[match * 2], Data_0200d128[match * 2 + 1]);
        actor->mode55 = 3;
        Func_0200094c(actor);
        Func_080f9010(188);
        actor->state44 = 0;
        actor->state59 = 0;
        actor->mode55 = 0;
        actor->y = 0xfff00000;
        Func_0808a1e0(slot, 3);
        actor->flags23 = 2;
        Func_080091c0(46, 39, 1, 1,
                      Data_0200d128[match * 2], Data_0200d128[match * 2 + 1]);
        Func_0808a1e0(0, shade);
        Func_0808a080(0)->flags23 |= 1;

        if (handle != 0) {
            Func_080090d0(handle);
        }

        if (Func_080770c0(772) != 0) {
            Func_0808a020();
            return;
        }

        if ((Func_0808a080(9)->flags23
             & Func_0808a080(10)->flags23
             & Func_0808a080(11)->flags23
             & 2) == 0) {
            continue;
        }

        progressA = Func_02000c5c(888, 680, Data_0200d488);
        progressB = Func_02000c5c(888, 680, Data_0200d508);

        while (progressA->running != 0 || progressB->running != 0) {
            if (progressA->finished != 0 || progressB->finished != 0) {
                Func_0808a010(30);
                Func_080f9010(158);
                Func_08009178(Data_0200dd12, 109, 37);
                { s32 f2 = 45; s32 g2 = 38; Func_080091c0(45, 37, 1, 1,  f2, g2); }

                if ((Func_0808a080(9)->x >> 20) == Data_0200d128[0]
                    && (Func_0808a080(9)->z >> 20) == Data_0200d128[1]) {
                    Func_080770c8(0x302);
                } else {
                    Func_080770c8(0x303);
                }
                Func_080770c8(772);
                break;
            }
            Func_080000c0(1);
        }
    }

    Func_0808a020();
}
