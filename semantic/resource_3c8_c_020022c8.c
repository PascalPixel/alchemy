#include "types.h"

/*
 * Resource 3c8 at 0x020022c8: drop two actors onto the heights their kind
 * words select, then flag every party member that is hanging below the
 * floor.
 *
 * The owner is complete: `push {r5, r6, r7, lr}` plus the high-register save
 * `mov r7, sl ; mov r6, r9 ; mov r5, r8 ; push {r5, r6, r7}` at 0x020022c8,
 * an 8-byte frame, and the matching unwind at 0x020023b8..0x020023c6,
 * followed by a four-word literal pool at 0x020023c8-0x020023d7.  No live
 * frame or register state escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * The incoming r0 is kept in r9 for the whole body and gates three things: an
 * extra service call before the drop, one after it, and the final call.  It
 * is a boolean-style flag; nothing reads it other than `!= 0` and the final
 * pass-through.
 *
 * `Data_0200d148` is an in-image table of 16.16 heights indexed by the actors'
 * signed kind halfword at +100 - the same field the byte-exact siblings
 * 0x02000594 and 0x020022a4 read, and the same `<< 2` scaling.  At this
 * overlay's proven 0x02008000 link base the table sits at file offset
 * 0x5148.  It is read four times, twice per actor: once to feed the placement
 * calls at 0x0200712e / 0x02007142 and once to store into the actor's +0x0c.
 *
 * The trailing sweep runs the unsigned range 0..4 and asks 0x02007224 for the
 * record with index + 8 - i.e. party slots 8..12, the same base offset the
 * byte-exact sibling 0x0200032c uses for its slot array.  The height test is
 * `y / 0x10000` written out as `if (y < 0) y += 0xffff ; y >>= 16` (pooled
 * 0x0000ffff), and the actor is reported only when that quotient is strictly
 * between -30 and 0.  The reported coordinates are `x >> 20` and `z >> 20`,
 * this package's grid conversion, passed as arguments five and six of the
 * six-argument placement ABI proven by the byte-exact sibling 0x020010cc.
 *
 * All thirteen call sites use their resolved resident-service identities.
 */

struct Actor_020022c8 {
    u8 unknown_00[8];
    s32 x;                      /* 0x08 */
    s32 y;                      /* 0x0c */
    s32 z;                      /* 0x10 */
    u8 unknown_14[0x50];
    s16 kind;                   /* 0x64 = 100 */
};

/* In-image height table, indexed by an actor's kind halfword. */
extern s32 Data_0200d148[];

/*
 * CALL SYMBOLS ARE PER-SITE: the raw assembly's thirteen `bl`s are direct
 * relocations against absolute .set symbols (verified via
 * `arm-none-eabi-objdump -dr -M force-thumb` on the assembled .o) -- lifted
 * verbatim below by call site, not the veneer-math final target names this
 * file used before (the same resolved import gets a different sub_ symbol
 * at each of its call sites).
 */
struct Actor_020022c8 *Func_02007186();
struct Actor_020022c8 *Func_0200718e();
struct Actor_020022c8 *Func_02007224();
void Func_020071a6();
void Func_020071b4();
void Func_02007308();
void Func_0200736a();
void Func_0200712e();
void Func_02007142();
void Func_02007230();
void Func_02007236();
void Func_020071de();
void Func_02007242();

void Func_020022c8(s32 announce)
{
    struct Actor_020022c8 *first;
    struct Actor_020022c8 *second;
    struct Actor_020022c8 *member;
    u32 slot;
    s32 height;

    first = Func_02007186(8);
    second = Func_0200718e(9);

    Func_020071a6(8, 0x8000, 0x4000);
    Func_020071b4(9, 0x8000, 0x4000);

    if (announce != 0) {
        Func_02007308(180);
    }

    Func_0200712e(first, first->x, Data_0200d148[first->kind], first->z);
    Func_02007142(second, second->x, Data_0200d148[second->kind], second->z);

    Func_02007230(8);
    Func_02007236(9);

    first->y = Data_0200d148[first->kind];
    second->y = Data_0200d148[second->kind];

    if (announce != 0) {
        Func_0200736a(0x121);
    }

    for (slot = 0; slot <= 4; slot++) {
        member = Func_02007224(slot + 8);
        height = member->y / 0x10000;
        if (height < 0 && height > -30) {
            Func_020071de(4, 19, 1, 1, member->x >> 20, member->z >> 20);
        }
    }

    Func_02007242(announce);
}
