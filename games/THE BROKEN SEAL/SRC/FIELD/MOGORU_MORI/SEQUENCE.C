#include "MORI.H"

void SceneActor_BobActorZeroWhenTargetClear(void)
{
    u8 *record;
    u8 *mode;
    u8 saved;
    s32 target[3];

    record = Actor_Get(ACTOR_PARTY_LEADER);
    mode = record + 85;
    saved = *mode;

    target[0] = *(s32 *)((u8 *)Object_GetById(0) + 8) + (s32)0xffe00000;
    target[1] = *(s32 *)((u8 *)Object_GetById(0) + 12);
    target[2] = *(s32 *)((u8 *)Object_GetById(0) + 16);

    if (SceneActor_TryRunSlotZeroMoveStep(target)!= 0) {
        /* r0 still holds the nonzero result of the test above. */
        Event_Begin();

        *mode = 0;
        Object_SetModeById(11, 7);

        *(s32 *)(record + 12) += (s32)0xffff0000;
        *(s32 *)(record + 20) += (s32)0xffff0000;
        WaitFrames(2);

        *(s32 *)(record + 12) += (s32)0xffff0000;
        *(s32 *)(record + 20) += (s32)0xffff0000;
        WaitFrames(10);

        *(s32 *)(record + 12) += 0x10000;
        *(s32 *)(record + 20) += 0x10000;
        WaitFrames(4);

        *(s32 *)(record + 12) += 0x10000;
        *(s32 *)(record + 20) += 0x10000;

        *mode = saved;
        Event_End();
    }
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
    Event_Begin();

    x = ((s32 *)Object_GetById(13))[2] >> 20;
    z = ((s32 *)Object_GetById(13))[4] >> 20;

    StagedActor_FillGridAttributeRectangle(2, x, z, 1, 1, 0xff);
    StagedActor_FillGridAttributeRectangle(2, x + 1, z, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x - 1, z, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x, z + 1, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x, z - 1, 1, 1, 0);

    if (x == 45 && z == 6) {
        u8 *record = (u8 *)Actor_Get(13);

        record[85] = 0;
        *(s32 *)(record + 20) = (s32)0xfffe0000;
        *(s32 *)(record + 12) = (s32)0xfffe0000;
    }

    /* Common exit; no argument registers are set. */
    Event_End();
}

void FieldScene_RunSupplementalSequenceOne(void)
{

    s32 x;
    s32 y;
    u8 *record;

    Event_Begin();
    record = Object_GetById(14);
    x = *(s32 *)(record + 8);
    record = Object_GetById(14);
    y = *(s32 *)(record + 16);
    x >>= 20;
    y >>= 20;
    StagedActor_FillGridAttributeRectangle(2, x, y, 1, 1, 255);
    StagedActor_FillGridAttributeRectangle(2, x + 1, y, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x - 1, y, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x, y + 1, 1, 1, 0);
    StagedActor_FillGridAttributeRectangle(2, x, y - 1, 1, 1, 0);
    record = Object_GetById(14);
    if ((*(s32 *)(record + 16) >> 20) == 27) {
        record = Object_GetById(14);
        record[85] = 0;
        *(s32 *)(record + 20) = -0x20000;
        *(s32 *)(record + 12) = -0x20000;
        GameFlag_Set(0x214);
        StagedActor_FillGridAttributeRectangle(2, 43, 23, 1, 1, 255);
    }
    Event_End();
}
