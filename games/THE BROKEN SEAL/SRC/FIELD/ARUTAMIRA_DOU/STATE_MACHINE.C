#include "ARUTAMIRA.H"

void SceneActor_SetPositionFromTransformedBase(s32 a, s32 b, s32 c)
{
    s32 k1 = 0x1f80000;
    s32 k2 = 0x180000;
    s32 k3 = 0x900000;
    u8 *obj = (u8 *)Engine_ActorGet(a);
    s32 buf[3];
    s32 *bp = buf;

    bp[0] = k1;
    bp[2] = k2;
    Vector_AddPolarOffset(b, c, bp);
    *(s32 *)(obj + 8) = bp[0];
    *(s32 *)(obj + 12) = bp[2];
    *(s32 *)(obj + 16) = k3;
}

/* Contiguous unnamed state-owner run for resource_3bd. */
void SceneActor_PlaceFiveActorsInRow(u8 *p)
{
    s32 i = 0;

    do {
        SceneActor_SetPositionFromTransformedBase(i + 11, 0x180000, p);
        p -= 13107;
        i++;
    } while (i <= 4);
}
