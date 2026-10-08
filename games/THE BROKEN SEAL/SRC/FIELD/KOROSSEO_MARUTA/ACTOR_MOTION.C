#include "LOG_ROLLING.H"
#include "CALL.H"
#include "text/MSG_IDS.H"

void FieldScene_RunFourStepActorMotion(s32 event)
{
    extern void Korosseo_FinishSoloRound();
    extern s32 FieldScene_RunMiddleSequence();
    extern void Korosseo_FadeInCompetitor(s32 actor, s32 x, s32 z);
    extern void Korosseo_RestoreCompetitor();
    extern void Engine_ObjectSetPosition();
    extern void Object_CommitPosition();
    extern void battle_owner_69();

    s32 result;
    struct FieldActor *actor;
    s32 x;
    s32 y;
    s32 raised_y;
    s32 shifted_x;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        result = ColossoLogRollingStage_RunStateInteraction(event, 4);
        if (result == 0) {
            Engine_EventSetMessage((s32)MsgKorosseoPlaceNormallyCalledFreeClimb);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x3580000, -1, 0xa80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(30);
            Event_ShowMessage(event, 0);
            Event_ShowMessage(event, 0);
            /* FAKEMATCH: through Value3 the coordinates load before r0; a direct call sets r0 first. */
            Value3((s32 (*)())Korosseo_FadeInCompetitor, 0, 0x330, 200);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x348, 200);
            /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
            Value3(Engine_ActorFaceDirection, 0, 0xc000, 20);
            battle_owner_69();
            Camera_MoveTo(-1, -1, -1, 0);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
            actor = Object_GetById(0);
            y = actor->y.fixed;
            x = actor->x.fixed;
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 10);
            raised_y = 0x60000 + y;
            Engine_ObjectSetPosition(actor, x, raised_y, actor->z.fixed);
            Object_CommitPosition(actor);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 14);
            shifted_x = 0x400000 + x;
            Engine_ObjectSetPosition(actor, shifted_x, raised_y, actor->z.fixed);
            Object_CommitPosition(actor);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 10);
            Engine_ObjectSetPosition(actor, shifted_x, y + 0x360000, actor->z.fixed);
            Object_CommitPosition(actor);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 15);
            Engine_ObjectSetPosition(actor, x + 0x300000, y + 0x360000, actor->z.fixed);
            Object_CommitPosition(actor);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 12);
            Event_ShowMessage(event, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
            ColossoLogRollingStage_InitializeStateInteraction(event, 4);
        } else if (result == 1) {
            Engine_EventSetMessage((s32)MsgKorosseoClearStageMustAbleChange);
            Event_ShowMessage(event, 0);
        }
        FieldScene_RunMiddleSequence(result, event, 4);
        Engine_EventEnd();
    }
}

void ColossoLogRollingStage_PositionActor(s32 selector, s32 x, s32 z)
{
    extern void ObjectDispatch_InitFromTable6();
    extern void Engine_ObjectSetPosition();

    s32 *record;

    record = (s32 *)Engine_ActorLookup(selector);
    if (record != 0) {
        ObjectDispatch_InitFromTable6();
        Object_SetMode(record, 5);
        Engine_ObjectSetPosition(record, x << 16, record[3], z << 16);
    }
}

void ColossoLogRollingStage_PositionAndActivateActor(s32 selector, s32 x, s32 z)
{
    extern void ObjectDispatch_InitFromTable6();
    extern void Engine_ObjectSetPosition();
    extern void Object_CommitPosition();

    s32 *record;

    record = (s32 *)Engine_ActorLookup(selector);
    if (record != 0) {
        ObjectDispatch_InitFromTable6();
        Object_SetMode(record, 5);
        Engine_ObjectSetPosition(record, x << 16, record[3], z << 16);
        Object_CommitPosition(record);
        Object_SetMode(record, 1);
    }
}

void FieldScene_RunOpeningAuxiliarySequence(s32 event)
{
    extern void Korosseo_FinishSoloRound();
    extern s32 FieldScene_RunMiddleSequence();
    extern void Korosseo_FadeInCompetitor(s32 actor, s32 x, s32 z);
    extern void Korosseo_RestoreCompetitor();

    s32 i;
    s32 result;
    struct FieldActor *actor;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        result = ColossoLogRollingStage_RunStateInteraction(event, 5);
        if (result == 0) {
            Engine_EventSetMessage((s32)MsgKorosseoCalledMovingSidewalkStage);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x4380000, -1, 0xa80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(30);
            Event_ShowMessage(event, 0);
            Event_ShowMessage(event, 0);
            /* FAKEMATCH: through Value3 the coordinates load before r0; a direct call sets r0 first. */
            Value3((s32 (*)())Korosseo_FadeInCompetitor, 0, 0x3d8, 184);
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
            actor = Object_GetById(0);
            for (i = 119; i >= 0; i--) {
                if (actor->x.fixed > 0x3e00000) {
                    actor->x.fixed -= 0x13333;
                }
                Engine_TaskWait(1);
            }
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x103, 60);
            ColossoLogRollingStage_PositionActor(0, 0x460, 184);
            Event_ShowMessage(event, 0);
            ColossoLogRollingStage_ResetActorMotion(0);
            gGameState.movement_mode = 1;
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
            ColossoLogRollingStage_InitializeStateInteraction(event, 5);
        } else if (result == 1) {
            Engine_EventSetMessage((s32)MsgKorosseoInStageMustTryOutpace);
            Event_ShowMessage(event, 0);
        }
        FieldScene_RunMiddleSequence(result, event, 5);
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
        Engine_ActorSetPosition(GERALD, x, z);
    }
    {
        s32 x = GameFlag_GetByte(912);
        s32 z = GameFlag_GetByte(920);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Engine_ActorSetPosition(IWAN, x, z);
    }
    {
        s32 x = GameFlag_GetByte(928);
        s32 z = GameFlag_GetByte(936);
        x <<= 20;
        x += center;
        z <<= 20;
        z += center;
        Engine_ActorSetPosition(MEARI, x, z);
    }
}
