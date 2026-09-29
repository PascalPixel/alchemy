#include "HASHIRA.H"
extern u8 MsgFieldFlippedSwitch[];

/* The four background scroll pairs; the shaken copy starts at the second. */
extern u16 gBgScroll[];

void CopyAndOffsetCoordinatePreset(void)
{
    u32 *destination;
    const u32 *source;
    u16 *coordinates;

    source = (const u32 *)&gBgScroll[2];
    destination = (u32 *)TakaraHashira_ShakenScroll;
    *destination++ = *source++;
    *destination++ = *source++;
    *destination = *source;
    coordinates = (u16 *)TakaraHashira_ShakenScroll;
    coordinates[1] += 0xc0;
    coordinates[3] += 0xc0;
    coordinates[5] += 0xc0;
}

void FieldScene_RunScene3b3SequenceA(void)
{
    s32 record;

    record = GameFlag_IsSet(0x200);
    if (record == 0) {
        TakaraHashira_CopyCellBlock(10, 19, 16, 5, record, 10, 31);
        TakaraHashira_CopyCellBlock(10, 51, 16, 5, 1, 10, 31);
        TakaraHashira_CopyCellBlock(42, 51, 16, 5, 2, 10, 31);
    } else {
        TakaraHashira_CopyCellBlock(10, 19, 16, 5, 0, 10, 31);
        TakaraHashira_CopyCellBlock(10, 83, 16, 5, 1, 10, 31);
        TakaraHashira_CopyCellBlock(42, 83, 16, 5, 2, 10, 31);
    }
    TakaraHashira_ShakeChance = 0;
    Engine_TaskAddCallback(CopyAndOffsetCoordinatePreset, 0xc80);
    Stage_Wait(1);
    Runtime_SetIrqHandler(1, 0, TakaraHashira_JitterBackgroundScroll);
    Audio_PlayCue(231);
    TakaraHashira_ShakeChance = 0;
    do {
        Stage_Wait(1);
    } while (++TakaraHashira_ShakeChance <= 100);
    Audio_PlayCue(0x121);
    if (GameFlag_IsSet(0x200) == 0) {
        Map_CopyCellsTo(0, 32, 32, 0, 32, 32);
        Map_CopyCellsTo(32, 32, 64, 0, 32, 32);
    } else {
        Map_CopyCellsTo(0, 64, 32, 0, 32, 32);
        Map_CopyCellsTo(32, 64, 64, 0, 32, 32);
    }
    Stage_Wait(1);
    Runtime_SetIrqHandler(1, 0, 0);
    Stage_Wait(1);
    Call1(Engine_TaskRemoveCallback, (s32)CopyAndOffsetCoordinatePreset);
    Map_Redraw();
    Stage_Wait(30);
}

/* Runs one of two near-identical setup sequences for record REC_ID and
 * records 9-15, chosen by the query call's return value; each sequence ends
 * with its own closing call carrying QUERY_FLAG. */
void FieldScene_RunFlaggedDisplayScene(void)
{
    u32 i;
    u8 *queried;
    u8 *record;

    Event_Begin();
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x1190000, -1, 0x1b00000, 1);
    Camera_WaitForMove();
    Message_ShowCentered((s32)MsgFieldFlippedSwitch, 1);
    queried = GameFlag_IsSet(QUERY_FLAG);
    if (queried == 0) {
        Audio_PlayCue(232);
        Map_AnimateCells(TakaraHashira_ShiftSteps1, 84, 24);
        Event_Wait(30);
        Audio_PlayCue(240);
        Actor_SetSpritePriority(REC_ID, 1);
        /* Flag byte at +85: cleared, since queried is zero here. */
        *((u8 *)Engine_ActorGet(REC_ID) + 85) = queried;
        record = Actor_Get(REC_ID);
        *(s32 *)(record + 12) = -0x200000;
        Actor_SetPosition(REC_ID, 0x1100000, 0x1a00000);
        Actor_SetAnimation(REC_ID, 1);
        Map_AnimateCells(TakaraHashira_ShiftSteps3, 80, 24);
        Map_AnimateCells(TakaraHashira_ShiftSteps5, 80, 28);
        Map_CopyCellsTo(65, 40, 16, 27, 2, 4);
        FieldScene_RunScene3b3SequenceA();
        SceneActor_ApplyPlacementQueryAndTag(9);
        SceneActor_ApplyPlacementQueryAndTag(10);
        SceneActor_ApplyPlacementQueryAndTag(11);
        SceneActor_ApplyPlacementQueryAndTag(12);
        SceneActor_ApplyPlacementQueryAndTag(13);
        SceneActor_ApplyPlacementQueryAndTag(14);
        SceneActor_ApplyPlacementQueryAndTag(15);
        Map_CopyCellAttributes(24, 3, 1, 1, 24, 8);
        GameFlag_Set(QUERY_FLAG);
    } else {
        Audio_PlayCue(232);
        Map_AnimateCells(TakaraHashira_ShiftSteps2, 84, 24);
        Event_Wait(30);
        Audio_PlayCue(230);
        /* Flag byte at +85: cleared unconditionally in this branch. */
        *((u8 *)Engine_ActorGet(REC_ID) + 85) = 0;
        record = Actor_Get(REC_ID);
        *(s32 *)(record + 12) = -0x200000;
        Actor_SetPosition(REC_ID, 0x1100000, 0x1b40000);
        Actor_SetAnimation(REC_ID, 2);
        Map_CopyCellsTo(65, 45, 16, 27, 2, 4);
        Map_AnimateCells(TakaraHashira_ShiftSteps4, 80, 24);
        FieldScene_RunScene3b3SequenceA();
        SceneActor_ApplyPlacementQuery(9);
        SceneActor_ApplyPlacementQuery(10);
        SceneActor_ApplyPlacementQuery(11);
        SceneActor_ApplyPlacementQuery(12);
        SceneActor_ApplyPlacementQuery(13);
        SceneActor_ApplyPlacementQuery(14);
        SceneActor_ApplyPlacementQuery(15);
        Map_CopyCellAttributes(24, 4, 1, 1, 24, 8);
        GameFlag_Clear(QUERY_FLAG);
    }
    Event_End();
}
