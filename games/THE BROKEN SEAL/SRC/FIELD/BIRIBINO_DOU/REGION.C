#include "REGION.H"

/* The cave's map regions and its gate, switch and statue scenes, up to the
 * overlay's exported entry. */

void SceneState_ConfigureRegion1_0_21x14(void)
{
    s32 w = 21;
    s32 h = 14;

    Map_CopyCellAttributes(1, 0, 1, 1, w, h);
}

void SceneState_ConfigureRegion0_0_21x14(void)
{
    s32 w = 21;
    s32 h = 14;

    Map_CopyCellAttributes(0, 0, 1, 1, w, h);
}

void SceneState_ApplyTwoRects(void)
{
    {
        s32 a5 = 1;
        s32 a6 = 3;

        Map_CopyCellsTo(111, 37, 97, 21, a5, a6);
    }
    {
        s32 a5 = 32;
        s32 a6 = 24;

        Map_CopyCellAttributes(46, 38, 3, 2, a5, a6);
    }
}

void FieldScene_RunTwoLayoutSteps(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 3;

        Map_CopyCellsTo(95, 21, 97, 21, fifth, sixth);
    }
    {
        s32 fifth = 32;
        s32 sixth = 25;

        Map_CopyCellAttributes(46, 38, 3, 1, fifth, sixth);
    }
}

void FieldScene_RunActor9Flag882Scene(void)
{
    Event_Begin();
    Actor_SetPosition(9, 0, 0);
    GameFlag_Set(0x882);
    Event_End();
}

void FieldScene_RunScene398SequenceA(void)
{
    Event_Begin();
    Actor_SetPosition(8, 0, 0);
    GameFlag_Set(0x883);
    Event_Wait(40);
    Actor_SetAnimationAndWait(15, 2);
    Actor_Get(15)->motion_flags = 0;
    Actor_Get(15)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpritePriority(15, 2);
    Map_CopyCellAttributes(0, 0, 1, 1, 18, 14);
    Event_End();
}

void FieldScene_RunActorFifteenScene(void)
{
    void Audio_PlayCue(s32);

    Event_Begin();
    Actor_SetChildValue(0xF, 0);
    Event_Wait(0x28);
    Audio_PlayCue(0xD2);
    Actor_SetAnimationAndWait(0xF, 6);
    Event_End();
}

void FieldScene_RunActorSixteenScene(void)
{
    void Event_End(void);
    void Audio_PlayCue(s32);

    Event_Begin();
    Actor_SetChildValue(0x10, 0);
    Event_Wait(0x28);
    Audio_PlayCue(0xD2);
    Actor_SetAnimationAndWait(0x10, 6);
    Event_End();
}

void FieldScene_RunActor17Steps28AndD2(void)
{
    void Event_End(void);

    Event_Begin();
    Actor_SetChildValue(0x11, 0);
    Event_Wait(0x28);
    Audio_PlayCue(0xD2);
    Actor_SetAnimationAndWait(0x11, 6);
    Event_End();
}

void FieldScene_RunScene398SequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 v5;

    rec7 = Engine_ActorGet(11);
    rec8 = Actor_Get(12);
    if ((*(s32 *)(rec7 + 8) >> 20) == 35) {
        if ((*(s32 *)(rec7 + 16) >> 20) != 23) {
            goto L_02000330;
        }
        GameFlag_Set(0x303);
    } else {
        L_02000330:;
        GameFlag_Clear(0x303);
    }
    if ((*(s32 *)(rec8 + 8) >> 20) == 35) {
        if ((*(s32 *)(rec8 + 16) >> 20) != 23) {
            goto L_02000350;
        }
        GameFlag_Set(0x304);
    } else {
        L_02000350:;
        GameFlag_Clear(0x304);
    }
    if (GameFlag_IsSet(0x303) == 0) {
        record = GameFlag_IsSet(0x304);
        if (record == 0) {
            goto L_020003c2;
        }
    }
    if (GameFlag_IsSet(0x302) == 0) {
        Event_Begin();
        Event_Wait(40);
        Audio_PlayCue(210);
        v5 = 36;
        Actor_SetAnimationAndWait(17, 6);
        Map_CopyCellAttributes(0, 1, 1, 1, v5, 22);
        Map_CopyCellAttributes(0, 2, 1, 1, v5, 24);
        Event_End();
    }
    GameFlag_Set(0x302);
    goto L_02000414;
    L_020003c2:;
    if (GameFlag_IsSet(0x302) != 0) {
        Event_Begin();
        Event_Wait(40);
        Audio_PlayCue(220);
        v5 = 36;
        Actor_SetAnimationAndWait(17, 2);
        Map_CopyCellAttributes(1, 1, 1, 1, v5, 22);
        Map_CopyCellAttributes(1, 2, 1, 1, v5, 24);
        Event_End();
    }
    GameFlag_Clear(0x302);
    L_02000414:;
}

void ActorPresentation_SetSceneCell31AndFlag305(void)
{
    s32 width = 8;
    s32 height = 13;

    Map_CopyCellAttributes(31, 0, 1, 1, width, height);
    GameFlag_Set(0x305);
}

void SceneState_SetGlobalByte17(void)
{
    FIELD_AT_OFFSET(*(void **)&gMapWork, s8 *, 0x17) = 1;
}

void SceneState_ClearRuntimeByte17(void)
{
    FIELD_AT_OFFSET(*(void **)&gMapWork, s8 *, 0x17) = 0;
}
