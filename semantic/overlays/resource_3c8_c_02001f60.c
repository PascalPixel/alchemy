typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

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
 * lane's 0x02004bd8 also reads for scene 0xb9; the match index is scaled by 8
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
 * The final wait loop polls two records returned by 0x02002de4 and 0x02002df0
 * (their words at +0 are the "still running" counters and their bytes at +99
 * the "finished" flags), stepping one frame per pass through 0x02006fbc.
 *
 * IMPORT IDENTITIES INSIDE THE OWN BODY.  0x02002124, reached from
 * 0x0200208a, disassembles to the middle of this very routine.  As everywhere
 * in this package an overlay `bl`'s encoded address is a stable identity for
 * the import it reaches after load-time fixup, not a place to disassemble;
 * see the note in resource_3c8_c_020002f0.c and the proof in
 * semantic/overlays/resource_3c8_c_02002f30.c.
 *
 * Thirty-seven `bl` sites reach 36 distinct targets - 0x02006ffa is called
 * once with two arguments and once with one, for its return value - matching
 * the inventory's call count.  The full list: 0x02006e22, 0x02006e10,
 * 0x02006dc4, 0x02006e46, 0x02006df4, 0x02006e0e, 0x02006ee4, 0x02002124,
 * 0x02006ffa, 0x02006f48, 0x02006ee6, 0x02006f06, 0x02002a24, 0x020070da,
 * 0x02007058, 0x02006f46, 0x0200707e, 0x02006fcc, 0x02006f22, 0x02006fb2,
 * 0x02006fe2, 0x02007002, 0x0200700a, 0x02002de4, 0x02002df0, 0x02007040,
 * 0x020071b6, 0x02006fd8, 0x0200700c, 0x0200708a, 0x0200709c, 0x0200707c,
 * 0x02007084, 0x0200708c, 0x02006fbc, 0x020070d0.  Import naming follows the
 * note in resource_3c8_c_020002f0.c.
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

/* Used for their return values. */
struct Actor_02001f60 *Func_02006e22();
struct Actor_02001f60 *Func_02006e46();
struct Actor_02001f60 *Func_02006ee4();
struct Actor_02001f60 *Func_02006fcc();
struct Actor_02001f60 *Func_02006ffa();
struct Actor_02001f60 *Func_02007002();
struct Actor_02001f60 *Func_0200700a();
struct Actor_02001f60 *Func_0200708a();
struct Actor_02001f60 *Func_0200709c();
struct Progress_02001f60 *Func_02002de4();
struct Progress_02001f60 *Func_02002df0();
s32 Func_02002124();
s32 Func_02006fb2();

/* Old-style declarations: the imports' real interfaces are not known here. */
void Func_02006e10();
void Func_02006dc4();
void Func_02006df4();
void Func_02006e0e();
struct Actor_02001f60 *Func_02006f48();
void Func_02006ee6();
void Func_02006f06();
void Func_02002a24();
void Func_020070da();
void Func_02007058();
void Func_02006f46();
void Func_0200707e();
void Func_02006f22();
void Func_02006fe2();
void Func_02007040();
void Func_020071b6();
void Func_02006fd8();
void Func_0200700c();
void Func_0200707c();
void Func_02007084();
void Func_0200708c();
void Func_02006fbc();
void Func_020070d0();

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
    leader = Func_02006e22();

    /* No argument register is written here; r0 still holds the pointer just
     * returned above, and that dataflow is preserved as written. */
    Func_02006e10(leader);

    Func_02006dc4(108, 39, 13, 7, 44, 39);

    for (slot = 9; slot <= 11; slot++) {
        actor = Func_02006e46(slot);

        if (actor->flags23 == 2) {
            Func_02006e0e(46, 39, 1, 1, actor->x >> 20, actor->z >> 20);
        } else {
            Func_02006df4(47, 39, 1, 1, actor->x >> 20, actor->z >> 20);
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
            neighbour = Func_02006ee4(other);
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
            handle = Func_02002124(actor->x, actor->y,
                                   actor->z + 0xfffc0000, 20);
            Func_02006ffa(0, 3);
        }

        /* Only r1 is written at the second site; r0 still carries the
         * record the first call returned. */
        Func_02006ee6(Func_02006f48(slot), 0);

        actor->flags22 = 0;
        actor->mode55 = 3;
        actor->state44 = 0;
        actor->state48 = 0x1999;
        Func_02006f06(42, 41, 1, 1,
                      Data_0200d128[match * 2], Data_0200d128[match * 2 + 1]);
        Func_02002a24(actor);
        Func_020070da(188);
        actor->state59 = 0;
        actor->mode55 = 0;
        actor->y = 0xfff00000;
        Func_02007058(slot, 3);
        actor->flags23 = 2;
        Func_02006f46(46, 39, 1, 1,
                      Data_0200d128[match * 2], Data_0200d128[match * 2 + 1]);
        Func_0200707e(0, shade);
        Func_02006fcc(0)->flags23 |= 1;

        if (handle != 0) {
            Func_02006f22(handle);
        }

        if (Func_02006fb2(772) != 0) {
            Func_02006fe2();
            return;
        }

        if ((Func_02006ffa(9)->flags23
             & Func_02007002(10)->flags23
             & Func_0200700a(11)->flags23
             & 2) == 0) {
            continue;
        }

        progressA = Func_02002de4(888, 680, Data_0200d488);
        progressB = Func_02002df0(888, 680, Data_0200d508);

        while (progressA->running != 0 || progressB->running != 0) {
            if (progressA->finished != 0 || progressB->finished != 0) {
                Func_02007040(30);
                Func_020071b6(158);
                Func_02006fd8(Data_0200dd12, 109, 37);
                Func_0200700c(45, 37, 1, 1, 45, 38);

                if ((Func_0200708a(9)->x >> 20) == Data_0200d128[0]
                    && (Func_0200709c(9)->z >> 20) == Data_0200d128[1]) {
                    Func_0200707c(0x302);
                } else {
                    Func_02007084(0x303);
                }
                Func_0200708c(772);
                break;
            }
            Func_02006fbc(1);
        }
    }

    Func_020070d0();
}
