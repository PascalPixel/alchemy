#include "types.h"

/*
 * Resource 378 owner at 0x02003334 (296 bytes).
 *
 * Complete owner: `push {lr}` at 0x02003334 and `pop {r0} ; bx r0` at
 * 0x02003440.  The popped register IS r0, so the return value does not
 * survive the epilogue and the owner is **void**.  0x02003334 + 296 =
 * 0x0200345c, where the next owner's `push {lr}` begins; the span decomposes
 * as head and dispatcher (0x02003334-0x02003363), 41-entry jump table
 * (0x02003364-0x02003407, data), case bodies and shared tail
 * (0x02003408-0x02003443) and the literal pool (0x02003444-0x0200345b).
 *
 * Second of the nine dispatch sites the tree described nowhere, adopted with
 * `--span 296` because discovery never seeded it.
 *
 * Same selector as 0x0200014c and `resource_371:037c` -- `Data_02000240[225]`,
 * `subs #10 ; cmp #40 ; bhi` for the range 10..50 -- and a DIFFERENT table:
 * 10,12 -> 0x02003408; 11 -> 0x02003422; 20,21,50 -> 0x0200342a; everything
 * else to the default at 0x02003434.  Read entry by entry; the family
 * resemblance was not taken as evidence for any of it.
 *
 * The default arm is also the shared tail: the two arms that fall out of
 * `0x02003408` and `0x02003422` branch INTO 0x02003434, which is what the
 * `bhi` targets, so the tail statements sit after the switch and those arms
 * `break`.  The 20/21/50 arm instead branches to the epilogue and returns.
 *
 * Case-arm ORDER off the ROM (§5b5); per-site RAW callee names (§5b3a).
 */

extern s16 Data_02000240[];

s32 Func_020065e0();
void Func_02006a6a();
void Func_02006938();
s32 Func_020069dc();
void Func_02006abe();
void Func_02006ac6();
void Func_02006ace();
void Func_02006a24();
void Func_02003790();
void Func_02006af2();
void Func_02006a36();

void Func_02003334(void)
{
    s16 *table;
    s32 scene;

    if (Func_020065e0() != 0) {
        Func_02006a6a(8);
        return;
    }

    Func_02006938();

    table = Data_02000240;
    scene = table[225];

    switch (scene) {
    case 10:
    case 12:
        if (Func_020069dc(0x855) != 0) {
            Func_02006abe(0x1376);
        } else {
            Func_02006ac6(0x1288);
        }
        break;
    case 11:
        Func_02006ace(0x1ce8);
        break;
    case 20:
    case 21:
    case 50:
        Func_02006a24();
        Func_02003790();
        return;
    default:
        break;
    }

    Func_02006af2(8, 0);
    Func_02006a36();
}
