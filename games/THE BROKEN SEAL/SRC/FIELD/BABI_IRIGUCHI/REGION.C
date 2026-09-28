#include "IRIGUCHI.H"

void SceneState_ConfigureRegion82_7AndApply768(void)
{
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a = 18;
    s32 b = 7;

    Iriguchi_CopyCellAttributes(82, 7, 1, 2, a, b);
    Iriguchi_TaskWait(1);
    GameFlag_Set(768);
}

void SceneState_ApplyRectsAtActors8And9(void)
{
    s32 *p = Actor_Get(8);

    Actor_SetSpritePriority(8, 1);
    Actor_SetSpritePriority(9, 1);
    {
        s32 k5 = 5, k6 = 19;

        Iriguchi_CopyCellAttributes(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = 17, k6 = 19;

        Iriguchi_CopyCellAttributes(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = p[2] >> 20, k6 = p[4] >> 20;

        Iriguchi_CopyCellAttributes(3, 3, 1, 1, k5, k6);
    }
    {
        s32 *q = Actor_Get(9);
        s32 k5 = q[2] >> 20, k6 = q[4] >> 20;

        Iriguchi_CopyCellAttributes(3, 3, 1, 1, k5, k6);
    }
}
