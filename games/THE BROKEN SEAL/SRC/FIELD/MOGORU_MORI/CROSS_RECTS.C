#include "MORI.H"

void SceneState_ApplyCrossRectsAroundActor11(void)
{
    s32 x;
    s32 z;

    /* No argument register is written before this branch. */
    Event_Begin();

    /* Both coordinates are 16.16 fixed point reduced to whole tiles with
     * `asrs #20`, i.e. 16 fractional bits plus a 16-unit tile pitch. */
    x = ((s32 *)Object_GetById(11))[2] >> 20;
    z = ((s32 *)Object_GetById(11))[4] >> 20;

    StagedActor_FillGridAttributeRectangle(2, x, z, 1, 1, 0xff);
    StagedActor_FillGridAttributeRectangle(2, x + 1, z, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x - 1, z, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x, z + 1, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x, z - 1, 1, 1, 0);

    if (x == 36 && z == 24) {
        u8 *p = (u8 *)Actor_Get(11);

        p[85] = 0;
        *(s32 *)(p + 20) = (s32)0xfffe0000;
        *(s32 *)(p + 12) = (s32)0xfffe0000;
    }

    /* Common exit; no argument registers are set. */
    Event_End();
}
