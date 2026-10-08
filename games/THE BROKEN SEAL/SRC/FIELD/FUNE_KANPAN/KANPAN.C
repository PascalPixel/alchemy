#define OWN_gGameState
#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "GLOBAL_CELLS.H"
#include "EDITION.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"
#include "CALL.H"
#include "SCENE_IDS.H"
#include "FIELD_SERVICE.H"
#include "text/MSG_IDS.H"


union KanpanSlot {
    s32 w;
    s16 h[2];
};



struct FlyBy {
    u8 unknown_00[0x64];
    s16 step;
};

s32 Engine_RandomNext();





extern struct EventWork *gEventWork;
void FuneKanpan_OpenFirstDoor();
void FuneKanpan_OpenSecondDoor();
void Engine_EventBegin();
void Engine_EventWait();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_CenterOnColumn();
void Engine_EventRequestExit();
void Engine_AudioPlayCue();

void Battle_ResetEffectCounter();
void FuneKanpan_OpenSecondDoor(void);
void FieldScene_RunStepThen10(s32 a);




extern s16 gGameState[][1];

void Engine_ActorSetSpritePriority();
s32 Engine_GameFlagIsSet();
void Engine_ActorSetPosition();
s32 Math_RemainderUnsigned();
void Engine_ActorEnableActionCallback();

void Event_CallWithLastActiveObjectId();

void Engine_EventBegin();
void Engine_TaskWait();
void Engine_ActorDestroy();
void FuneKanpan_RunRobinTalk();
void Engine_EventEnd();

void FuneKanpan_AnnouncePreparationsReady();
void FieldScene_CallPairWith10();
void FuneKanpan_OpenFirstDoor(void);

void FieldScene_RunStepThen10();
void Engine_TaskWait();
void Engine_ActorSetPosition();
void Engine_ActorSetChildValue();
void Engine_ActorSetDestination();
void Engine_ActorRunRepeatedMotion();
void Engine_EventSetMessage();
void Graphics_EnableObjLayerAndCallbacks();
void Engine_ActorEnableActionCallback();
void ObjectDispatch_StopCallbacksAndHideLayers();
void UiText_ShowCenteredMessage();
void Object_RefreshSelectorById();
void Engine_ActorWalkTo();
void Ui_SetRenderResultFromObject();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();

void Engine_ActorSetChildValue();
void Engine_ActorSetSpriteFlags();
void Engine_ActorSetAnimation();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Engine_EventWait();
void Engine_ActorSetSpeed();
void Engine_ActorSetDestination();
void Engine_ObjectMotionSetPositionAndCommit();
void Engine_AudioPlayCue();
void Object_SetActionCallbackAndRefreshById();
void Engine_EventCloseScreen();
void Engine_ActorStop();
void Engine_EventRequestExit();



void OverlayObject_DecayFields24And28();
void Party_SetFields1ceAnd1d0();
void BattleFx_SetWeightedResult();
void Engine_CameraFollowActor();
void Engine_ActorWalkToAndWait();
void Engine_ActorJump();
void Engine_WorkSetValuesIfNonNegative();
void Engine_ActorFaceDirection();
void Engine_ActorWalkTo();
void Engine_ActorSetAttachedEffect();

void FieldScene_RunStepThen10(s32 actor);
void Object_RefreshSelectorById(s32 actor);

struct MapLayer {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    u8 unknown_10[32];
};

struct MapWork {
    s32 *camera;
    u8 unknown_04[16];
    struct MapLayer layers[8];
};

u16 FuneKanpan_SlotPhase[4] = { 0 };

s32 FuneKanpan_WaveAngleY = 0;
s32 FuneKanpan_SlotWork = 0;

u16 FuneKanpan_SlotValue[8] = { 0 };

s32 FuneKanpan_LayerScroll[2] = { 0 };

s32 FuneKanpan_WaveAngleX = 0;
s32 FuneKanpan_DeckSpare = 0;

s32 FuneKanpan_LayerSpeed[2] = { 0 };


s32 SceneActor_OscillateHeightBetweenLimits(struct FieldActor *actor)
{
    s16 *flag = (s16 *)&actor->work;
    s32 val;
    s32 tmp;

    if (*flag != 0) {
        val = actor->y.fixed - (((u32)(Engine_RandomNext() << 15)) >> 16) - 0x8000;
        actor->y.fixed = val;
        if (val >= 0x40000)
            goto done;
        tmp = 0;
    } else {
        val = actor->y.fixed + (((u32)(Engine_RandomNext() << 15)) >> 16) + 0x8000;
        actor->y.fixed = val;
        if (val <= 0xC0000)
            goto done;
        tmp = 1;
    }
    *flag = tmp;
done:
    return 1;
}

s32 SceneActor_SetFacingFromSample(struct FieldActor *actor)
{
    u32 v = ((u32)(Engine_RandomNext() << 5)) >> 16;

    if (v == 6) {
        s32 t = 0xD0;
        actor->facing = t << 8;
    } else if (v == 9) {
        s32 t = 0xB0;
        actor->facing = t << 8;
    }
    return 1;
}

s32 FuneKanpan_UpdateFlyByForActor21(struct FieldActor *obj)
{
    struct FlyBy *gull;

    gull = (struct FlyBy *)obj;
    switch (gull->step) {
    case 0:
        if ((((u32)Engine_RandomNext() * 40) >> 16) == 0) {
            gull->step++;
        }
        break;
    case 1:
        gull->step++;
        break;
    case 2:
        obj->velocity_y = 0x40000;
        obj->speed = 0x40000;
        obj->acceleration = 0x20000;
        FieldObject_SetPosition((s32)obj, 0x1080000, 0, 0x2960000);
        gull->step++;
        break;
    case 3:
        gull->step++;
        break;
    case 4:
        if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x && obj->target_z == obj->target_y) {
            gull->step++;
            Engine_AudioPlayCue(152);
            if (obj->rise_enabled != 0) {
                Actor_FaceDirection(21, 0xb000, 0);
            } else {
                Actor_FaceDirection(21, 0x5000, 0);
            }
            if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
                Object_GetById(21)->velocity_y = 0x20000;
            } else {
                Actor_ShowEmote(21, 0x103, 0);
                Object_GetById(21)->velocity_y = 0x60000;
            }
        }
        break;
    case 5:
        gull->step++;
        break;
    case 6:
        gull->step++;
        obj->velocity_y = 0x40000;
        obj->speed = 0x20000;
        obj->acceleration = 0x10000;
        if (obj->rise_enabled != 0) {
            FieldObject_SetPosition((s32)obj, 0xfc0000, 0, 0x2860000);
        } else {
            FieldObject_SetPosition((s32)obj, 0x1000000, 0, 0x2ae0000);
        }
        break;
    case 7:
        gull->step++;
        break;
    case 8:
        if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x && obj->target_z == obj->target_y) {
            gull->step++;
        }
        break;
    case 9:
        gull->step = 0;
        break;
    }
    return 1;
}

s32 FuneKanpan_UpdateFlyByForActor22(struct FieldActor *obj)
{
    struct FlyBy *gull;

    gull = (struct FlyBy *)obj;
    switch (gull->step) {
    case 0:
        if ((((u32)Engine_RandomNext() * 40) >> 16) == 0) {
            gull->step++;
        }
        break;
    case 1:
        gull->step++;
        break;
    case 2:
        obj->velocity_y = 0x40000;
        obj->speed = 0x40000;
        obj->acceleration = 0x20000;
        FieldObject_SetPosition((s32)obj, 0xb00000, 0, 0x2b80000);
        gull->step++;
        break;
    case 3:
        gull->step++;
        break;
    case 4:
        if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x && obj->target_z == obj->target_y) {
            gull->step++;
            Engine_AudioPlayCue(152);
            if (obj->rise_enabled != 0) {
                Actor_FaceDirection(22, 0xd000, 0);
            } else {
                Engine_ActorFaceDirection(22, 0, 0);
            }
            if (((u32)Engine_RandomNext() << 2) >> 16 != 0) {
                Object_GetById(22)->velocity_y = 0x20000;
            } else {
                Actor_ShowEmote(22, 0x103, 0);
                Object_GetById(22)->velocity_y = 0x60000;
            }
        }
        break;
    case 5:
        gull->step++;
        break;
    case 6:
        gull->step++;
        obj->velocity_y = 0x40000;
        obj->speed = 0x20000;
        obj->acceleration = 0x10000;
        if (obj->rise_enabled != 0) {
            FieldObject_SetPosition((s32)obj, 0xb80000, 0, 0x2a00000);
        } else {
            FieldObject_SetPosition((s32)obj, 0xca0000, 0, 0x2b40000);
        }
        break;
    case 7:
        gull->step++;
        break;
    case 8:
        if (obj->target_x == ACTOR_NO_TARGET && obj->target_y == obj->target_x && obj->target_z == obj->target_y) {
            gull->step++;
        }
        break;
    case 9:
        gull->step = 0;
        break;
    }
    return 1;
}

s32 FuneKanpan_RandomlyStartActorHop(struct FieldActor *actor)
{
    if ((((u32)(Engine_RandomNext() * 40)) >> 16) == 0)
        actor->velocity_y = 0x40000;
    return 1;
}

void OverlayObject_DecayFields24And28(struct FieldActor *actor)
{
    if (actor->scale_x > 0x10000) {
        actor->scale_x += 0xFFFFF800;
        actor->scale_y += 0xFFFFF800;
    }
}

s32 FuneKanpan_IdleTurn(u8 *actor)
{
    /* FAKEMATCH: word heading temporaries retain movs/lsls formation before the halfword stores. */

    if (actor[98] != 0) {
        actor[98]--;
    } else {
        u32 roll = (u32)(Engine_RandomNext() * 300) >> 16;

        if (roll > 200) {
            s32 dir = 0xd000;

            *(u16 *)(actor + 6) = dir;
        } else if (roll > 100) {
            s32 dir = 0x5000;

            *(u16 *)(actor + 6) = dir;
        } else {
            *(u16 *)(actor + 6) = 0;
        }
        actor[98] = ((u32)(Engine_RandomNext() * 80) >> 16) + 80;
    }
    return 1;
}

u8 *FuneKanpan_GetEntrances(void)
{
    return FuneKanpan_SceneTableA;
}

u8 *FuneKanpan_GetRegions(void)
{
    return FuneKanpan_SceneTableB;
}

u8 *FuneKanpan_GetExits(void)
{
    return FuneKanpan_SceneTableC;
}

u8 *FuneKanpan_GetPlacements(void)
{
    s32 v;

    if (Engine_GameFlagIsSet(0x93e))
        return gFuneKanpanPlacementsFlag93e;
    if (Engine_GameFlagIsSet(0x927))
        return gFuneKanpanPlacementsFlag927;
    v = Engine_GameFlagIsSet(0x928);
    if (v != 0)
        return gFuneKanpanPlacementsFlag928;
    if (Engine_GameFlagIsSet(0x911)) {
        if (Engine_GameFlagIsSet(0x925)) {
            gFuneKanpanPlacementsFlag911[0x14E] = v;
            gFuneKanpanPlacementsFlag911[0x1AE] = 2;
            gFuneKanpanPlacementsFlag911[0x1C6] = 2;
        } else if (Engine_GameFlagIsSet(0x922)) {
            gFuneKanpanPlacementsFlag911[0x1AE] = 1;
            gFuneKanpanPlacementsFlag911[0x1C6] = 1;
        }
        return gFuneKanpanPlacementsFlag911;
    }
    return gFuneKanpanPlacements;
}

u8 *FuneKanpan_GetEvents(void)
{
    if (Engine_GameFlagIsSet(0x93e))
        return gFuneKanpanEventsFlag93e;
    if (Engine_GameFlagIsSet(0x8A0))
        return gFuneKanpanEventsFlag8a0;
    if (Engine_GameFlagIsSet(0x928))
        return gFuneKanpanEventsFlag928;
    return gFuneKanpanEvents;
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    struct FieldActor *rec7;
    s32 record;
    s32 shown;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x925) != 0) {
        Engine_EventSetMessage((s32)MsgFuneNowWeHaveProtectShip);
        Event_ShowMessage(21, 0);
    } else {
        if (GameFlag_IsSet(0x922) != 0) {
            Engine_ActorRunRepeatedMotion(21, 2);
            Engine_EventSetMessage((s32)MsgFuneSomebodyStopThem);
            Event_ShowMessage(21, 0);
            rec7 = Object_GetById(21);
            record = Random_Next();
            shown = ((u32)(90 * record) >> 16) + 60;
            rec7->timer = shown;
            Engine_ActorEnableActionCallback(21, FuneKanpan_RandomActorActions);
        } else {
            Actor_ShowEmote(21, 0x103, 0);
            Engine_ActorStartRepeatedMotion(21, 3);
            Engine_EventSetMessage((s32)MsgFuneToldWereLeavingSoonSet);
            Event_ShowMessage(21, 0);
        }
    }
    Engine_EventEnd();
}

void FieldScene_RunScene3afSequenceA(void)
{
    struct FieldActor *rec7;
    s32 record;
    s32 shown;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x925) != 0) {
        Engine_EventSetMessage((s32)MsgFuneHaveMakeThemPromiseHelp);
        Event_ShowMessage(24, 0);
    } else {
        if (GameFlag_IsSet(0x922) != 0) {
            Engine_ActorRunRepeatedMotion(24, 2);
            Engine_EventSetMessage((s32)MsgFuneTheyCantPlanningMutiny);
            Event_ShowMessage(24, 0);
            rec7 = Object_GetById(24);
            record = Random_Next();
            shown = ((u32)(90 * record) >> 16) + 60;
            rec7->timer = shown;
            Engine_ActorEnableActionCallback(24, FuneKanpan_RandomActorActions);
        } else {
            Actor_ShowEmote(24, 0x103, 0);
            Engine_ActorStartRepeatedMotion(24, 3);
            Engine_EventSetMessage((s32)MsgFuneIfWeDontLeaveSoon);
            Event_ShowMessage(24, 0);
        }
    }
    Engine_EventEnd();
}

void SceneDialogue_RunActor21Line(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgFuneHeadedColosso);
    Engine_EventAskYesNo(21, 0);
    Engine_EventEnd();
}

void FuneKanpan_OpenFirstDoor(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x271) == 0) {
        Audio_PlayCue(158);
        Map_CopyCellsTo(30, 94, 13, 94, 1, 3);
        GameFlag_Set(0x271);
    }
}

void FuneKanpan_OpenSecondDoor(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x272) == 0) {
        Audio_PlayCue(158);
        Map_CopyCellsTo(30, 108, 13, 108, 1, 2);
        GameFlag_Set(0x272);
    }
}

void FuneKanpan_RunDeckStateEvent(void)
{

    s32 v5;
    struct EventWork *p6;

    p6 = gEventWork;
    Engine_EventBegin();
    v5 = 0;
    switch (p6->touched_trigger) {
    case 1:
        v5 = 1;
        FuneKanpan_OpenFirstDoor();
        break;
    case 3:
        v5 = 1;
        FuneKanpan_OpenSecondDoor();
        break;
    }
    if (v5 != 0) {
        Actor_SetSpeed(0, 0x9999, 0x4ccc);
        FieldObjectMotion_CenterOnColumn(0, 1, -10);
        Engine_EventWait(10);
    } else {
        Engine_AudioPlayCue(123);
    }
    Engine_EventRequestExit(p6->touched_trigger);
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
void FuneKanpan_RunDeckStateEventFromEntry(void)
{
    /* FAKEMATCH: the measured Call3 spelling sets actor and x before shifting z; a direct call changes six bytes of argument setup. */
    if ((u16)(Object_GetById(ACTOR_PARTY_LEADER)->facing - 0x6001) <= 0x3ffe) {
        Engine_EventBegin();
        Actor_WalkTo(ACTOR_PARTY_LEADER, 214, 664);
        Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
        FuneKanpan_RunDeckStateEvent();
    }
}
#endif

void FieldScene_RunActorAndEffectPresentationSetup(void)
{
    struct FieldActor *record;

    if (GameFlag_IsSet(0x911) != 0) {
        Engine_EventBegin();
        Battle_ResetEffectCounter();
        Actor_FaceActor(ACTOR_PARTY_LEADER, 20, 10);
        Camera_SetSpeed(0x19999, 0x3333);
        Camera_MoveTo(0xbe0000, -1, 0x2c40000, 1);
        Engine_CameraWaitForMove();
        Engine_EventWait(40);
        Engine_ActorRunRepeatedMotion(22, 1);
        Engine_EventSetMessage((s32)MsgFuneWonderCouldHaveHappened);
        FieldScene_RunStepThen10(0x4016);
        Actor_ShowEmote(20, 0x102, 60);
        Engine_ActorStartRepeatedMotion(20, 2);
        FieldScene_RunStepThen10(20);
        Engine_ActorRunRepeatedMotion(22, 1);
        Actor_FaceDirection(22, 0x5000, 0);
        FieldScene_RunStepThen10(0x4016);
        Engine_ActorRunRepeatedMotion(20, 1);
        FieldScene_CallPairWith10(20, 0xb000);
        FieldScene_RunStepThen10(20);
        FieldScene_CallPairWith10(23, 0x3000);
        Engine_ActorSetAnimation(23, 3);
        FieldScene_RunStepThen10(0x4017);
        Actor_ShowEmote(22, 0x101, 40);
        Actor_FaceDirection(22, 0x8000, 20);
        FieldScene_RunStepThen10(0x4016);
        FieldScene_CallPairWith10(23, 0);
        Engine_ActorSetAnimationAndWait(23, 4);
        FieldScene_RunStepThen10(0x4017);
        Actor_ShowEmote(20, 0x100, 40);
        Engine_ActorStartRepeatedMotion(20, 2);
        FieldScene_RunStepThen10(20);
        Engine_ActorSetAnimationAndWait(22, 3);
        FieldScene_RunStepThen10(0x4016);
        FieldScene_CallPairWith10(20, 0xd000);
        Engine_ActorSetAnimation(23, 3);
        Engine_ActorSetAnimationAndWait(20, 3);
        Engine_EventWait(60);
        Actor_ShowEmote(22, 0x106, 40);
        FieldScene_CallPairWith10(22, 0x5000);
        Engine_EventSetMessage((s32)MsgFuneTheresNothingWeCanDo);
        Engine_ActorStartRepeatedMotion(22, 1);
        FieldScene_RunStepThen10(0x4016);
        Actor_ShowEmote(20, 0x101, 40);
        Engine_ActorStartRepeatedMotion(20, 2);
        FieldScene_RunStepThen10(20);
        Actor_ShowEmote(22, 0x108, 20);
        Event_ShowMessageAndWait(0x4016, 0, 20);
        Actor_ShowEmote(23, 0x102, 60);
        FieldScene_RunStepThen10(0x4017);
        FieldScene_CallPairWith10(22, 0x8000);
        Engine_ActorSetAnimationAndWait(22, 3);
        Event_ShowMessageAndWait(0x4016, 0, 20);
        Actor_ShowEmote(20, 0x102, 40);
        Engine_ActorStartRepeatedMotion(20, 2);
        FieldScene_RunStepThen10(20);
        FieldScene_CallPairWith10(22, 0x5000);
        Engine_ActorSetAnimation(22, 4);
        FieldScene_RunStepThen10(22);
        Actor_FaceDirection(20, 0xb000, 0);
        Actor_FaceDirection(23, 0x3000, 40);
        Actor_FaceDirection(23, 0, 0);
        Actor_FaceDirection(20, 0xd000, 20);
        Engine_ActorRunRepeatedMotion(22, 2);
        Engine_EventWait(20);
        FieldScene_RunStepThen10(0x4016);
        Actor_SetAttachedEffect(23, 0x102);
        Actor_SetAttachedEffect(20, 0x102);
        Engine_EventWait(40);
        Engine_ActorSetAnimationAndWait(22, 3);
        FieldScene_RunStepThen10(0x4016);
        Camera_SetSpeed(0xcccc, 0x1999);
        Camera_MoveTo(0xb60000, -1, 0x2f80000, 1);
        Actor_SetSpeed(23, 0xcccc, 0x6666);
        ((s32 (*)())Engine_ActorEnableActionCallback)(23, (s32)FuneKanpan_PresentationActionsA);
        Actor_SetSpeed(22, 0xcccc, 0x6666);
        ((s32 (*)())Engine_ActorEnableActionCallback)(22, (s32)FuneKanpan_PresentationActionsB);
        Actor_SetSpeed(20, 0xcccc, 0x6666);
        Actor_WalkToAndWait(20, 182, 0x2f8);
        Engine_ActorStartRepeatedMotion(20, 2);
        Actor_ShowEmote(20, 0x100, 60);
        FieldScene_CallPairWith10(20, 0xd000);
        Event_ShowMessageAndWait(20, 0, 20);
        Engine_ActorSetAnimationAndWait(20, 3);
        Engine_ActorJump(20, 4, 0);
        Actor_FaceDirection(20, 0x3000, 40);
        Camera_SetSpeed(0x10000, 0x2000);
        Camera_MoveTo(0xd80000, -1, 0x3160000, 1);
        {
            struct FieldActor *record = Object_GetById(20);
            record->priority_flags = ACTOR_PRIORITY_AUTOMATIC | record->priority_flags;
        }
        Actor_SetSpeed(20, 0x13333, 0x9999);
        Actor_WalkToAndWait(20, 182, 0x30e);
        Actor_WalkToAndWait(20, 192, 0x328);
        Actor_WalkToAndWait(20, 216, 0x328);
        FieldScene_CallPairWith10(20, 0xd000);
        Engine_ActorRunRepeatedMotion(20, 2);
        FuneKanpan_OpenSecondDoor();
        Actor_WalkToAndWait(20, 216, 0x31e);
        Actor_SetPosition(20, 0, 0);
        record = Object_GetById(20);
        record->scale_x = 0x10000;
        record = Object_GetById(20);
        record->scale_y = 0x10000;
        GameFlag_Set(0x920);
        Engine_EventEnd();
    }
}

void FuneKanpan_WatchCrewmanLeaveRight(void)
{
    u8 bits;

    if (GameFlag_IsSet(0x911) != 0) {
        if (GameFlag_IsSet(0x922) == 0) {
            Engine_EventBegin();
            Battle_ResetEffectCounter();
            FuneKanpan_ConfrontCrewman();
            Actor_SetSpeed(20, 0x6666, 0x3333);
            Object_GetById(20)->steering_flags &= ~ACTOR_FACES_MOTION;
            Actor_WalkToAndWait(20, 232, 0x330);
            Engine_EventWait(1);
            bits = 1;
            {
                u8 *record = (s32)Object_GetById(20);
                u8 value = record[90];

                record[90] = value | bits;
            }
            Engine_EventWait(20);
            Engine_ActorStartRepeatedMotion(20, 2);
            FieldScene_RunStepThen10(20);
            Actor_SetSpeed(20, 0x13333, 0x9999);
            Object_GetById(20)->steering_flags &= ~ACTOR_FACES_MOTION;
            Actor_WalkToAndWait(20, 244, 0x324);
            Engine_EventWait(1);
            {
                struct FieldActor *record = Object_GetById(20);

                bits |= record->steering_flags;
                record->steering_flags = bits;
            }
            Engine_EventWait(20);
            Actor_SetSpeed(20, 0x33333, 0x19999);
            Actor_WalkToAndWait(20, 248, 0x30a);
            Actor_WalkToAndWait(20, 248, 0x2bc);
            Actor_SetPosition(20, 0xf60000, 0x2000000);
            Actor_FaceDirection(20, 0, 0);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
            Engine_EventEnd();
        }
    }
}

void FuneKanpan_WatchCrewmanLeaveLeft(void)
{
    u8 bits;

    if (GameFlag_IsSet(0x911) != 0) {
        if (GameFlag_IsSet(0x922) == 0) {
            Engine_EventBegin();
            Battle_ResetEffectCounter();
            FuneKanpan_ConfrontCrewman();
            Actor_SetSpeed(20, 0x6666, 0x3333);
            Object_GetById(20)->steering_flags &= ~ACTOR_FACES_MOTION;
            Actor_WalkToAndWait(20, 202, 0x330);
            Engine_EventWait(1);
            bits = 1;
            {
                u8 *record = (s32)Object_GetById(20);
                u8 value = record[90];

                record[90] = value | bits;
            }
            Engine_EventWait(20);
            Engine_ActorStartRepeatedMotion(20, 2);
            FieldScene_RunStepThen10(20);
            Actor_SetSpeed(20, 0x13333, 0x9999);
            Object_GetById(20)->steering_flags &= ~ACTOR_FACES_MOTION;
            Actor_WalkToAndWait(20, 192, 0x324);
            Engine_EventWait(1);
            {
                struct FieldActor *record = Object_GetById(20);

                bits |= record->steering_flags;
                record->steering_flags = bits;
            }
            Engine_EventWait(20);
            Actor_SetSpeed(20, 0x33333, 0x19999);
            Actor_WalkToAndWait(20, 180, 0x30a);
            Actor_WalkToAndWait(20, 180, 0x2bc);
            Actor_SetPosition(20, 0xf60000, 0x2000000);
            Actor_FaceDirection(20, 0, 0);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
            Engine_EventEnd();
        }
    }
}

void FuneKanpan_ConfrontCrewman(void)
{
    u32 i;
    s32 record;

    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0xd80000, -1, 0x3380000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    FuneKanpan_OpenSecondDoor();
    Map_CopyCellsTo(30, 108, 13, 108, 1, 2);
    Engine_EventWait(10);
    Actor_SetPosition(20, 0xd80000, 0x3200000);
    Actor_SetSpeed(20, 0x13333, 0x9999);
    Actor_WalkToAndWait(20, 216, 0x32e);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 20, 10);
    Engine_ActorSetAnimationAndWait(20, 4);
    Engine_ActorStartRepeatedMotion(20, 2);
    Actor_ShowEmote(20, 0x100, 20);
    Actor_FaceActor(20, ACTOR_PARTY_LEADER, 20);
    Engine_ActorStartRepeatedMotion(20, 2);
    Engine_EventSetMessage((s32)MsgFuneDidntDoAnything);
    Event_ShowMessageAndWait(20, 0, 20);
    Engine_ActorShowEmote(20, 0x102, 0);
    GameFlag_Set(0x923);
}

s32 FuneKanpan_ResetDeck(void)
{
    u8 **base = ((u8 * *)gMapWork);
    u8 *map = base[0] + 0x104;

    Engine_GameFlagClear(0x11c);
    *(s32 *)(base[19] + 0x1c0) = 0x209;
    *(s32 *)(map + 0x1c) = 0;
    FuneKanpan_WaveAngleX = (u16)Engine_RandomNext();
    FuneKanpan_WaveAngleY = (u16)Engine_RandomNext();
    FuneKanpan_LayerScroll[0] = 0;
    FuneKanpan_LayerScroll[1] = 0;
    FuneKanpan_LayerSpeed[0] = 0;
    Engine_MapRedraw();
    Engine_TaskWait(1);
    FuneKanpan_ApplyEntryState();
    return 0;
}

void FuneKanpan_ApplyEntryState(void)
{
    /* FAKEMATCH: retained return/prototype call casts preserve the current call lowering. */

    s32 flag;

    Engine_GameFlagSet(0x144);
    if (Engine_GameFlagIsSet(0x109) != 0) {
        Engine_GameFlagClear(0x271);
        Engine_GameFlagClear(0x272);
    }
    if (Engine_GameFlagIsSet(0x93e) != 0) {
        SceneState_ApplyFiveRectsAtColumn78();
        DialogueLayout_ConfigureTwoRegions();
        Engine_ActorSetChildValue(24, 2);
    } else if (Engine_GameFlagIsSet(0x8a0) != 0) {
        SceneState_ApplyFiveRectsAtColumn78();
        DialogueLayout_ConfigureTwoRegions();
    } else if ((flag = Engine_GameFlagIsSet(0x927)) != 0) {
        SceneState_ApplyFiveRectsAtColumn78();
        SceneEffect_InitSlotsEightToNineteen();
        Value2(Engine_TaskAddCallback, (s32)SceneState_ConfigureEntries8Through19, 0xc80);
        FuneKanpan_LayerScroll[1] = 0x200000;
        FuneKanpan_LayerSpeed[1] = 0x13333;
        Engine_TaskAddCallback(FuneKanpan_RockDeck, 0xc80);
    } else if (Engine_GameFlagIsSet(0x928) != 0) {
        SceneState_ApplyFiveRectsAtColumn78();
        FuneKanpan_LayerScroll[1] = flag;
        FuneKanpan_LayerSpeed[1] = flag;
        Engine_TaskAddCallback(FuneKanpan_RockDeck, 0xc80);
    }
    if (Engine_GameFlagIsSet(0x927) == 0) {
        SceneState_InitActorSlots8To19();
    }
    switch (gGameState[225][0]) {
    case 4:
        Object_GetById(0)->sprite->priority = 1;
        break;
    case 10:
        if (Engine_GameFlagIsSet(0x928) != 0) {
            FuneKanpan_SetupVisibleLeaderDeparture();
        } else {
            FuneKanpan_SetupHiddenLeaderDeparture();
        }
        return;
    case 11:
        FieldScene_RunActorTwentyDialogueSequence();
        return;
    case 12:
        FieldScene_RunShipDeckEventScript();
        return;
    case 13:
        FuneKanpan_RunDeckCrewScene();
        return;
    case 14:
        FieldScene_RunActorSequence();
        return;
    case 15:
        FuneKanpan_RunJumpScene();
        return;
    case 16:
        if (Engine_GameFlagIsSet(0x109) != 0) {
            FuneKanpan_SetupDeckCrewman();
        } else {
            FieldScene_ConfigureLeadActors();
        }
        return;
    case 17:
        FieldScene_ConfigureThreeActors();
        return;
    case 18:
        if (Engine_GameFlagIsSet(0x109) == 0) {
            FieldScene_RunPartyRosterScene();
        }
        return;
    case 19:
        FuneKanpan_ArriveAtTolbi();
        return;
    }
    if (Engine_GameFlagIsSet(0x93e) != 0) {
        *(s32 *)(((u8 *)gMapWork[0]) + 236) = 0x410000;
    } else if (Engine_GameFlagIsSet(0x8a0) != 0) {
        ((void (*)(void))FuneKanpan_PlaceDeckActors)();
    } else if (Engine_GameFlagIsSet(0x92b) != 0) {
        FuneKanpan_SetupDeckCrewman();
    } else if (Engine_GameFlagIsSet(0x928) != 0) {
        FuneKanpan_SetupRaisedCrewman();
    } else if (Engine_GameFlagIsSet(0x925) != 0) {
        SceneActor_PlaceActors20To27();
    } else if (Engine_GameFlagIsSet(0x911) != 0) {
        FuneKanpan_PlaceRandomDeckActors();
    }
}

void FuneKanpan_PlaceRandomDeckActors(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 base6_200c4d8;
    s32 a;

    Engine_ActorSetSpritePriority(27, 1);
    Engine_ActorSetSpritePriority(23, 1);
    Engine_ActorSetSpritePriority(22, 1);
    Engine_ActorSetSpritePriority(26, 1);
    Engine_ActorSetSpritePriority(24, 1);
    if (Engine_GameFlagIsSet(0x920) != 0) {
        Engine_ActorSetPosition(22, 0xa20000, 0x29a0000);
        record = (s32)Object_GetById(22);
        {
            s32 shown = 0x8000;

            *(u16 *)(record + 6) = shown;
        }
        Engine_ActorSetPosition(23, 0, 0);
        Engine_ActorSetPosition(20, 0, 0);
    }
    rec7 = Engine_GameFlagIsSet(0x922);
    if (rec7 != 0) {
        Engine_ActorSetPosition(21, 0x1080000, 0x2be0000);
        record = (s32)Object_GetById(21);
        {
            u16 shown = 0x5000;

            *(u16 *)(record + 6) = shown;
        }
        a = (s32)Object_GetById(21);
        record = Engine_RandomNext();
        {
            /* FAKEMATCH: the temporary makes the +60 add come before the +100. */
            s32 t = Math_RemainderUnsigned(record, 90) + 60;

            a += 100;
            *(u16 *)a = t;
        }
        Engine_ActorEnableActionCallback(21, FuneKanpan_RandomActorActions);
        Engine_ActorSetPosition(24, 0xf80000, 0x2a80000);
        a = Actor_Get(24);
        record = Engine_RandomNext();
        a += 100;
        *(u16 *)a = (Math_RemainderUnsigned(record, 90) + 60);
        Engine_ActorEnableActionCallback(24, FuneKanpan_RandomActorActions);
        Engine_ActorSetPosition(22, 0, 0);
    } else {
        if (Engine_GameFlagIsSet(0x923) != 0) {
            Actor_SetPosition(20, 0xf60000, 0x2000000);
            record = (s32)Object_GetById(20);
            *(u16 *)(record + 6) = rec7;
        }
    }
}

void SceneActor_PlaceActors20To27(void)
{
    s32 m = 0xA0;

    m <<= 7;
    Engine_ActorSetPosition(21, 0x1060000, 0x2C20000);
    Object_GetById(21)->facing = m;
    Engine_ActorSetPosition(24, 0xA40000, 0x2880000);
    {
        s32 z = 0;
        Object_GetById(24)->facing = z;
    }
    Engine_ActorSetSpritePriority(24, 1);
    Engine_ActorSetPosition(25, 0xC60000, 0x2990000);
    {
        s32 x = 0x80;
        Object_GetById(25)->facing = x << 8;
    }
    Engine_ActorSetSpritePriority(25, 1);
    Engine_ActorSetPosition(26, 0xBC0000, 0x2A60000);
    {
        s32 x = 0xB0;
        Object_GetById(26)->facing = x << 8;
    }
    Engine_ActorSetPosition(27, 0xBA0000, 0x27B0000);
    Object_GetById(27)->facing = m;
    Engine_ActorSetPosition(22, 0, 0);
    Engine_ActorSetPosition(23, 0, 0);
    Engine_ActorSetPosition(20, 0, 0);
}

void FuneKanpan_SetupRaisedCrewman(void)
{
    struct FieldActor *record;
    u8 bits;

    Engine_EventBegin();
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScript);
    Engine_TaskWait(1);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(23, 0xee0000, 0x2720000);
    Actor_SetPosition(22, 0xcc0000, 0x2090000);
    record = Object_GetById(22);
    record->y.fixed = 0x100000;
    bits = 128;
    {
        struct FieldActor *record = Object_GetById(22);
        u8 value = record->collision_flags;

        record->collision_flags = value | bits;
    }
    Actor_SetSpeed(22, 0x9999, 0x4ccc);
    Engine_ActorEnableActionCallback(22, FuneKanpan_LeadActionsA);
    {
        struct FieldActor *record = Object_GetById(21);

        bits |= record->collision_flags;
        record->collision_flags = bits;
    }
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Engine_ActorEnableActionCallback(21, FuneKanpan_LeadActionsB);
    if (GameFlag_IsSet(0x109) != 0) {
        FuneKanpan_PlaceLeaderOnDeck();
    }
    Engine_EventEnd();
}

void FuneKanpan_SetupDeckCrewman(void)
{
    struct FieldActor *record;

    Engine_EventBegin();
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScript);
    Engine_TaskWait(1);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(23, 0xee0000, 0x2720000);
    Actor_SetPosition(22, 0x10c0000, 0x2a60000);
    record = Object_GetById(22);
    {
        u16 shown = 0;

        record->facing = shown;
    }
    Engine_ActorEnableActionCallback(22, FuneKanpan_LeadActionsC);
    {
        struct FieldActor *record = Object_GetById(21);
        record->collision_flags = 128 | record->collision_flags;
    }
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Engine_ActorEnableActionCallback(21, FuneKanpan_LeadActionsB);
    if (GameFlag_IsSet(0x109) != 0) {
        FuneKanpan_PlaceLeaderOnDeck();
    }
    Engine_EventEnd();
}

void FuneKanpan_PlaceDeckActors(s32 a0, s32 a1)
{
    struct FieldActor *record;
    s32 v5;

    *(s32 *)((s32)gMapWork[0] + 236) = 0x410000;
    Engine_EventBegin();
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScriptE);
    Engine_TaskWait(1);
    Engine_ActorDestroy(24);
    Actor_SetPosition(23, 0xee0000, 0x2720000);
    v5 = 192;
    record = Object_GetById(23);
    record->facing = (v5 << 6);
    if (Engine_GameFlagIsSet(0x903) != 0) {
        Actor_SetPosition(22, 0xa20000, 0x27a0000);
        record = Object_GetById(22);
        record->facing = (v5 << 6);
        Actor_SetPosition(21, 0xa20000, 0x2a40000);
        record = Object_GetById(21);
        {
            s32 facing = 0xd000; /* FAKEMATCH: word temporary keeps the facing as movs+lsls */

            record->facing = facing;
        }
    } else {
        Engine_ActorSetPosition(22, 0xa00000, 0x28c0000);
        record = Object_GetById(22);
        record->facing = (v5 << 6);
        Actor_SetPosition(21, 0xa60000, 0x29c0000);
        record = Object_GetById(21);
        {
            s32 facing = 0xb000; /* FAKEMATCH: word temporary keeps the facing as movs+lsls */

            record->facing = facing;
        }
    }
    if (((struct GameState *)gGameState)->entrance == 6) {
        FuneKanpan_RunRobinTalk();
    }
    Engine_EventEnd();
}

void FuneKanpan_SetupHiddenLeaderDeparture(void)
{
    struct FieldActor *record;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(24, 0, 0);
    Actor_SetPosition(25, 0, 0);
    Actor_SetPosition(26, 0, 0);
    Actor_SetPosition(27, 0, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(23, 0, 0);
    record = Object_GetById(23);
    {
        u16 shown = 0x3000;

        record->facing = shown;
    }
    Actor_SetPosition(21, 0xe80000, 0x28a0000);
    record = Object_GetById(21);
    {
        s32 shown = 0xb000;

        record->facing = shown;
    }
    Camera_MoveTo(0xe80000, -1, 0x27c0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    FuneKanpan_AnnouncePreparationsReady(23, 21);
}

void FuneKanpan_SetupVisibleLeaderDeparture(void)
{
    struct FieldActor *record;

    Engine_EventBegin();
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScript);
    Engine_TaskWait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xe80000, 0x27c0000);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_TaskWait(1);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_ActorStop(22);
    Engine_ActorStop(21);
    Engine_TaskWait(1);
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(20, 0, 0);
    record = Object_GetById(20);
    {
        u16 shown = 0x3000;

        record->facing = shown;
    }
    Actor_SetPosition(23, 0xe80000, 0x28a0000);
    record = Object_GetById(23);
    {
        s32 shown = 0xb000;

        record->facing = shown;
    }
    Engine_TaskWait(1);
    FuneKanpan_AnnouncePreparationsReady(20, 23);
}

void FuneKanpan_AnnouncePreparationsReady(s32 a0, s32 a1)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    FuneKanpan_OpenFirstDoor();
    Actor_SetPosition(a0, 0xd80000, 0x24c0000);
    Actor_SetSpeed(a0, 0xcccc, 0x6666);
    Actor_WalkToAndWait(a0, 216, 0x258);
    Actor_WalkToAndWait(a0, 218, 0x25c);
    Actor_WalkToAndWait(a0, 234, 0x25c);
    Actor_WalkToAndWait(a0, 236, 0x26a);
    Actor_FaceDirection(a0, 0x5000, 20);
    Engine_ActorSetAnimationAndWait(a0, 3);
    Engine_EventWait(20);
    FieldScene_CallPairWith10(a1, 0x5000);
    Engine_ActorJump(a1, 4, 40);
    Engine_ActorStartRepeatedMotion(a1, 2);
    Engine_EventSetMessage((s32)MsgFunePreparationsReady);
    Event_ShowMessageAndWait(a1, 0, 20);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(10);
}

void FieldScene_RunActorTwentyDialogueSequence(void)
{
    Engine_EventBegin();
    Event_CallWithLastActiveObjectId((s32)FuneKanpan_CrewScript);
    Engine_TaskWait(1);
    Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(20, 1);
    Engine_EventSetMessage((s32)MsgFuneAyeCaptainSeaMonsters);
    Engine_EventShowMessageAndWait(20, 0, 10);
    FieldScene_CallPairWith10(22, 0x5000);
    Engine_ActorJump(22, 4, 20);
    Engine_ActorStartRepeatedMotion(22, 2);
    Engine_EventShowMessageAndWait(0x6016, 0, 20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(11);
}

void FieldScene_RunShipDeckEventScript(void)
{
    s32 v;

    Engine_EventBegin();
    Event_CallWithLastActiveObjectId((s32)FuneKanpan_CrewScript);
    Engine_TaskWait(1);
    Event_CallWithLastActiveObjectId((s32)FuneKanpan_CrewScriptB);
    Engine_TaskWait(1);
    Call3(Engine_ActorSetPosition, 21, 16252928, 47710208);
    Engine_ActorSetChildValue(0, 15);
    ((void (*)(void*,s32))Engine_ActorSetSpriteFlags)(Object_GetById(0), 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    ((void * (*)())Engine_EventOpenScreen)();
    Actor_SetSpeed(21, 104857, 52428);
    Call3(Engine_ActorWalkToAndWait, 21, 242, 692);
    Call3(Engine_ActorWalkToAndWait, 21, 196, 678);
    Call3(Engine_ActorWalkToAndWait, 21, 182, 654);
    Engine_ActorRunRepeatedMotion(21, 2);
    Engine_EventSetMessage((s32)MsgFuneNoUseLate);
    FieldScene_RunStepThen10(40981);
    Actor_SetSpeed(0, 157286, 78643);
    FieldObjectMotion_ResetAndSetPosition(0, 154, 609);
    Engine_AudioPlayCue(146);
    v = 0;
    Object_GetById(24)->timer = v;
    Object_GetById(25)->timer = v;
    Object_GetById(26)->timer = v;
    Call3(Engine_ActorSetPosition, 24, 2097152, 31719424);
    Call3(Engine_ActorSetPosition, 25, 5505024, 32505856);
    Call3(Engine_ActorSetPosition, 26, 1048576, 39059456);
    Actor_SetSpeed(24, 157286, 78643);
    Actor_SetSpeed(25, 157286, 78643);
    Actor_SetSpeed(26, 157286, 78643);
    v = (s32)FuneKanpan_DeckEventActions;
    Engine_ActorEnableActionCallback(24, v);
    Engine_ActorEnableActionCallback(25, v);
    Engine_ActorEnableActionCallback(26, v);
    Engine_ActorSetChildValue(24, 3);
    Engine_ActorSetChildValue(25, 3);
    Engine_ActorSetChildValue(26, 3);
    do {
        Engine_TaskWait(1);
    } while (*(s16 *)&Object_GetById(24)->timer == 0);
    FuneKanpan_OpenFirstDoor();
    /* FAKEMATCH: the do/while sets r0 = 21 first, straight after the call. */
    do {
        Call3(Engine_ActorWalkTo, 21, 196, 612);
    } while (0);
    Object_RefreshSelectorById(24);
    Engine_EventWait(10);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(10);
    Graphics_EnableObjLayerAndCallbacks();
    Ui_SetRenderResultFromObject(21);
    UiText_ShowCenteredMessage((s32)MsgFuneArrgh, 1, 0);
    ObjectDispatch_StopCallbacksAndHideLayers();
    Engine_EventRequestExit(12);
}

void FuneKanpan_RunDeckCrewScene(void)
{

    Engine_EventBegin();
    Engine_ActorSetChildValue(0, 15);
    Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScript);
    Engine_TaskWait(1);
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScriptC);
    Engine_TaskWait(1);
    Engine_ActorSetAnimation(31, 0);
    Engine_ActorSetSpriteFlags(Object_GetById(24), 1);
    Engine_ActorSetSpriteFlags(Object_GetById(25), 1);
    Engine_ActorSetSpriteFlags(Object_GetById(26), 1);
    Engine_ActorSetSpriteFlags(Object_GetById(27), 1);
    Engine_ActorSetSpriteFlags(Object_GetById(28), 1);
    Engine_ActorSetSpriteFlags(Object_GetById(29), 1);
    Actor_SetPosition(22, 0x1000000, 0x2800000);
    Engine_ActorEnableActionCallback(22, FuneKanpan_CrewActionsD);
    Actor_SetPosition(21, 0x10c0000, 0x2b40000);
    Engine_ActorEnableActionCallback(22, FuneKanpan_CrewActionsD);
    Actor_SetPosition(24, 0xf20000, 0x25c0000);
    Actor_SetPosition(25, 0x1080000, 0x2580000);
    Actor_SetPosition(26, 0xfe0000, 0x29c0000);
    Engine_ActorSetPosition(27, 0x11a0000, 0x2920000);
    Object_GetById(24)->rise_enabled = 0;
    Object_GetById(25)->rise_enabled = 1;
    Object_GetById(26)->rise_enabled = 0;
    Object_GetById(27)->rise_enabled = 1;
    Engine_ActorEnableActionCallback(24, FuneKanpan_CrewActionsB);
    Engine_ActorEnableActionCallback(25, FuneKanpan_CrewActionsB);
    Engine_ActorEnableActionCallback(26, FuneKanpan_CrewActionsA);
    Engine_ActorEnableActionCallback(27, FuneKanpan_CrewActionsA);
    Engine_ActorSetPosition(20, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(0x190);
    Actor_SetPosition(28, 0xfe0000, 0x2e40000);
    Actor_SetPosition(29, 0x180000, 0x24a0000);
    Actor_SetSpeed(28, 0x19999, 0xcccc);
    Actor_SetSpeed(29, 0x19999, 0xcccc);
    Actor_SetDestination(29, 172, 0x284);
    FieldObject_MotionSetPositionAndCommit(28, 200, 0x294);
    Actor_SetSpeed(0, 0x40000, 0x20000);
    Actor_SetDestination(0, 174, 0x26c);
    Engine_ObjectMotionSetPositionAndCommit(28, 180, 0x244);
    Engine_AudioPlayCue(146);
    Engine_ActorEnableActionCallback(28, FuneKanpan_CrewActionsC);
    Engine_ActorEnableActionCallback(29, FuneKanpan_CrewActionsC);
    Engine_AudioPlayCue(240);
    Actor_SetPosition(31, 0x860000, 0x2520000);
    Engine_ActorEnableActionCallback(31, FuneKanpan_CrewActionsE);
    Engine_EventWait(10);
    Actor_SetPosition(30, 0x860000, 0x2480000);
    Actor_SetSpeed(30, 0x40000, 0x20000);
    Object_GetById(30)->velocity_y = 0x80000;
    Engine_ObjectMotionSetPositionAndCommit(30, 186, 0x264);
    Engine_ActorSetSpriteFlags(Object_GetById(30), 1);
    Engine_EventWait(10);
    Actor_SetSpeed(30, 0x20000, 0x10000);
    Engine_ObjectMotionSetPositionAndCommit(30, 216, 0x258);
    FieldScene_CallPairWith10(30, 0xc000);
    FuneKanpan_OpenFirstDoor();
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(30, FuneKanpan_SailorActions);
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(28, FuneKanpan_SailorActions);
    Engine_EventWait(10);
    Object_SetActionCallbackAndRefreshById(29, FuneKanpan_SailorActions);
    Engine_EventWait(20);
    Engine_AudioPlayCue(147);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_ActorStop(24);
    Engine_ActorStop(25);
    Engine_ActorStop(26);
    Engine_ActorStop(27);
    Engine_EventWait(10);
    Graphics_EnableObjLayerAndCallbacks();
    Ui_SetRenderResultFromObject(21);
    UiText_ShowCenteredMessage(MsgFuneArrgh, 1, 0);
    ObjectDispatch_StopCallbacksAndHideLayers();
    Engine_EventRequestExit(13);
}

void FieldScene_RunActorSequence(void)
{
    u32 i;
    struct FieldActor *rec8;
    struct FieldActor *record;
    s32 base5_200c8c4;
    s32 base5_200c8b0;
    s32 base5_200c8d8;
    s32 base5_0;
    const u8 *base5_200c888;

    Engine_EventBegin();
    Engine_ActorSetChildValue(0, 15);
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScript);
    Engine_TaskWait(1);
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScriptD);
    Engine_TaskWait(1);
    Engine_ActorSetPosition(22, 0xb00000, 0x2b80000);
    record = Object_GetById(22);
    {
        s32 shown = 0xd000;

        record->facing = shown;
    }
    Engine_ActorSetPosition(21, 0x1080000, 0x2960000);
    record = Object_GetById(21);
    {
        s32 shown = 0xb000;

        record->facing = shown;
    }
    Actor_SetPosition(24, 0xb80000, 0x2a00000);
    Actor_SetPosition(25, 0xca0000, 0x2b40000);
    Actor_SetPosition(26, 0xfc0000, 0x2860000);
    Actor_SetPosition(27, 0x1000000, 0x2ae0000);
    Actor_SetPosition(28, 0xac0000, 0x2780000);
    Engine_ActorSetPosition(29, 0x1000000, 0x26e0000);
    {
        /* FAKEMATCH: halfword zero retains the short literal-pool reach. */
        struct { u16 v; } zero;

        zero.v = 0;
        Object_GetById(24)->rise_enabled = zero.v;
        Object_GetById(25)->rise_enabled = 1;
        Object_GetById(26)->rise_enabled = zero.v;
        Object_GetById(27)->rise_enabled = 2;
    }
    Engine_ActorSetPosition(20, 0, 0);
    Engine_ActorEnableActionCallback(24, FuneKanpan_DeckActionsA);
    Engine_ActorEnableActionCallback(25, FuneKanpan_DeckActionsA);
    Engine_ActorEnableActionCallback(26, FuneKanpan_DeckActionsB);
    Engine_ActorEnableActionCallback(27, FuneKanpan_DeckActionsB);
    Engine_ActorEnableActionCallback(28, FuneKanpan_DeckActionsC);
    Engine_ActorEnableActionCallback(29, FuneKanpan_DeckActionsC);
    Engine_ActorSetChildValue(24, 3);
    Engine_ActorSetChildValue(25, 3);
    Engine_ActorSetChildValue(26, 3);
    Engine_ActorSetChildValue(27, 3);
    Engine_ActorSetChildValue(28, 3);
    Engine_ActorSetChildValue(29, 3);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(80);
    Engine_AudioPlayCue(147);
    rec8 = Object_GetById(31);
    rec8->scale_x = 0x1999;
    rec8->scale_y = 0x1999;
    rec8->x.fixed = 0xc20000;
    rec8->z.fixed = 0x2820000;
    for (base5_0 = 0; (u32)base5_0 < 16; base5_0++) {
        rec8->scale_x += 0xf5c;
        rec8->scale_y += 0xf5c;
        Engine_TaskWait(1);
    }
    rec8 = Object_GetById(30);
    rec8->scale_x = 0x11999;
    rec8->scale_y = 0x11999;
    rec8->x.fixed = 0xc20000;
    rec8->y.fixed = 0x500000;
    rec8->z.fixed = 0x2820000;
    {
        u16 shown = 0x5000;

        rec8->facing = shown;
    }
    rec8->bounce = 0x6666;
    rec8->gravity = 0x20000;
    Engine_EventWait(80);
    Engine_AudioPlayCue(147);
    Engine_ActorSetPosition(31, 0, 0);
    record = Object_GetById(30);
    Engine_ActorSetSpriteFlags(record, 1);
    Actor_SetSpeed(0, 0x19999, 0xcccc);
    rec8 = Object_GetById(0);
    {
        /* FAKEMATCH: halfword zero retains the short literal-pool reach. */
        struct { u16 v; } zero;

        zero.v = 0;
        rec8->motion_flags = zero.v;
    }
    Actor_SetDestination(0, 216, 0x264);
    Actor_SetSpeed(30, 0x19999, 0xcccc);
    FieldObject_MotionSetPositionAndCommit(30, 196, 0x258);
    FieldObject_MotionSetPositionAndCommit(30, 216, 0x258);
    Engine_ActorStop(28);
    Engine_TaskWait(1);
    Actor_SetSpeed(28, 0x19999, 0xcccc);
    base5_200c888 = FuneKanpan_SailorActions;
    Engine_ActorEnableActionCallback(28, base5_200c888);
    FieldScene_CallPairWith10(30, 0xd000);
    FuneKanpan_OpenFirstDoor();
    Engine_EventWait(10);
    Engine_ActorEnableActionCallback(30, base5_200c888);
    Engine_ActorStop(29);
    Engine_TaskWait(1);
    Actor_SetSpeed(29, 0x19999, 0xcccc);
    Object_SetActionCallbackAndRefreshById(29, base5_200c888);
    Engine_EventWait(20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_ActorStop(24);
    Engine_ActorStop(25);
    Engine_ActorStop(26);
    Engine_ActorStop(27);
    Engine_ActorStop(28);
    Engine_ActorStop(29);
    Engine_EventWait(10);
    Graphics_EnableObjLayerAndCallbacks();
    Ui_SetRenderResultFromObject(21);
    UiText_ShowCenteredMessage((s32)MsgFuneArrgh, 1, 0);
    ObjectDispatch_StopCallbacksAndHideLayers();
    Engine_EventRequestExit(14);
}

void FuneKanpan_RunJumpScene(void)
{
    struct FieldActor *actor;

    Engine_EventBegin();
    Event_CallWithLastActiveObjectId((s32)FuneKanpan_CrewScriptE);
    Engine_TaskWait(1);
    Engine_CameraFollowActor(25, 1);
    Engine_TaskWait(1);
    Engine_ActorSetAnimation(21, 5);
    Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    actor->facing = FACING_SOUTH;
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    FuneKanpan_OpenFirstDoor();
    Engine_EventWait(10);
    Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(216), PIXELS(586));
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 216, 598);
    Engine_EventWait(20);
    FieldScene_CallPairWith10(0, 0x6000);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 194, 624);
    Engine_AudioPlayCue(181);
    Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    Engine_EventWait(20);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    Object_GetById(25)->motion_flags = 0;
    Actor_SetSpeed(25, 0x20000, 0x10000);
    Engine_ActorSetDestination(25, 216, 612);
    Engine_AudioPlayCue(149);
    Engine_ActorSetSpritePriority(22, 2);
    Engine_ActorSetAnimation(22, 5);
    actor = Object_GetById(22);
    actor->velocity_y = 0x80000;
    actor->gravity = 0xb333;
    actor->scale_x = 0x1a000;
    actor->scale_y = 0x1a000;
    actor->update = (void (*)(union FieldObject *))OverlayObject_DecayFields24And28;
    actor->bounce = 0x8000;
    Actor_SetSpeed(22, 0x60000, 0x30000);
    Engine_ObjectMotionSetPositionAndCommit(22, 182, 0x26a);
    Engine_ActorSetSpriteFlags(Object_GetById(22), 0);
    FieldScene_CallPairWith10(0, 0xa000);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 80);
    Actor_SetDestination(25, 232, 564);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 204, 610);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 208, 598);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 598);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
        Engine_ActorSetPosition(GERALD, actor->x.fixed, actor->z.fixed);
    }
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
        Engine_ActorSetPosition(IWAN, actor->x.fixed, actor->z.fixed);
    }
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
        Engine_ActorSetPosition(MEARI, actor->x.fixed, actor->z.fixed);
    }
    Actor_SetSpeed(GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(IWAN, 0x10000, 0x8000);
    Actor_SetSpeed(MEARI, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 250, 584);
    Actor_WalkTo(GERALD, 240, 600);
    Actor_WalkTo(IWAN, 254, 600);
    Engine_ActorWalkToAndWait(MEARI, 248, 616);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_ActorSetAnimation(GERALD, 1);
    Engine_ActorSetAnimation(IWAN, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    Engine_ActorFaceDirection(MEARI, FACING_NORTH, 20);
    Engine_AudioPlayCue(149);
    Engine_EventWait(40);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Actor_SetAttachedEffect(IWAN, 0x102);
    Engine_ActorSetAttachedEffect(MEARI, 0x102);
    Engine_EventWait(60);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
    Actor_SetSpeed(MEARI, 0xcccc, 0x6666);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 564);
    Actor_WalkTo(GERALD, 248, 564);
    Engine_EventWait(20);
    Actor_WalkTo(IWAN, 248, 564);
    Actor_WalkTo(MEARI, 248, 564);
    Engine_EventWait(20);
    ((struct GameState *)gGameState)->saved_scene = (s32)&SceneId_FuneHeya;
    /* FAKEMATCH: the one-pass block keeps the entrance store ahead of the battle start's address; a plain store is scheduled after it. */
    do {
        ((struct GameState *)gGameState)->saved_entrance = 30;
    } while (0);
    ((struct GameState *)gGameState)->battle_start = 3;
    Party_SetFields1ceAnd1d0((s32)&SceneId_FuneKanpan, 16);
    BattleFx_SetWeightedResult(62, 3);
    Engine_EventEnd();
}

void FieldScene_ConfigureLeadActors(void)
{
    struct FieldActor *record;

    Engine_EventBegin();
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScript);
    Engine_TaskWait(1);
    Actor_SetPosition(20, 0xb60000, 0x26a0000);
    Actor_SetPosition(23, 0xee0000, 0x2720000);
    Actor_SetPosition(22, 0x10c0000, 0x2a60000);
    record = Object_GetById(22);
    {
        u16 shown = 0;

        record->facing = shown;
    }
    Engine_ActorEnableActionCallback(22, FuneKanpan_LeadActionsC);
    {
        struct FieldActor *record = Object_GetById(21);
        record->collision_flags = 128 | record->collision_flags;
    }
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Engine_ActorEnableActionCallback(21, FuneKanpan_LeadActionsB);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Actor_SetSpeed(20, 0x19999, 0xcccc);
    Actor_WalkToAndWait(20, 182, 0x224);
    FieldScene_CallPairWith10(20, 0);
    FieldScene_CallPairWith10(0, 0x8000);
    Engine_ActorRunRepeatedMotion(20, 1);
    Engine_EventSetMessage((s32)MsgFuneThankRobinDidGoodAgainst);
    FieldScene_RunStepThen10(20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(40);
    Actor_FaceDirection(20, 0x5000, 20);
    Actor_ShowEmote(20, 0x105, 60);
    Event_ShowMessageAndWait(20, 0, 40);
    FieldScene_CallPairWith10(20, 0);
    FieldScene_RunStepThen10(20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(20, 3);
    Actor_WalkToAndWait(20, 182, 0x258);
    Actor_WalkToAndWait(20, 216, 0x258);
    FieldScene_CallPairWith10(20, 0xc000);
    FuneKanpan_OpenFirstDoor();
    Engine_EventWait(10);
    Actor_WalkToAndWait(20, 216, 0x244);
    Actor_SetPosition(20, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    GameFlag_Set(0x92b);
    GameFlag_Clear(0x302);
    Engine_EventEnd();
}

void FieldScene_ConfigureThreeActors(void)
{
    u32 i;
    struct FieldActor *record;

    Engine_EventBegin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Event_CallWithLastActiveObjectId((u32)FuneKanpan_CrewScript);
    Engine_TaskWait(1);
    Actor_SetPosition(20, 0xc40000, 0x1f60000);
    record = Object_GetById(20);
    record->facing = 0xa000;
    Actor_SetPosition(22, 0xb80000, 0x20c0000);
    record = Object_GetById(22);
    record->facing = 0xb000;
    Engine_ActorSetSpritePriority(21, 1);
    Actor_SetPosition(21, 0xb80000, 0x2780000);
    record = Object_GetById(21);
    record->facing = 0xb000;
    /* FAKEMATCH: needed to match: the work pointer is read as a word, which may alias the facing stores, so they stay ahead of its load. */
    *(s32 *)(*(s32 *)&gEventWork + 0x1c0) = 0x202;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_ActorJump(22, 4, 10);
    Engine_ActorJump(22, 6, 20);
    Engine_EventSetMessage((s32)MsgFuneCanSeeLand);
    FieldScene_RunStepThen10(22);
    Engine_ActorSetAnimationAndWait(20, 3);
    Actor_SetSpeed(21, 0x30000, 0x18000);
    Actor_WalkToAndWait(21, 180, 0x222);
    Engine_ActorFaceDirection(21, 0xb000, 40);
    Engine_ActorRunRepeatedMotion(21, 1);
    FieldScene_RunStepThen10(21);
    Engine_EventRequestExit(15);
}

void FieldScene_RunPartyRosterScene(void)
{
    s32 roster[4];
    s32 matched[4];
    s32 category[4];
    u32 index;
    s32 found;
    s32 actor;
    s32 member;
    struct FieldActor *record;

    found = 0;
    for (index = 0; index <= 3; index++) {
        roster[index] = SceneState_FindFirstSetFlagOfGroup((s32)index);
        category[index] = 0;
    }

    member = 23;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    member = 24;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    member = 25;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    member = 27;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    if (found == 4)
        goto scene;
    member = 28;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    if (found == 4)
        goto scene;
    member = 29;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = found;
            found++;
            break;
        }
    }
    if (found == 4)
        goto scene;
    member = 26;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = 10;
            found++;
            break;
        }
    }
    if (found == 4)
        goto scene;
    member = 30;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = 11;
            found++;
            break;
        }
    }
    if (found == 4)
        goto scene;
    member = 31;
    for (index = 0; index <= 3; index++) {
        if (roster[index] == member) {
            matched[found] = member;
            category[found] = 20;
            break;
        }
    }

scene:
    Engine_EventBegin();
    Engine_ActorSetChildValue(0, 15);
    Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetPosition(32, record->x.fixed, record->z.fixed);
    }
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_CameraFollowActor(32, 1);

    FuneKanpan_OpenFirstDoor();

    Engine_EventWait(10);

    for (index = 0; index <= 3; index++) {
        actor = matched[index];
        Actor_SetPosition(actor, 216 << 16, 146 << 18);
        if (category[index] == 20) {
            Actor_SetSpeed(actor, 0xcccc, 0x6666);
        } else {
            Actor_SetSpeed(actor, 128 << 9, 128 << 8);
        }
        Actor_WalkToAndWait(actor, 216, 150 << 2);
        Actor_WalkToAndWait(actor, 192, 0x26a);
        Engine_ActorWalkToAndWait(actor, 192, 164 << 2);

        switch (category[index]) {
        case 0:
            Engine_ActorShowEmote(actor, 129 << 1, 60);
            Engine_EventSetMessage((s32)MsgFuneMadeFinallyLucky);
            break;
        case 1:
            FieldScene_CallPairWith10(actor, 208 << 8);
            Engine_ActorShowEmote(actor, 129 << 1, 60);
            Engine_EventSetMessage((s32)MsgFuneOtherPassengersAlready);
            break;
        case 2:
            Engine_ActorShowEmote(actor, 0x105, 60);
            Engine_EventSetMessage((s32)MsgFuneDontRowAnymore);
            break;
        case 3:
            Engine_ActorRunRepeatedMotion(actor, 1);
            Engine_EventSetMessage((s32)MsgFuneOhhhHaventWorkout);
            break;
        case 10:
            Engine_ActorSetAnimationAndWait(actor, 3);
            Engine_EventSetMessage((s32)MsgFuneRowingShipMore);
            break;
        case 11:
            Engine_ActorSetAnimation(actor, 4);
            Engine_EventSetMessage((s32)MsgFuneTotallyLostOcean);
            break;
        case 20:
            Engine_ActorSetAnimationAndWait(actor, 4);
            Engine_ActorShowEmote(actor, 0x107, 40);
            Engine_EventSetMessage((s32)MsgFuneMakeOldMan);
            break;
        default:
            break;
        }

        FieldScene_RunStepThen10(actor);
        Engine_ActorEnableActionCallback(actor, FuneKanpan_RosterActions);
    }

    Object_RefreshSelectorById(actor);
    Engine_EventWait(40);
    Engine_ActorSetPosition(0, 216 << 16, 146 << 18);
    Engine_TaskWait(1);
    Engine_ActorSetChildValue(0, 0);
    Engine_ActorSetSpriteFlags(Object_GetById(0), 1);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_WalkToAndWait(0, 216, 150 << 2);
    Actor_WalkToAndWait(0, 190, 153 << 2);
    Engine_EventSetMessage((s32)MsgFuneThanksHardWork);
    FieldScene_RunStepThen10(20);
    Engine_ActorFaceDirection(0, 192 << 8, 0);

    Object_GetById(32)->motion_flags = 0;
    Actor_SetSpeed(32, 128 << 10, 128 << 9);
    Actor_SetDestination(32, 196, 141 << 2);

    Actor_SetSpeed(20, 0xcccc, 0x6666);
    Actor_WalkToAndWait(20, 182, 0x22b);
    Actor_FaceDirection(20, 192 << 6, 20);
    Engine_ActorRunRepeatedMotion(20, 1);
    FieldScene_RunStepThen10(20);
    Engine_ActorSetAnimationAndWait(20, 3);
    FieldScene_RunStepThen10(20);
    Engine_ActorFaceDirection(20, 128 << 8, 40);
    FieldScene_RunStepThen10(20);
    FieldScene_CallPairWith10(20, 192 << 6);
    FieldScene_RunStepThen10(20);
    Engine_ActorSetAnimationAndWait(20, 3);
    Engine_CameraFollowActor(0, 1);
    Actor_WalkToAndWait(20, 188, 128 << 2);
    Engine_ActorFaceDirection(20, 192 << 6, 0);
    Engine_ActorSetPosition(32, 0, 0);

    Engine_GameFlagClear(0x12f);
    Engine_EventEnd();
}

s32 SceneState_FindFirstSetFlagOfGroup(u32 sel)
{
    s32 v = 0;
    s32 id = 23;
    u32 i;

    switch (sel) {
    case 0:
        v = 0x92C;
        break;
    case 1:
        v = 0x935;
        break;
    case 2:
        v = 0x917;
        break;
    case 3:
        v = 0x990;
        break;
    }
    for (i = 0; i < 9; i++) {
        if (Engine_GameFlagIsSet(v)!= 0) return id;
        v++;
        id++;
    }
    return 0;
}

void FuneKanpan_RockDeck(void)
{
    /* FAKEMATCH: the second angle sum is held across the first angle mask to retain the current store order. */

    struct MapWork *map = ((struct MapWork *)gMapWork[0]);
    s32 *camera = map->camera;
    s32 dx = Engine_MathCos(FuneKanpan_WaveAngleX);
    s32 dy = Engine_MathSin(FuneKanpan_WaveAngleY);
    struct MapLayer *layer;

    *camera++ += dx >> 1;
    *camera += dy;
    FuneKanpan_WaveAngleX += (u32)(Engine_RandomNext() * 3 << 7) >> 16;
    {
        s32 turn = FuneKanpan_WaveAngleY + ((u32)(Engine_RandomNext() << 9) >> 16);

        FuneKanpan_WaveAngleX &= 0xffff;
        FuneKanpan_WaveAngleY = turn & 0xffff;
    }
    layer = &map->layers[5];
    layer->x = FuneKanpan_LayerScroll[0];
    FuneKanpan_LayerScroll[0] -= FuneKanpan_LayerSpeed[0];
    if (FuneKanpan_LayerScroll[0] < 0) {
        FuneKanpan_LayerScroll[0] += 0x200000;
    }
    if (FuneKanpan_LayerScroll[0] > 0x200000) {
        FuneKanpan_LayerScroll[0] -= 0x200000;
    }
    layer->y = FuneKanpan_LayerScroll[1];
    FuneKanpan_LayerScroll[1] -= FuneKanpan_LayerSpeed[1];
    if (FuneKanpan_LayerScroll[1] < 0) {
        FuneKanpan_LayerScroll[1] += 0x200000;
    }
}
