/* The four-step actor motion, actor placement and the opening sequence. */
#include "LOG_ROLLING.H"
extern u8 MsgKorosseoAnotherLogRollingArea[];
extern u8 MsgKorosseoCalledMovingSidewalkStage[];
extern u8 MsgKorosseoClearStageMustAbleChange[];
extern u8 MsgKorosseoHereMustFigureOutHow[];
extern u8 MsgKorosseoInStageMustTryOutpace[];
extern u8 MsgKorosseoPlaceNormallyCalledFreeClimb[];

void FieldScene_RunFourStepActorMotion(s32 a0)
{
    extern void Korosseo_FinishSoloRound();
    extern s32 FieldScene_RunMiddleSequence();
    extern s32 Korosseo_FadeInCompetitor();
    extern void Korosseo_RestoreCompetitor();
    extern void Object_SetPosition();
    extern void Object_CommitPosition();
    extern void battle_owner_69();

    s32 result;
    s32 actor;
    s32 x;
    s32 y;
    s32 raised_y;
    s32 shifted_x;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Event_Begin();
        result = Value2(ColossoLogRollingStage_RunStateInteraction, a0, 4);
        if (result == 0) {
            Event_SetMessage((s32)MsgKorosseoPlaceNormallyCalledFreeClimb);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x3580000, -1, 0xa80000, 1);
            Camera_WaitForMove();
            Event_Wait(30);
            Event_ShowMessage(a0, 0);
            Event_ShowMessage(a0, 0);
            Value3(Korosseo_FadeInCompetitor, 0, 0x330, 200);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x348, 200);
            Value3(Engine_ActorFaceDirection, 0, 0xc000, 20);
            battle_owner_69();
            Camera_MoveTo(-1, -1, -1, 0);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
            actor = Value1(Engine_ActorGet, 0);
            y = *(s32 *)(actor + 12);
            x = *(s32 *)(actor + 8);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 10);
            raised_y = 0x60000 + y;
            Object_SetPosition(actor, x, raised_y, *(s32 *)(actor + 16));
            Object_CommitPosition(actor);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 14);
            shifted_x = 0x400000 + x;
            Object_SetPosition(actor, shifted_x, raised_y, *(s32 *)(actor + 16));
            Object_CommitPosition(actor);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 10);
            Object_SetPosition(actor, shifted_x, y + 0x360000, *(s32 *)(actor + 16));
            Object_CommitPosition(actor);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 15);
            Object_SetPosition(actor, x + 0x300000, y + 0x360000, *(s32 *)(actor + 16));
            Object_CommitPosition(actor);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 12);
            Event_ShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
            ColossoLogRollingStage_InitializeStateInteraction(a0, 4);
        } else if (result == 1) {
            Event_SetMessage((s32)MsgKorosseoClearStageMustAbleChange);
            Event_ShowMessage(a0, 0);
        }
        Value3(FieldScene_RunMiddleSequence, result, a0, 4);
        Event_End();
    }
}

void ColossoLogRollingStage_PositionActor(s32 selector, s32 x, s32 z)
{
    extern s32 *ObjectTable_Get();
    extern void ObjectDispatch_InitFromTable6();
    extern void Object_SetPosition();

    s32 *record;

    record = ObjectTable_Get(selector);
    if (record != 0) {
        ObjectDispatch_InitFromTable6();
        Object_SetAnimation(record, 5);
        Object_SetPosition(record, x << 16, record[3], z << 16);
    }
}

void ColossoLogRollingStage_PositionAndActivateActor(s32 selector, s32 x, s32 z)
{
    extern s32 *ObjectTable_Get();
    extern void ObjectDispatch_InitFromTable6();
    extern void Object_SetPosition();
    extern void Object_CommitPosition();

    s32 *record;

    record = ObjectTable_Get(selector);
    if (record != 0) {
        ObjectDispatch_InitFromTable6();
        Object_SetAnimation(record, 5);
        Object_SetPosition(record, x << 16, record[3], z << 16);
        Object_CommitPosition(record);
        Object_SetAnimation(record, 1);
    }
}

void FieldScene_RunOpeningAuxiliarySequence(s32 a0)
{
    extern void Korosseo_FinishSoloRound();
    extern s32 FieldScene_RunMiddleSequence();
    extern s32 Korosseo_FadeInCompetitor();
    extern void Korosseo_RestoreCompetitor();

    s32 i;
    s32 rec2;
    s32 rec7;
    s32 record;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Event_Begin();
        rec2 = Value2(ColossoLogRollingStage_RunStateInteraction, a0, 5);
        if (rec2 != 0) {
        } else {
            Event_SetMessage((s32)MsgKorosseoCalledMovingSidewalkStage);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x4380000, -1, 0xa80000, 1);
            Camera_WaitForMove();
            Event_Wait(30);
            Event_ShowMessage(a0, 0);
            Event_ShowMessage(a0, 0);
            Value3(Korosseo_FadeInCompetitor, 0, 0x3d8, 184);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
            ColossoLogRollingStage_PositionAndActivateActor(0, 0x3e0, 184);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x4ccc, 0x2666);
            Call3(ColossoLogRollingStage_PositionActor, 0, 0x460, 184);
            Event_Wait(120);
            Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
            Event_Wait(120);
            ColossoLogRollingStage_ResetActorMotion(0);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
            Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
            rec7 = Engine_ActorGet(0);
            for (i = 119; i >= 0; i--) {
                if (*(s32 *)(rec7 + 8) > 0x3e00000) {
                    *(s32 *)(rec7 + 8) += -0x13333;
                }
                Task_Wait(1);
            }
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x103, 60);
            Value3(ColossoLogRollingStage_PositionActor, 0, 0x460, 184);
            Event_ShowMessage(a0, 0);
            ColossoLogRollingStage_ResetActorMotion(0);
            {
                u8 *flag = (u8 *)&gGameState;

                flag[498] = 1;
            }
            Korosseo_RestoreCompetitor(0);
            Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
            ColossoLogRollingStage_InitializeStateInteraction(a0, 5);
            goto L_02002494;
        }
        if (rec2 == 1) {
            Event_SetMessage((s32)MsgKorosseoInStageMustTryOutpace);
            Event_ShowMessage(a0, 0);
        }
        L_02002494:;
        Value3(FieldScene_RunMiddleSequence, rec2, a0, 5);
        Event_End();
    }
}

void ColossoLogRollingStage_RunLogRollingInteraction(s32 actor)
{
    extern void Korosseo_FinishSoloRound(void);

    s32 state;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
        return;
    }

    Event_Begin();
    state = ColossoLogRollingStage_RunStateInteraction(actor, 6);

    if (state == 0) {
        Event_SetMessage((s32)MsgKorosseoAnotherLogRollingArea);
        Camera_SetSpeed(0x30000, 0x6000);
        Camera_MoveTo(0x5080000, -1, 0x980000, 1);
        Camera_WaitForMove();
        Event_Wait(30);
        Event_ShowMessage(actor, 0);
        ColossoLogRollingStage_StartPaletteTask(0xb4, 0x58, 0);
        Event_Wait(60);
        Event_ShowMessage(actor, 0);
        ColossoLogRollingStage_StartPaletteTaskFromState(0x20, 0x54, 10);
        Event_Wait(30);
        Event_ShowMessage(actor, 0);
        ColossoLogRollingStage_StartPaletteTaskFromState(0x60, 0x54, 30);
        Event_Wait(60);
        Event_ShowMessage(actor, 0);
        ColossoLogRollingStage_StopPaletteTask();
        Event_Wait(2);
        Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
        ColossoLogRollingStage_InitializeStateInteraction(actor, 6);
    } else if (state == 1) {
        Event_SetMessage((s32)MsgKorosseoHereMustFigureOutHow);
        Event_ShowMessage(actor, 0);
    }

    FieldScene_RunMiddleSequence(state, actor, 6);
    Event_End();
}

void ColossoLogRollingStage_RestoreActorPositions(void)
{
    extern s32 GameFlag_GetByte();

    s32 center;

    {
        s32 x = GameFlag_GetByte(896);
        s32 z = GameFlag_GetByte(904);
        center = 0x80000;
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Actor_SetPosition(ACTOR_GERALD, x, z);
    }
    {
        s32 x = GameFlag_GetByte(912);
        s32 z = GameFlag_GetByte(920);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Actor_SetPosition(ACTOR_IVAN, x, z);
    }
    {
        s32 x = GameFlag_GetByte(928);
        s32 z = GameFlag_GetByte(936);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Actor_SetPosition(ACTOR_MIA, x, z);
    }
}
