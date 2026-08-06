#include "types.h"

/*
 * Resource 3c8 at 0x02002b14: the eight-tile variant of the "take your marks"
 * step - the same construct as semantic/overlays/resource_3c8_c_02001f60.c,
 * run over actor slots 15..18 against the eight-entry position table
 * `Data_0200d164` instead of slots 9..11 against the four-entry
 * `Data_0200d128`.
 *
 * The owner is complete: `push {r5, r6, r7, lr}` plus the high-register saves
 * `mov r7, fp ; mov r6, sl ; mov r5, r9 ; push {r5, r6, r7}` and
 * `mov r7, r8 ; push {r7}` at 0x02002b14, a 16-byte frame, and the matching
 * unwind at 0x02002ea8..0x02002eb8, followed by an alignment halfword and a
 * literal pool at 0x02002eba-0x02002ee3.  No live frame or register state
 * escapes the row; the next prologue is at 0x02002ee4.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.  The early exit at 0x02002d98 skips the loop's own tail
 * call at 0x02007d42 and lands on the same unwind.
 *
 * `Data_0200d164` is the eight-entry `(x >> 20, z >> 20)` table this reconstruction's
 * 0x02004bd8 reads for every scene other than 0xb9; entries are word pairs,
 * so the match index is scaled by 8.  A match additionally requires the
 * actor's `y` to be non-negative.  Index 8 is the "no tile" sentinel and is
 * also written back over a real index when a second actor is found on the
 * same tile.
 *
 * Differences from 0x02001f60 beyond the table and the slot range: the two
 * "nudge the actor behind" scans compare `(actor->z >> 20) - 1` against the
 * neighbour's, and they do not stop at the first match; and the final gate
 * ANDs four `flags23` bytes (slots 15..18) rather than three.
 *
 * The eight bytes at [sp,#0]/[sp,#4] carry arguments five and six of this
 * package's six-argument placement ABI, the order fixed by the byte-exact
 * sibling 0x020010cc.  [sp,#8] holds the leader record fetched once at the
 * top; [sp,#12] holds an optional handle produced only when the leader is not
 * past the target tile, and released unconditionally afterwards.
 *
 * Field offsets are this package's actor record: 0x08/0x0c/0x10 the 16.16
 * x/y/z triple, 0x22 `flags22`, 0x23 `flags23`, 0x44/0x48 the
 * `state44`/`state48` pair (0 and 0x1999), 0x50 the sprite, 0x55 `mode55`,
 * 0x59 `state59`.  `(sprite->flags9 << 28) >> 30` extracts bits 2-3 as an
 * unsigned 2-bit field - the same bits the spawner at 0x02000118 writes from
 * `params->unk00 & 3`.  Constants: 128<<9 = 0x10000, 128<<6 = 0x2000,
 * 216<<16 = 0x00d80000, 158<<18 = 0x02780000, 194<<2 = 776, 190<<2 = 760;
 * 0xfffc0000, 0xfff00000, 0x1999, 0x6666 and 0xccc are pooled.
 *
 * Calls use resolved resident-service and local-helper identities.  Two pairs
 * of sites that formerly shared overlay-relative aliases are intentionally
 * distinct: actor fetch versus placement, and sound cue versus delay.
 */

struct Sprite_02002b14 {
    u8 unknown_00[9];
    u8 flags9;                  /* 0x09 */
};

struct Actor_02002b14 {
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
    struct Sprite_02002b14 *sprite;  /* 0x50 */
    u8 unknown_54[1];
    u8 mode55;                  /* 0x55 */
    u8 unknown_56[3];
    u8 state59;                 /* 0x59 */
};

struct Progress_02002b14 {
    s32 running;                /* 0x00 */
    u8 unknown_04[95];
    u8 finished;                /* 0x63 = 99 */
};

/* Eight `(x >> 20, z >> 20)` pairs. */
extern s32 Data_0200d164[];

extern u8 Data_0200d77c[];
extern u8 Data_0200d7c8[];
extern u8 Data_0200dac8[];
extern u8 Data_0200dd3c[];

struct Actor_02002b14 *Func_0808a080();
struct Progress_02002b14 *Func_02000c5c();
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
void Func_0808a208();
void Func_0808a220();
void Func_0808a218();
void Func_0808a010();
void Func_0808a210();
void Func_08009148();
void Func_08009098();
void Func_08009178();
void Func_080770c8();
void Func_080000c0();

void Func_02002b14(void)
{
    struct Actor_02002b14 *leader;
    struct Actor_02002b14 *actor;
    struct Actor_02002b14 *neighbour;
    struct Progress_02002b14 *progressA;
    struct Progress_02002b14 *progressB;
    s32 handle;
    s32 shade;
    s32 match;
    s32 other;
    s32 slot;

    handle = 0;
    leader = Func_0808a080(0, 0);

    /* No argument register is written here; r0 still holds the pointer just
     * returned above, and that dataflow is preserved as written. */
    Func_0808a018(leader);

    { s32 f1 = 5; s32 g1 = 48; Func_080091c0(69, 48, 4, 2,  f1, g1); }
    { s32 f2 = 9; s32 g2 = 37; Func_080091c0(73, 37, 9, 13,  f2, g2); }

    for (slot = 15; slot <= 18; slot++) {
        actor = Func_0808a080(slot);

        if (actor->flags23 == 2) {
            Func_080091c0(73, 48, 1, 1, actor->x >> 20, actor->z >> 20);
        } else {
            Func_080091c0(72, 48, 1, 1, actor->x >> 20, actor->z >> 20);
        }

        match = 8;
        if ((actor->x >> 20) == Data_0200d164[0]
            && (actor->z >> 20) == Data_0200d164[1]
            && actor->y >= 0) {
            match = 0;
        } else {
            for (other = 1; other <= 7; other++) {
                if ((actor->x >> 20) == Data_0200d164[other * 2]
                    && (actor->z >> 20) == Data_0200d164[other * 2 + 1]
                    && actor->y >= 0) {
                    match = other;
                    break;
                }
            }
        }
        if (match == 8) {
            continue;
        }

        for (other = 15; other <= 18; other++) {
            neighbour = Func_0808a080(other);
            if (other != slot
                && (actor->x >> 20) == (neighbour->x >> 20)
                && (actor->z >> 20) == (neighbour->z >> 20)) {
                match = 8;
                break;
            }
        }
        if (match == 8) {
            continue;
        }

        shade = (leader->sprite->flags9 << 28) >> 30;

        if ((u32)(leader->z >> 20) <= (u32)Data_0200d164[match * 2 + 1]) {
            handle = Func_02000098(actor->x, actor->y,
                                   actor->z + 0xfffc0000, 20);
            Func_0808a1e0(0, 3);
        }

        for (other = 15; other <= 18; other++) {
            neighbour = Func_0808a080(other);
            if (other != slot
                && (actor->x >> 20) == (neighbour->x >> 20)
                && ((actor->z >> 20) - 1) == (neighbour->z >> 20)) {
                Func_0808a1e0(other, 3);
            }
        }

        /* Only r1 is written at the second site; r0 still carries the record
         * the first call returned. */
        Func_080091e0(Func_0808a080(slot), 0);

        actor->flags22 = 0;
        actor->mode55 = 3;
        actor->state48 = 0x1999;
        actor->state44 = 0;
        Func_080091c0(6, 44, 1, 1,
                      Data_0200d164[match * 2], Data_0200d164[match * 2 + 1]);
        Func_0200094c(actor);
        Func_080f9010(188);
        actor->state59 = 0;
        actor->mode55 = 0;
        actor->y = 0xfff00000;
        Func_0808a1e0(slot, 3);
        actor->flags23 = 2;
        Func_080091c0(73, 48, 1, 1,
                      Data_0200d164[match * 2], Data_0200d164[match * 2 + 1]);
        Func_0808a1e0(0, shade);
        Func_0808a080(0)->flags23 |= 1;

        for (other = 15; other <= 18; other++) {
            neighbour = Func_0808a080(other);
            if (other != slot
                && (actor->x >> 20) == (neighbour->x >> 20)
                && ((actor->z >> 20) - 1) == (neighbour->z >> 20)) {
                Func_0808a1e0(other, 1);
                Func_0808a080(other)->flags23 |= 1;
            }
        }

        Func_080090d0(handle);

        if (Func_080770c0(776) != 0) {
            Func_0808a020();
            return;
        }

        if ((Func_0808a080(15)->flags23
             & Func_0808a080(16)->flags23
             & Func_0808a080(17)->flags23
             & Func_0808a080(18)->flags23
             & 2) == 0) {
            continue;
        }

        Func_0808a208(0x10000, 0x2000);
        Func_0808a220(14, 1);
        Func_0808a218();

        progressA = Func_02000c5c(136, 776, Data_0200d77c);
        Func_0808a010(30);
        Func_0808a208(0x6666, 0xccc);
        Func_0808a210(0x00d80000, -1, 0x02780000, 1);
        Func_08009148(progressA);
        Func_08009098(progressA, Data_0200d7c8);
        progressB = Func_02000c5c(216, 760, Data_0200dac8);

        while (progressA->running != 0 || progressB->running != 0) {
            if (progressA->finished != 0 || progressB->finished != 0) {
                Func_0808a010(30);
                Func_08009178(Data_0200dd3c, 77, 35);
                { s32 f3 = 13; s32 g3 = 36; Func_080091c0(13, 35, 1, 1,  f3, g3); }
                Func_080770c8(776);
                break;
            }
            Func_080000c0(1);
        }
    }

    Func_0808a020();
}
