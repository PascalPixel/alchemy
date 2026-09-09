#include "types.h"

/*
 * Parking step for actor slot 13 in resource_39f. It marks the actor's own
 * tile 0xff, clears the four orthogonally adjacent tiles, and once the actor
 * stands on tile (45, 6) clears the record's mode byte and writes -2.0 in
 * 16.16 into the words at +12 and +20.
 *
 * The 176-byte owner at 0x02001b84 runs past its code to include an alignment
 * halfword and the pool word 0xfffe0000 at 0x02001c30.
 */

void Func_02004932();
s32 *Func_02004950();
s32 *Func_02004958();
/* The five tile-painting calls take (layer, x, z, width, height, value) and
 * all reach the same routine, but each keeps its own call word: the encoding
 * is per site, so they must not be collapsed onto one alias. */
s32 Func_02001dfa();
s32 Func_02001e0e();
s32 Func_02001e20();
s32 Func_02001e32();
s32 Func_02001e44();
s32 *Func_020049ca();
void Func_020049cc();

void SceneActor_MarkActorThirteenTileAndPark(void)
{
    s32 x;
    s32 z;

    /* No argument register is written before this branch. */
    Func_02004932();

    x = Func_02004950(13)[2] >> 20;
    z = Func_02004958(13)[4] >> 20;

    Func_02001dfa(2, x, z, 1, 1, 0xff);
    Func_02001e0e(2, x + 1, z, 1, 1, 0);
    Func_02001e20(2, x - 1, z, 1, 1, 0);
    Func_02001e32(2, x, z + 1, 1, 1, 0);
    Func_02001e44(2, x, z - 1, 1, 1, 0);

    if (x == 45 && z == 6) {
        u8 *record = (u8 *)Func_020049ca(13);

        record[85] = 0;
        *(s32 *)(record + 20) = (s32)0xfffe0000;
        *(s32 *)(record + 12) = (s32)0xfffe0000;
    }

    /* Common exit; no argument registers are set. */
    Func_020049cc();
}
