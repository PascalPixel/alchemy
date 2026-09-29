#include "TASK.H"

/* The scene's tables, which the entry veneers publish to the map engine. */
extern const struct SceneEntrance gKorosseoKabeEntrances[];
extern const u32 gKorosseoKabeExits[];
extern const struct ScenePlacement gKorosseoKabePlacements[];

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gKorosseoKabeEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gKorosseoKabeExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gKorosseoKabePlacements;
}

/*
 * Two bytes, `bx lr', with no prologue and no pool.  Two data-table slots
 * install it as a handler, so the empty body is deliberate rather than
 * padding.
 */
void SceneData_NoOpHandler(void)
{
}

void FieldScene_RunSingleStep(void)
{
    StagedActor_PushActorAhead();
}

void SceneState_ConfigureRegionByActorElevenColumn(void)
{
    u8 *work;
    s32 v0;
    s32 v1;

    work = (u8 *)Engine_ActorGet(11);
    if ((*(s32 *)(work + 8) >> 20) == 36) {
        GameFlag_Set(0x335);
        v0 = 0x23;
        v1 = 0x4D;
        Map_CopyCellAttributes(0x23, 0x4E, 1, 1, v0, v1);
    } else {
        GameFlag_Clear(0x335);
        v0 = 0x23;
        v1 = 0x4D;
        Map_CopyCellAttributes(0x22, 0x4D, 1, 1, v0, v1);
    }
}

void FieldScene_RunTwoStepSequence(void)
{
    StagedActor_PushActorAhead();
    SceneState_ConfigureRegionByActorElevenColumn();
}

void SceneState_StoreSlotTileXToWork832To848(void)
{
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 14;
    s32 sixth = 11;
    s32 *record;
    s32 value;

    Map_CopyCellAttributes(100, 11, 12, 4, fifth, sixth);

    record = (s32 *)Engine_ActorGet(12);
    value = record[2] >> 20;
    GameFlag_SetByte(832, value);
    Map_CopyCellAttributes(71, 16, 1, 1, value, 16);

    record = (s32 *)Engine_ActorGet(13);
    value = record[2] >> 20;
    GameFlag_SetByte(840, value);
    Map_CopyCellAttributes(71, 16, 1, 1, value, 16);

    record = (s32 *)Engine_ActorGet(14);
    value = record[2] >> 20;
    GameFlag_SetByte(848, value);
    Map_CopyCellAttributes(71, 16, 1, 1, value, 16);
}

void SceneState_SetWorkByte35(void)
{
    u8 *record = *(u8 **)gEffectWork;

    record[53] = 1;
}
