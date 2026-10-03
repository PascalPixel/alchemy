/* The four-step actor motion, actor placement and the opening sequence. */
#include "LOG_ROLLING.H"
#include "CALL.H"
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
    extern void Korosseo_FadeInCompetitor(s32 actor, s32 x, s32 z);
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
        Engine_EventBegin();
        result = ColossoLogRollingStage_RunStateInteraction(a0, 4);
        if (result == 0) {
            Engine_EventSetMessage((s32)MsgKorosseoPlaceNormallyCalledFreeClimb);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x3580000, -1, 0xa80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(30);
            Event_ShowMessage(a0, 0);
            Event_ShowMessage(a0, 0);
            /* FAKEMATCH: direct void call moves r0 before r2; preserve native argument order. */
            Call3(Korosseo_FadeInCompetitor, 0, 0x330, 200);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x348, 200);
            /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
            Value3(Engine_ActorFaceDirection, 0, 0xc000, 20);
            battle_owner_69();
            Camera_MoveTo(-1, -1, -1, 0);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
            actor = Object_GetById(0);
            y = *(s32 *)(actor + 12);
            x = *(s32 *)(actor + 8);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 10);
            raised_y = 0x60000 + y;
            Object_SetPosition(actor, x, raised_y, *(s32 *)(actor + 16));
            Object_CommitPosition(actor);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 14);
            shifted_x = 0x400000 + x;
            Object_SetPosition(actor, shifted_x, raised_y, *(s32 *)(actor + 16));
            Object_CommitPosition(actor);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 10);
            Object_SetPosition(actor, shifted_x, y + 0x360000, *(s32 *)(actor + 16));
            Object_CommitPosition(actor);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 15);
            Object_SetPosition(actor, x + 0x300000, y + 0x360000, *(s32 *)(actor + 16));
            Object_CommitPosition(actor);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 12);
            Event_ShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
            ColossoLogRollingStage_InitializeStateInteraction(a0, 4);
        } else if (result == 1) {
            Engine_EventSetMessage((s32)MsgKorosseoClearStageMustAbleChange);
            Event_ShowMessage(a0, 0);
        }
        FieldScene_RunMiddleSequence(result, a0, 4);
        Engine_EventEnd();
    }
}

void ColossoLogRollingStage_PositionActor(s32 selector, s32 x, s32 z)
{
    extern void ObjectDispatch_InitFromTable6();
    extern void Object_SetPosition();

    s32 *record;

    record = (s32 *)Engine_ActorLookup(selector);
    if (record != 0) {
        ObjectDispatch_InitFromTable6();
        Object_SetMode(record, 5);
        Object_SetPosition(record, x << 16, record[3], z << 16);
    }
}

void ColossoLogRollingStage_PositionAndActivateActor(s32 selector, s32 x, s32 z)
{
    extern void ObjectDispatch_InitFromTable6();
    extern void Object_SetPosition();
    extern void Object_CommitPosition();

    s32 *record;

    record = (s32 *)Engine_ActorLookup(selector);
    if (record != 0) {
        ObjectDispatch_InitFromTable6();
        Object_SetMode(record, 5);
        Object_SetPosition(record, x << 16, record[3], z << 16);
        Object_CommitPosition(record);
        Object_SetMode(record, 1);
    }
}

void FieldScene_RunOpeningAuxiliarySequence(s32 a0)
{
    extern void Korosseo_FinishSoloRound();
    extern s32 FieldScene_RunMiddleSequence();
    extern void Korosseo_FadeInCompetitor(s32 actor, s32 x, s32 z);
    extern void Korosseo_RestoreCompetitor();

    s32 i;
    s32 rec2;
    s32 rec7;
    s32 record;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        rec2 = ColossoLogRollingStage_RunStateInteraction(a0, 5);
        if (rec2 != 0) {
        } else {
            Engine_EventSetMessage((s32)MsgKorosseoCalledMovingSidewalkStage);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x4380000, -1, 0xa80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(30);
            Event_ShowMessage(a0, 0);
            Event_ShowMessage(a0, 0);
            /* FAKEMATCH: direct void call moves r0 before r2; preserve native argument order. */
            Call3(Korosseo_FadeInCompetitor, 0, 0x3d8, 184);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
            ColossoLogRollingStage_PositionAndActivateActor(0, 0x3e0, 184);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x4ccc, 0x2666);
            ColossoLogRollingStage_PositionActor(0, 0x460, 184);
            Engine_EventWait(120);
            Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
            Engine_EventWait(120);
            ColossoLogRollingStage_ResetActorMotion(0);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
#if !defined(TBS_EDITION_ES) && !defined(TBS_EDITION_FR) && !defined(TBS_EDITION_IT)
            Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
#endif
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
            rec7 = Object_GetById(0);
            for (i = 119; i >= 0; i--) {
                if (*(s32 *)(rec7 + 8) > 0x3e00000) {
                    *(s32 *)(rec7 + 8) += -0x13333;
                }
                Engine_TaskWait(1);
            }
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x103, 60);
            ColossoLogRollingStage_PositionActor(0, 0x460, 184);
            Event_ShowMessage(a0, 0);
            ColossoLogRollingStage_ResetActorMotion(0);
            {
                u8 *flag = (u8 *)&gGameState;

                flag[498] = 1;
            }
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
            ColossoLogRollingStage_InitializeStateInteraction(a0, 5);
            goto L_02002494;
        }
        if (rec2 == 1) {
            Engine_EventSetMessage((s32)MsgKorosseoInStageMustTryOutpace);
            Event_ShowMessage(a0, 0);
        }
        L_02002494:;
        FieldScene_RunMiddleSequence(rec2, a0, 5);
        Engine_EventEnd();
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

    Engine_EventBegin();
    state = ColossoLogRollingStage_RunStateInteraction(actor, 6);

    if (state == 0) {
        Engine_EventSetMessage((s32)MsgKorosseoAnotherLogRollingArea);
        Engine_CameraSetSpeed(0x30000, 0x6000);
        Engine_CameraMoveTo(0x5080000, -1, 0x980000, 1);
        Engine_CameraWaitForMove();
        Engine_EventWait(30);
        Engine_EventShowMessage(actor, 0);
        ColossoLogRollingStage_StartPaletteTask(0xb4, 0x58, 0);
        Engine_EventWait(60);
        Engine_EventShowMessage(actor, 0);
        ColossoLogRollingStage_StartPaletteTaskFromState(0x20, 0x54, 10);
        Engine_EventWait(30);
        Engine_EventShowMessage(actor, 0);
        ColossoLogRollingStage_StartPaletteTaskFromState(0x60, 0x54, 30);
        Engine_EventWait(60);
        Engine_EventShowMessage(actor, 0);
        ColossoLogRollingStage_StopPaletteTask();
        Engine_EventWait(2);
        Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
        ColossoLogRollingStage_InitializeStateInteraction(actor, 6);
    } else if (state == 1) {
        Engine_EventSetMessage((s32)MsgKorosseoHereMustFigureOutHow);
        Engine_EventShowMessage(actor, 0);
    }

    FieldScene_RunMiddleSequence(state, actor, 6);
    Engine_EventEnd();
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
        Engine_ActorSetPosition(ACTOR_GERALD, x, z);
    }
    {
        s32 x = GameFlag_GetByte(912);
        s32 z = GameFlag_GetByte(920);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Engine_ActorSetPosition(ACTOR_IVAN, x, z);
    }
    {
        s32 x = GameFlag_GetByte(928);
        s32 z = GameFlag_GetByte(936);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Engine_ActorSetPosition(ACTOR_MIA, x, z);
    }
}
