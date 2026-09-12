#include "types.h"

#define SceneActor_PassOffsetPointOfActorZero Func_0200103c
#define SceneActor_MarkActorThirteenTileAndPark Func_02001b84
s32 *Func_02003e00(s32 a);
void Func_02001d38(s32 *p);
void Func_02004932();
s32 *Func_02004950();
s32 *Func_02004958();
s32 Func_02001dfa();
s32 Func_02001e0e();
s32 Func_02001e20();
s32 Func_02001e32();
s32 Func_02001e44();
s32 *Func_020049ca();
void Func_020049cc();

/* The five tile-painting calls take (layer, x, z, width, height, value) and
 * all reach the same routine, but each keeps its own call word: the encoding
 * is per site, so they must not be collapsed onto one alias. */

void SceneActor_PassOffsetPointOfActorZero(void)
{
    s32 v[3];
    s32 *p = Func_02003e00(0);

    v[0] = (p[2] & 0xfff00000) + 0x80000;
    v[1] = p[3];
    v[2] = (p[4] & 0xfff00000) + 0xffe80000;
    Func_02001d38(v);
}

/*
 * Parking step for actor slot 13 in resource_39f. It marks the actor's own
 * tile 0xff, clears the four orthogonally adjacent tiles, and once the actor
 * stands on tile (45, 6) clears the record's mode byte and writes -2.0 in
 * 16.16 into the words at +12 and +20.
 *
 * The 176-byte owner at 0x02001b84 runs past its code to include an alignment
 * halfword and the pool word 0xfffe0000 at 0x02001c30.
 */
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
