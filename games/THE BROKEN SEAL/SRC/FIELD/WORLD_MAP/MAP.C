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
#include "STORY.H"
#include "TYPES.H"
#include "FIELD_SERVICE.H"
#include "IWRAM_CALL.H"
#include "SCENE_IDS.H"
#include "CALL.H"
#include "FIELD_EVENT.H"
#include "text/MSG_IDS.H"

void Vector_AddPolarOffset(s32 radius, s32 angle, union FieldCoordinate *pos);

enum {
    CONTACT_LAST_ACTOR = 65,
    CONTACT_TRIGGER_BASE = 100,
    FLAG_CONTACT_PAUSED = 0x163,
    FLAG_CONTACT_BLOCKED = 0x104
};

extern u8 gDebugMode[];

void Event_SetDestination(s32 a0, s32 a1);
void PartyInventory_Discard(s32 item);
void RunEventScript01(void);


void StoryProgress_TriggerEvent0808(void)
{
    struct EventWork *work = gEventWork;

    if (gGameState.encounter_steps >= Math_Divide(work->encounter_threshold * 9, 10)) {
        if ((u32)Engine_RandomNext() < 0x8000) {
            BattleFx_SetPhaseRequest(0x808, 3);
            work->encounter_bias = 0;
        } else {
            gGameState.encounter_steps = work->encounter_threshold;
        }
    }
}

void StoryProgress_TriggerEvent0809(void)
{
    struct GameState *state = &gGameState;
    s32 *steps = &state->encounter_steps;
    struct EventWork *work = gEventWork;

    if (*steps >= Math_Divide(work->encounter_threshold * 9, 10)) {
        BattleFx_SetPhaseRequest(0x809, 42);
        work->encounter_bias = 0;
    }
}

void StoryProgress_TriggerEvent080A(void)
{
    struct GameState *state = &gGameState;
    s32 *steps = &state->encounter_steps;
    struct EventWork *work = gEventWork;

    if (*steps >= Math_Divide(work->encounter_threshold * 9, 10)) {
        BattleFx_SetPhaseRequest(0x80a, 24);
        work->encounter_bias = 0;
    }
}

void StoryActor_AdvanceTimer(struct FieldActor *actor)
{
    if ((s16)actor->timer <= 0) {
        actor->timer++;
    } else {
        Engine_ObjectDispatchRelease(actor);
    }
}

void StoryActor_ConfigureSpawnedObject(struct FieldActor *actor)
{
    struct FieldActor *spark;
    s32 x;
    s32 y;
    s32 z;

    if (gFrameCount & 4) {
        actor->scale_x = 0x14ccc;
        actor->scale_y = 0x14ccc;
    } else {
        actor->scale_x = 0x10000;
        actor->scale_y = 0x10000;
    }
    if (!(gFrameCount & 2))
        return;
    x = actor->x.fixed;
    y = actor->y.fixed;
    z = actor->z.fixed;
    spark = Engine_ObjectCreate(0x11d, x, y, z);
    Engine_AudioPlayCue(0xf6);
    if (spark == 0)
        return;
    spark->motion_flags = 0;
    ((struct FieldSpritePriority *)spark->sprite)->priority = 1;
    Engine_ActorSetSpriteFlags(spark, 0);
    Object_SetMode(spark, 1);
    spark->timer = 0;
    spark->update = (void (*)(union FieldObject *))StoryActor_AdvanceTimer;
}

void WorldMap_UpdateBobbingMarker(struct FieldActor *actor)
{
    s16 *angle;

    if (gFrameCount & 2)
        Engine_ObjectSetPartPalettes(actor, 10);
    else
        Engine_ObjectSetPartPalettes(actor, 7);
    if ((s16)actor->work == 0) {
        angle = (s16 *)&actor->timer;
        actor->x.fixed = 0x15d00000;
        actor->y.fixed = Iwram_MulQ16(Engine_MathSin(*angle << 3), 0x40000) + 0x100000;
        actor->z.fixed = 0x5300000;
        Vector_AddPolarOffset(0x100000, *angle, &actor->x);
        actor->facing = *angle + 0x4000;
        *angle += 0x400;
    }
}

s32 StoryActor_Initialize(struct FieldActor *actor)
{
    if (Engine_GameFlagIsSet(0x30) != 0) {
        return 0;
    }
    if (Engine_GameFlagIsSet(0x16E) != 0) {
        return 0;
    }
    actor->update = (void (*)(union FieldObject *))WorldMap_UpdateBobbingMarker;
    actor->motion_flags = 0;
    actor->timer = 0;
    actor->work = 0;
    actor->scale_x = 0x8000;
    actor->scale_y = 0x8000;
    return 0;
}

u8 *WorldMap_GetEntrances(void)
{
    return gWorldMapEntrances;
}

s32 WorldMap_GetRegions(void)
{
    return 0;
}

u8 *WorldMap_GetExits(void)
{
    return gWorldMapExits;
}

s32 StoryActor_ApplyFlaggedMode(struct FieldActor *actor)
{
    StoryActor_ApplyMapRotation(actor);
    if (Engine_GameFlagIsSet(0x847) != 0) {
        Object_SetMode(actor, 2);
    }
    return 1;
}

s32 StoryActor_ApplyMapRotation(struct FieldActor *actor)
{
    struct MapRenderWork *work = gMapWork[0];
    struct FieldSprite *sprite = actor->sprite;

    sprite->rotation = work->rotation;
    sprite->flags = 0;
    return 1;
}

s32 StoryActor_ApplyMapRotationWithCollision(struct FieldActor *actor)
{
    struct MapRenderWork *work = gMapWork[0];
    struct FieldSprite *sprite = actor->sprite;

    actor->collision_flags |= 1;
    sprite->rotation = work->rotation;
    return 1;
}

s32 StoryActor_ResetPosition(struct FieldActor *actor)
{
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ObjectSetPartPalettes(actor, 10);
    actor->collision_flags = 0;
    if (Engine_GameFlagIsSet(0x8A0) != 0) {
        Engine_GameFlagSet(0x2f1);
        actor->x.fixed = 0;
        actor->y.fixed = 0;
    }
    return 0;
}

s32 StoryActor_ClearActiveFlag(struct FieldActor *actor)
{
    if ((actor->active & 1) && (gFrameCount & 1))
        actor->active ^= 1;
    return 1;
}

struct ScenePlacement *StoryScene_SelectPlacementTable(void)
{
    switch (gGameState.entrance) {
    case 49:
        if (Engine_GameFlagIsSet(0x94f) == 0 && Engine_GameFlagIsSet(0x941) != 0) {
            return gWorldMapPlacements49;
        }
        break;
    case 64:
        if (Engine_GameFlagIsSet(0x85a) == 0) {
            return gWorldMapPlacements64;
        }
        break;
    case 65:
    case 70:
        return gWorldMapPlacements65;
    case 71:
        return gWorldMapPlacements71;
    case 72:
        return gWorldMapPlacements72;
    case 73:
        return gWorldMapPlacements73;
    case 66:
    case 67:
    case 68:
    case 69:
    case 75:
        return gWorldMapPlacements66;
    case 80:
        return gWorldMapPlacements80;
    default:
        break;
    }

    Engine_GameFlagSet(0x235);
    return gWorldMapPlacements;
}

void StoryScene_SetBranchValueFromX(s32 trigger, s32 exit_before, s32 exit_after)
{
    struct EventWork *work;
    struct FieldActor *subject;
    struct FieldActor *other;

    subject = Object_GetById(trigger - CONTACT_TRIGGER_BASE);
    other = Object_GetById(gGameState.selected_actor);
    work = gEventWork;
    if (other->x.fixed < subject->x.fixed) {
        work->requested_exit = exit_before;
    } else {
        work->requested_exit = exit_after;
    }
    Engine_AudioPlayCue(0x7B);
}

void StoryScene_SetBranchValueFromZ(s32 trigger, s32 exit_before, s32 exit_after)
{
    struct EventWork *work;
    struct FieldActor *subject;
    struct FieldActor *other;

    subject = Object_GetById(trigger - CONTACT_TRIGGER_BASE);
    other = Object_GetById(gGameState.selected_actor);
    work = gEventWork;
    if (other->z.fixed < subject->z.fixed) {
        work->requested_exit = exit_before;
    } else {
        work->requested_exit = exit_after;
    }
    Engine_AudioPlayCue(0x7B);
}

void SceneState_SetValues130_6_47(void)
{
    StoryScene_SetBranchValueFromX(0x82, 6, 0x2F);
}

void SceneState_ApplyValues150And46And11(void)
{
    StoryScene_SetBranchValueFromZ(0x96, 0x2E, 0x0B);
}

void SceneState_ApplyValues116And56And21(void)
{
    StoryScene_SetBranchValueFromZ(0x74, 0x38, 0x15);
}

void SceneState_ApplyValues151And25And54(void)
{
    StoryScene_SetBranchValueFromZ(0x97, 0x19, 0x36);
}

void FieldScene_RunStep7D3B1E(void)
{

    StoryScene_SetBranchValueFromZ(0x7D, 0x3B, 0x1E);
}

struct SceneEvent *WorldMap_GetEvents(void)
{
    return gWorldMapEvents;
}

void MapActor_UpdateContact(void)
{
    struct FieldActor *leader;
    struct EventWork *work;
    struct FieldActor *actor;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    s32 leader_reach;
    u32 i;

    leader = Object_GetById(gGameState.selected_actor);
    leader_reach = leader->sprite->scale * leader->radius;
    work = gEventWork;
    actor = work->view_center;
    left = actor->x.fixed - PIXELS(160);
    right = actor->x.fixed + PIXELS(160);
    top = actor->z.fixed - PIXELS(300);
    bottom = actor->z.fixed + PIXELS(200);

    for (i = ACTOR_FIRST_PLACED; i <= CONTACT_LAST_ACTOR; i++) {
        s32 x;
        s32 z;
        s32 dx;
        s32 reach;
        s32 scale;

        actor = Engine_ActorLookup(i);
        if (actor == NULL) {
            continue;
        }
        x = actor->x.fixed;
        z = actor->z.fixed;
        if (x < left || x > right || z < top || z > bottom) {
            actor->active = 0;
            continue;
        }
        actor->active = 1;
        if (gDebugMode[0] != 0 && Engine_GameFlagIsSet(FLAG_CONTACT_PAUSED) != 0) {
            continue;
        }
        scale = actor->sprite->scale;
        dx = actor->x.fixed - leader->x.fixed;
        if (dx < 0) {
            dx = leader->x.fixed - actor->x.fixed;
        }
        reach = leader_reach + actor->radius * scale;
        if (dx + (actor->z.fixed - leader->z.fixed < 0 ? leader->z.fixed - actor->z.fixed
                                                       : actor->z.fixed - leader->z.fixed)
                < reach
            && Engine_GameFlagIsSet(FLAG_CONTACT_BLOCKED) == 0) {
            work->touched_trigger = i + CONTACT_TRIGGER_BASE;
        }
    }
}

void SceneState_ApplyFlag85aBranch(void)
{
    if (Engine_GameFlagIsSet(0x85a) == 0) {
        Engine_EventRequestExit(101);
    } else {
        Engine_AudioPlayCue(123);
        Engine_EventRequestExit(3);
    }
}

void FieldScene_RunStep74(void)
{

    Engine_EventBegin();
    Engine_EventRequestExit(74);
}

s32 WorldMap_EnterScene(void)
{
    struct GameState *state = &gGameState;
    s16 *entrance = &state->entrance;

    if (*entrance == 99) {
        Engine_GameFlagSet(0x160);
        Engine_GameFlagSet(0x161);
        Engine_GameFlagSet(0x163);
    }
    if (*entrance == 90) {
        Scene_RunScene371SequenceA(0);
        Event_SetDestination((s32)&SceneId_MakyuriChojo1, 1);
    } else if (*entrance == 91) {
        Scene_RunScene371SequenceA(1);
        Event_SetDestination((s32)&SceneId_VinasuChojo, 93);
    } else if (*entrance == 78) {
        Engine_EventBegin();
        PartyInventory_Discard(242);
        Engine_EventRequestExit(112);
    } else {
        Engine_GameFlagSet(0x144);
        gEventWork->start_transition = 0x400;
        gEventWork->transition_frames = 16;
        Engine_TaskWait(1);
        Engine_CameraSetSpeed(0x80000, 0x10000);
        Engine_GameFlagClear(0x12f);
        Engine_TaskAddCallback(MapActor_UpdateContact, 0xc80);
        if (Engine_GameFlagIsSet(0x90a) == 0) {
            FieldMap_SetWindowCellTile(128, 256, 176, 56);
        }
        switch (*entrance) {
        case 1:
            if (GameFlag_IsSet(0x815) == 0) {
                Engine_GameFlagSet(0x815);
                Engine_GameFlagSet(0x85c);
            }
            break;
        case 33:
            if (Engine_GameFlagIsSet(0x109) != 0) {
                if (Engine_GameFlagIsSet(0x85d) == 0 && Engine_GameFlagIsSet(0x234) != 0) {
                    gWorldMapTriggerActor = 55;
                    Actor_SetPosition(55, 0x17940000, 0xd480000);
                    Object_GetById(gWorldMapTriggerActor)->facing = 0x3000;
                    WorldMap_ActivateSite138(gWorldMapTriggerActor);
                }
            } else if (Engine_GameFlagIsSet(0x85d) == 0 && Engine_GameFlagIsSet(0x9b8) == 0) {
                WorldMap_RunBlackOrbScene();
            }
            break;
        case 49:
            if (Engine_GameFlagIsSet(0x94f) == 0 && Engine_GameFlagIsSet(0x941) != 0) {
                RunEventScript01();
            }
            break;
        case 64:
            if (Engine_GameFlagIsSet(0x85a) == 0) {
                FieldScene_RunActorTransferSequence();
            }
            break;
        case 65:
            WorldMap_TransferToExit102();
            break;
        case 66:
            WorldMap_TransferToExit103();
            break;
        case 67:
            WorldMap_TransferToExit104();
            break;
        case 68:
            WorldMap_TransferToFuneHeya();
            break;
        case 69:
            WorldMap_TransferToExit106();
            break;
        case 70:
            WorldMap_CompleteTransfer();
            break;
        case 71:
            WorldMap_DisembarkCarriage();
            break;
        case 72:
            WorldMap_RunOpeningRise();
            break;
        case 73:
            StoryScene_StartTransition();
            break;
        case 74:
        case 76:
        case 77:
            Engine_GameFlagSet(0x11c);
            if (GameFlag_GetByte(0x2f8) != 0) {
                gGameState.movement_mode = 2;
                Engine_TaskAddCallback(StoryScene_UpdateSelectedActorProgress, 0xc80);
            }
            break;
        case 75:
            FieldScene_RunActorEightApproach();
            break;
        case 80:
            FieldScene_RunActorPresentationSequence();
            break;
        default:
            Object_GetById(53)->scale_x = 0x14000;
            Object_GetById(53)->scale_y = 0x14000;
            break;
        }
    }
    return 0;
}

void RunEventScript01(void)
{
    u32 i;
    struct FieldActor *record;

    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Battle_SetObjectFlag5bWhenMode3();
    GameFlag_Set(0x94f);
    Actor_SetPosition(11, 0x16e00000, 0x49c0000);
    Actor_SetDestinationOffset(11, 24, 8);
    Engine_ActorWaitForMove(11);
    Engine_EventWait(60);
    Actor_SetPosition(12, 0x16e00000, 0x49c0000);
    Actor_SetDestinationOffset(12, 12, 24);
    Engine_EventWait(30);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_FaceDirection(12, 0xd000, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(12, 3);
    Engine_EventWait(120);
    Actor_SetPosition(8, 0x16f80000, 0x4b80000);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(12, 2);
    record = Object_GetById(8);
    if (record != 0) {
        Actor_SetDestination(12, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(12);
    Actor_SetPosition(12, 0, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(11, 2);
    record = Object_GetById(8);
    if (record != 0) {
        Actor_SetDestination(11, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(11);
    Actor_SetPosition(11, 0, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    record = Object_GetById(8);
    if (record != 0) {
        Actor_SetDestination(ACTOR_PARTY_LEADER, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Engine_EventWait(60);
    Actor_SetSpeed(8, 0x8000, 0x4000);
    Actor_SetDestinationOffset(8, 56, 8);
    Engine_ActorWaitForMove(8);
    Actor_SetDestinationOffset(8, 40, 40);
    Engine_ActorWaitForMove(8);
    Actor_SetDestinationOffset(8, 8, 88);
    Engine_ActorWaitForMove(8);
    Engine_EventCloseScreen();
    Engine_EventRequestExit(108);
    Engine_EventEnd();
}

void FieldScene_RunActorTransferSequence(void)
{
    struct FieldActor *actor;
    struct FieldActor *record;
    const u8 *action;

    actor = Object_GetById(15);
    Engine_EventBegin();
    BattleFx_ScheduleRatioTransition(0x14000, 1);
    Engine_TaskWait(4);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Battle_SetObjectFlag5bWhenMode3();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    Actor_SetSpeed(GERALD, 0x19999, 0xcccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x16fc, 0x628);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_SetPosition(8, 0x16d80000, 0x6280000);
    Engine_TaskWait(1);
    Actor_SetChildValue(8, 15);
    record = Actor_Get(8);
    Engine_ActorSetSpriteFlags(record, 0);
    Actor_SetSpeed(10, 0x19999, 0x6666);
    Actor_SetSpeed(11, 0x19999, 0x6666);
    Actor_SetSpeed(12, 0x19999, 0x6666);
    Actor_SetSpeed(13, 0x19999, 0x6666);
    Audio_PlayCue(141);
    Engine_ActorEnableActionCallback(10, gTransferArrive10);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(11, gTransferArrive11);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(12, gTransferArrive12);
    Engine_EventWait(20);
    Object_SetActionCallbackAndRefreshById(13, gTransferArrive13);
    Audio_PlayCue(0x121);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetPosition(GERALD, record->x.fixed, record->z.fixed);
    }
    Actor_WalkToAndWait(GERALD, 0x1704, 0x640);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(GERALD, 0xa000, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(GERALD, 0x101, 60);
    Actor_SetPosition(8, 0x16d80000, 0x6380000);
    Engine_TaskWait(1);
    Engine_EventSetMessage((s32)MsgWorldMapWeCantStayAnotherMinute);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(GERALD, 0x6000, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(GERALD, 0xa000, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(GERALD, 0x6000, 10);
    Audio_PlayCue(141);
    Engine_ActorEnableActionCallback(10, gTransferDepart10);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(11, gTransferDepart11);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(12, gTransferDepart12);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(GERALD, 0x4000, 10);
    Object_SetActionCallbackAndRefreshById(13, gTransferDepart13);
    Audio_PlayCue(0x121);
    Engine_EventWait(20);
    Battle_ClearObjectFlag5bWhenMode3();
    Engine_ActorSetAnimation(10, 1);
    Engine_ActorSetAnimation(11, 1);
    Engine_ActorSetAnimation(12, 1);
    Engine_ActorSetAnimation(13, 1);
    Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Battle_SetObjectFlag5bWhenMode3();
    Actor_SetPosition(9, 0x16080000, 0x6d80000);
    Engine_TaskWait(1);
    Actor_SetSpeed(9, 0x13333, 0x9999);
    Actor_WalkToAndWait(9, 0x1608, 0x6c8);
    Actor_WalkToAndWait(9, 0x15f8, 0x6c8);
    Actor_WalkToAndWait(9, 0x15f8, 0x6f8);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(9, 0x102);
    Engine_EventWait(60);
    Actor_FaceDirection(9, 0, 20);
    Engine_ActorStartRepeatedMotion(9, 3);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetPosition(8, 0x16180000, 0x6f80000);
    Engine_TaskWait(1);
    Actor_SetChildValue(8, 0);
    record = Actor_Get(8);
    Engine_ActorSetSpriteFlags(record, 1);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_WalkToAndWait(8, 0x1608, 0x6f8);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_FaceDirection(8, 0x3000, 60);
    Actor_FaceDirection(8, 0x8000, 10);
    Actor_SetAttachedEffect(8, 0x102);
    Engine_EventWait(60);
    Actor_FaceDirection(9, 0x3000, 0);
    Actor_FaceDirection(8, 0x3000, 40);
    Actor_SetAttachedEffect(8, 0x102);
    Engine_EventWait(60);
    Engine_ActorStartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(0x2008, 0, 40);
    Engine_ActorRunRepeatedMotion(9, 1);
    Actor_FaceDirection(9, 0, 10);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_ShowEmote(8, 0x105, 60);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_ShowEmote(9, 0x101, 60);
    Event_ShowMessageAndWait(9, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Battle_ClearObjectFlag5bWhenMode3();
    Audio_PlayCue(107);
    Camera_SetSpeed(0x40000, 0x40000);
    WorldMap_ShakeTransferStartCamera();
    Audio_PlayCue(0x121);
    Actor_ShowEmote(8, 0x100, 0);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_FaceDirection(9, 0, 40);
    Actor_FaceDirection(8, 0xb000, 0);
    Actor_FaceDirection(9, 0xb000, 0);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x15e80000, -1, 0x6c80000, 1);
    Engine_CameraWaitForMove();
    Actor_SetPosition(14, 0x15a80000, 0x6a80000);
    Engine_TaskWait(1);
    Actor_SetSpeed(14, 0x4ccc, 0x2666);
    Engine_ActorEnableActionCallback(14, gTransferGuide14);
    Engine_EventWait(160);
    actor->gravity = 0x1999;
    actor->bounce = 0x1999;
    actor->scale_x = 0x18000;
    actor->scale_y = 0x18000;
    actor->timer = 0;
    actor->y.fixed = 0x400000;
    actor->sprite->rotation = 0xf000;
    Engine_ActorSetSpriteFlags(actor, 0);
    Object_SetMode(actor, 2);
    Engine_TaskWait(1);
    record = Actor_Get(15);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_TaskAddCallback(WorldMap_UpdateTransferredActorLanding, 0xc80);
    do {
        Engine_TaskWait(1);
    } while ((s16)actor->timer == 0);
    record = Actor_Get(15);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Actor_Get(14);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_EventWait(10);
    record = Actor_Get(9);
    record->velocity_y = 0x60000;
    record = Object_GetById(8);
    record->velocity_y = 0x60000;
    Audio_PlayCue(145);
    Camera_SetSpeed(0x40000, 0x40000);
    WorldMap_ShakeTransferLandingCamera();
    WorldMap_ShakeTransferLandingCamera();
    Engine_EventWait(60);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
    Engine_CameraWaitForMove();
    Battle_SetObjectFlag5bWhenMode3();
    Actor_SetAttachedEffect(9, 0x102);
    Actor_SetAttachedEffect(8, 0x102);
    Engine_EventWait(60);
    Engine_TaskRemoveCallback(WorldMap_UpdateTransferredActorLanding);
    Engine_TaskWait(1);
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(15, 0, 0);
    Actor_FaceDirection(8, 0x8000, 10);
    Engine_ActorJump(8, 4, 40);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_FaceDirection(9, 0, 10);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(8, 0xc000, 40);
    Event_ShowMessageAndWait(0x2008, 0, 20);
    Engine_ActorJump(9, 4, 20);
    Event_ShowMessageAndWait(9, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x8000, 10);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_SetAttachedEffect(9, 0x102);
    Engine_EventWait(80);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_ActorSetAnimationAndWait(9, 3);
    Actor_WalkToAndWait(8, 0x1618, 0x6f8);
    Actor_SetPosition(8, 0, 0);
    Actor_WalkToAndWait(9, 0x15f8, 0x6c8);
    Actor_WalkToAndWait(9, 0x1608, 0x6c8);
    Actor_WalkToAndWait(9, 0x1608, 0x6d8);
    Actor_SetPosition(9, 0, 0);
    Audio_PlayCue(141);
    Engine_ActorEnableActionCallback(10, gTransferReturn10);
    Engine_ActorEnableActionCallback(11, gTransferReturn11);
    Engine_EventWait(40);
    Engine_ActorEnableActionCallback(12, gTransferReturn12);
    Engine_EventWait(40);
    Object_SetActionCallbackAndRefreshById(13, gTransferReturn13);
    Battle_ClearObjectFlag5bWhenMode3();
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x170c0000, 0x6280000);
    Actor_SetPosition(GERALD, 0x17140000, 0x6400000);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0x16d80000, -1, 0x6480000, 1);
    Engine_CameraWaitForMove();
    action = gTransferGather;
    Engine_ActorEnableActionCallback(10, action);
    Engine_EventWait(20);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x16d80000, -1, 0x6080000, 1);
    Engine_ActorEnableActionCallback(11, action);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(12, action);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(GERALD, 0x8000, 0);
    Engine_ActorEnableActionCallback(13, action);
    Engine_EventWait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(GERALD, 0xc000, 0);
    Object_RefreshSelectorById(13);
    Audio_PlayCue(0x121);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0x16f80000, -1, 0x6480000, 1);
    Engine_CameraWaitForMove();
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(GERALD, 0xa000, 80);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(GERALD, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(GERALD, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(GERALD);
    Actor_SetPosition(GERALD, 0, 0);
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x16d80000, -1, 0x6480000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x16d8, 0x628);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    GameFlag_Set(0x85a);
    Engine_EventRequestExit(3);
    Engine_EventEnd();
}

void WorldMap_ShakeTransferStartCamera(void)
{
    Camera_MoveTo(0x160c0000, -1, 0x6f80000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x16040000, -1, 0x6fc0000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x160c0000, -1, 0x6f40000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x160c0000, -1, 0x6fc0000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x16040000, -1, 0x6f40000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x160a0000, -1, 0x6f80000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x16060000, -1, 0x6fa0000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x160a0000, -1, 0x6f60000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x160a0000, -1, 0x6fa0000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x16060000, -1, 0x6f60000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
    Engine_TaskWait(4);
}

void WorldMap_ShakeTransferLandingCamera(void)
{
    Camera_MoveTo(0x15ec0000, -1, 0x6c80000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x15e40000, -1, 0x6cc0000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x15ec0000, -1, 0x6c40000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x15ec0000, -1, 0x6cc0000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x15e40000, -1, 0x6c40000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x15e80000, -1, 0x6c80000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x15ea0000, -1, 0x6c80000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x15e60000, -1, 0x6ca0000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x15ea0000, -1, 0x6c60000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x15ea0000, -1, 0x6ca0000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x15e60000, -1, 0x6c60000, 1);
    Engine_TaskWait(4);
    Camera_MoveTo(0x15e80000, -1, 0x6c80000, 1);
    Engine_TaskWait(4);
}

void WorldMap_UpdateTransferredActorLanding(void)
{
    struct FieldActor *actor;
    struct FieldActor *guide;

    actor = Object_GetById(15);
    guide = Object_GetById(14);
    actor->x.fixed = guide->x.fixed;
    actor->z.fixed = guide->z.fixed;
    if (actor->y.fixed < 0xa0000) {
        actor->y.fixed = 0xa0000;
        if (GameFlag_IsSet(0x200) == 0) {
            Audio_PlayCue(145);
            Object_SetMode(actor, 3);
            GameFlag_Set(0x200);
            actor->timer = 1;
        }
    }
}

void WorldMap_TransferToExit102(void)
{
    struct FieldActor *actor;

    actor = Object_GetById(8);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    actor->scale_y = 0x14000;
    actor->scale_x = 0x14000;
    Engine_CameraFollowActor(8, 1);
    Engine_EventOpenScreen();
    Actor_SetSpeed(8, 0x6666, 0x3333);
    Actor_WalkToAndWait(8, 0x14a8, 0x918);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    GameFlag_Set(0x927);
    Engine_EventRequestExit(102);
    Engine_EventEnd();
}

void WorldMap_TransferToExit103(void)
{
    struct FieldActor *actor;

    actor = Object_GetById(8);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x1f080000, 0xc80000);
    actor->scale_x = 0x14000;
    actor->scale_y = 0x14000;
    Engine_TaskWait(1);
    Engine_CameraFollowActor(8, 1);
    Engine_EventOpenScreen();
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    actor->timer = 0;
    Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
    do {
        Engine_TaskWait(1);
    } while ((s16)actor->timer == 0);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    GameFlag_Set(0x927);
    Engine_EventRequestExit(103);
    Engine_EventEnd();
}

void WorldMap_TransferToExit104(void)
{
    struct FieldActor *actor;

    actor = Object_GetById(8);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x1f080000, 0xc80000);
    actor->scale_x = 0x14000;
    actor->scale_y = 0x14000;
    Engine_TaskWait(1);
    Engine_CameraFollowActor(8, 1);
    Engine_EventOpenScreen();
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    actor->timer = 0;
    Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
    do {
        Engine_TaskWait(1);
    } while ((s16)actor->timer == 0);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    GameFlag_Set(0x927);
    Engine_EventRequestExit(104);
    Engine_EventEnd();
}

void WorldMap_TransferToFuneHeya(void)
{
    struct FieldActor *actor;

    actor = Object_GetById(8);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x1f080000, 0xc80000);
    actor->scale_x = 0x14000;
    actor->scale_y = 0x14000;
    Engine_TaskWait(1);
    Engine_CameraFollowActor(8, 1);
    Engine_EventOpenScreen();
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    actor->timer = 0;
    Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
    do {
        Engine_TaskWait(1);
    } while ((s16)actor->timer == 0);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    GameFlag_Set(0x927);
    Engine_EventRequestExit(105);
    Engine_EventEnd();
}

void WorldMap_TransferToExit106(void)
{
    struct FieldActor *actor;

    actor = Object_GetById(8);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x1f080000, 0xc80000);
    actor->scale_x = 0x14000;
    actor->scale_y = 0x14000;
    Engine_TaskWait(1);
    Engine_CameraFollowActor(8, 1);
    Engine_EventOpenScreen();
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    actor->timer = 0;
    if (StoryScene_ComputeOpposingSlotDelta() == 11) {
        Engine_ActorEnableActionCallback(8, gTransferLeaderTurn);
    } else {
        Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
    }
    do {
        Engine_TaskWait(1);
    } while ((s16)actor->timer == 0);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    GameFlag_Set(0x927);
    Engine_EventRequestExit(106);
    Engine_EventEnd();
}

void WorldMap_CompleteTransfer(void)
{
    struct FieldActor *actor;

    actor = Object_GetById(8);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x13e80000, 0x9180000);
    actor->scale_y = 0x14000;
    actor->scale_x = 0x14000;
    Engine_TaskWait(1);
    Engine_CameraFollowActor(8, 1);
    Engine_EventOpenScreen();
    Actor_SetSpeed(8, 0x6666, 0x3333);
    Actor_WalkToAndWait(8, 0x13c8, 0x918);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    GameFlag_Set(0x93e);
    GameFlag_Clear(0x927);
    Engine_EventRequestExit(107);
    Engine_EventEnd();
}

void WorldMap_ShowDjinnTutorial(void)
{
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_ScheduleRatioTransition(0x10000, 6);
    Event_WaitForZoomTransition();
    Battle_SetObjectFlag5bWhenMode3();
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventSetMessage((s32)MsgWorldMapAfterBringingDjinniIntoYour);
    Event_ShowMessage(8, 0);
    Engine_EventWait(30);
    Audio_PlayCue(111);
    Menu_AnimateSelectionToEntry(0, 2);
    GameFlag_Clear(0x16f);
    GameFlag_Clear(0x171);
    ItemMenu_Open();
    Engine_ActorJump(8, 4, 30);
    Engine_EventSetMessage((s32)MsgWorldMapNextIllShowHowCan);
    Event_ShowMessage(8, 0);
    GameFlag_Clear(0x16f);
    GameFlag_Set(0x171);
    ItemMenu_Open();
    Engine_EventWait(30);
    BattleFx_SetWeightedResult(12, 6);
}

void Owner_RefreshActiveRatios(s32 owner);
s32 Djinn_AddToOwner();
s32 Trade_AddOffer();
void BattleEffect_CleanupSceneObjects(void);
void UiWork_PushValueSlot(s32 value, s32 slot);
void BattleFx_RunPageEffectForSlot(s32 actor, s32 a1, s32 a2);

#define DJINNI 8
#define FLAG_DJINNI_MET 0x16e

void WorldMap_MeetVenusDjinni(void)
{
    struct FieldActor *djinni;
    struct FieldActor *leader;
    s32 x;
    s32 z;
    s32 count;
    s32 i;

    count = 0;
    djinni = Object_GetById(DJINNI);
    leader = Object_GetById(0);
    x = (leader->x.fixed - 0x15d00000) / 2 + 0x15d00000;
    z = (leader->z.fixed - 0x5300000) / 2 + 0x5300000;
    if (!GameFlag_IsSet(FLAG_DJINNI_MET)) {
        Owner_RefreshActiveRatios(1);
        GameFlag_Set(FLAG_DJINNI_MET);
        Event_Begin();
        leader = Actor_Get(0);
        if (leader != 0)
            Actor_SetPosition(DJINNI, leader->x.fixed, leader->z.fixed);
        Djinn_AddToOwner(0, 0, 0);
        Trade_AddOffer(0, 0, 0);
        Battle_SetObjectFlag5bWhenMode3();
        Actor_FaceActor(0, DJINNI, 0);
        Event_Wait(10);
        Actor_ShowEmote(0, 0x101, 60);
        djinni->work = 1;
        Actor_FaceActor(DJINNI, 0, 0);
        Task_Wait(16);
        Event_SetMessage((s32)MsgWorldMapOh);
        Event_ShowMessage(DJINNI, 0);
        Battle_ClearObjectFlag5bWhenMode3();
        BattleFx_ScheduleRatioTransition(0x13333, 6);
        Event_WaitForZoomTransition();
        Battle_SetObjectFlag5bWhenMode3();
        djinni->motion_flags = 2;
        djinni->gravity = 0x4000;
        djinni->speed = 0x10000;
        djinni->acceleration = 0x10000;
        djinni->velocity_y = 0;
        djinni->terrain_height = 0;
        FieldObject_SetPosition(djinni, 0x15d00000, 0, 0x5300000);
        for (i = 0; i < 16; i++) {
            djinni->scale_x += 0x800;
            djinni->scale_y += 0x800;
            Task_Wait(1);
        }
        Actor_FaceActor(DJINNI, 0, 0);
        Actor_FaceActor(0, DJINNI, 0);
        Task_Wait(16);
        djinni->update = 0;
        Engine_ObjectSetPartPalettes(djinni, 0);
        djinni->gravity = 0x10000;
        Event_ShowMessage(DJINNI, 0);
        Audio_PlayCue(131);
        Engine_PsynergyBegin(140, 0);
        for (i = 59; i >= 0; i--) {
            if (gFrameCount & 2)
                Engine_ObjectSetPartPalettes(djinni, 7);
            else
                Engine_ObjectSetPartPalettes(djinni, 0);
            if ((gFrameCount & 15) == 0)
                WorldMap_CreateLinkedEffects(djinni);
            Task_Wait(1);
        }
        BattleEffect_CleanupSceneObjects();
        Engine_ObjectSetPartPalettes(djinni, 0);
        Actor_RunRepeatedMotion(DJINNI, 2);
        Event_ShowMessage(DJINNI, 0);
        Actor_ShowEmote(0, 0x102, 30);
        Event_ShowMessage(DJINNI, 0);
        Actor_ShowEmote(0, 0x101, 30);
        Actor_WalkToAndWait(DJINNI, x >> 16, z >> 16);
        Actor_SetAnimation(0, 22);
        Event_ShowMessage(DJINNI, 0);
        Actor_ShowEmote(0, 0x101, 40);
        Actor_Jump(DJINNI, 4, 30);
        UiWork_PushValueSlot(300, 4);
        Event_ShowMessage(DJINNI, 0);
        Actor_ShowEmote(0, 0x100, 30);
        Event_ShowMessage(DJINNI, 0);
        Actor_RunRepeatedMotion(0, 2);
        Event_ShowMessage(DJINNI, 0);
        Actor_Jump(DJINNI, 2, 30);
        Event_ShowMessage(DJINNI, 0);
        i = 0;
        djinni->motion_flags = 0;
        FieldObject_SetPosition(djinni, x, 0x100000, z);
        for (; i < 16; i++) {
            djinni->facing += 0x1000;
            Task_Wait(1);
        }
        Actor_SetAnimation(0, 1);
        Event_ShowMessage(DJINNI, 0);
        djinni->motion_flags = 2;
        djinni->velocity_y = 0;
        djinni->terrain_height = 0;
        for (i = 7; i >= 0; i--) {
            djinni->facing += 0x1000;
            Task_Wait(1);
        }
        Actor_SetAnimation(0, 22);
        Event_ShowMessage(DJINNI, 0);
        Actor_ShowEmote(DJINNI, 0x102, 30);
        Actor_FaceActor(DJINNI, 0, 0);
        Actor_RunRepeatedMotion(DJINNI, 2);
        Event_ShowMessage(DJINNI, 0);
        Actor_Jump(DJINNI, 2, 30);
        Event_OpenMessage(DJINNI, 0);
        count = 0;
    plead:
        if (Event_ChooseYesNo(0, 0) == 1) {
            Actor_Jump(DJINNI, 2, 20);
            Actor_Jump(DJINNI, 2, 20);
            if (count == 6) {
                Event_SetMessage((s32)MsgWorldMapMeanieDontCare);
                Event_ShowMessage(DJINNI, 0);
                goto listened;
            }
            Event_SetMessage(count + (s32)MsgWorldMapComePromiseWont);
            Event_OpenMessage(DJINNI, 0);
            count++;
            goto plead;
        }
        Actor_SetAnimation(0, 22);
        Actor_Jump(DJINNI, 2, 20);
        Actor_Jump(DJINNI, 4, 20);
        Event_SetMessage((s32)MsgWorldMapSeeWontRegret);
        Event_ShowMessage(DJINNI, 0);
listened:
        UiWork_PushValueSlot(300, 4);
        Audio_PlayCue(81);
        count = (s32)MsgWorldMapAbilityVenusDjinni;
        Message_ShowCentered(count++, 3);
        Event_SetMessage(count);
        {
            s32 frames = 20;
            s32 flags;

            asm volatile("" : : "r"(frames)); /* FAKEMATCH: sets the jump's frames first */
            Actor_Jump(DJINNI, 2, frames);
            flags = 0;
            asm volatile("" : : "r"(flags)); /* FAKEMATCH: sets the message flags first */
            Event_ShowMessage(DJINNI, flags);
        }
        Audio_PlayCue(9);
        goto finish;
    }
    Event_Begin();
    leader = Object_GetById(0);
    if (leader != 0)
        Actor_SetPosition(DJINNI, leader->x.fixed, leader->z.fixed);
    djinni->velocity_y = 0xa0000;
    FieldObject_SetPosition(djinni, x, 0, z);
    Event_Wait(30);
    Battle_SetObjectFlag5bWhenMode3();
    Actor_FaceActor(DJINNI, 0, 0);
    Actor_FaceActor(0, DJINNI, 0);
    Actor_SetAnimation(0, 22);
    Event_SetMessage((s32)MsgWorldMapSeeDjinnUseful);
    Actor_Jump(DJINNI, 2, 20);
    Actor_Jump(DJINNI, 2, 20);
    Event_ShowMessage(DJINNI, 0);
    Actor_RunRepeatedMotion(DJINNI, 2);
    Event_ShowMessage(DJINNI, 0);
    Audio_PlayCue(111);
    Menu_AnimateSelectionToEntry(0, 2);
    GameFlag_Set(0x16f);
    GameFlag_Clear(0x171);
    ItemMenu_Open();
    Event_SetMessage((s32)MsgWorldMapGoSetStandby);
    FieldObject_SetPosition(djinni, 0x15d00000, 0, 0x5300000);
    Event_Wait(30);
    Event_ShowMessage(DJINNI, 0);
    Actor_FaceActor(DJINNI, 0, 0);
    Event_ShowMessage(DJINNI, 0);
    Event_OpenMessage(DJINNI, 0);
    if (Event_ChooseYesNo(0, 0) != 1)
        goto learn;
    Actor_SetAnimation(0, 22);
    Actor_RunRepeatedMotion(DJINNI, 2);
    Event_SetMessage((s32)MsgWorldMapHmmmmExplainAgain);
    Event_OpenMessage(DJINNI, 0);
    if (Event_ChooseYesNo(0, 0) == 1)
        goto learn;
    Event_ShowMessage(DJINNI, 0);
    {
        s32 tile_x = x >> 16;
        s32 tile_z = z >> 16;

        asm("" : "=r"(tile_z) : "0"(tile_z) : "r3"); /* FAKEMATCH: clobbers r3, a dependent of the z copy and not of the x copy */
        asm("" : "=r"(tile_z) : "0"(tile_z) : "r3"); /* FAKEMATCH: a second one outnumbers the x copy's, so sched2 copies z first */
        Actor_WalkToAndWait(DJINNI, tile_x, tile_z);
    }
finish:
    WorldMap_ShowDjinnTutorial();
    Battle_ClearObjectFlag5bWhenMode3();
    return;
learn:
    Actor_SetAnimation(0, 22);
    Event_SetMessage((s32)MsgWorldMapYeahWantLearn);
    Actor_Jump(DJINNI, 2, 20);
    Actor_Jump(DJINNI, 2, 20);
    Actor_SetAnimationAndWait(0, 3);
    Actor_ShowEmote(DJINNI, 0x100, 30);
    Event_ShowMessage(DJINNI, 0);
    GameFlag_Set(0x16f);
    GameFlag_Set(0x171);
    ItemMenu_Open();
    Actor_Jump(DJINNI, 2, 20);
    Event_ShowMessage(DJINNI, 0);
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_RunPageEffectForSlot(DJINNI, 0, 0);
    Audio_PlayCue(42);
    Event_End();
    GameFlag_Clear(FLAG_DJINNI_MET);
    GameFlag_Clear(0x16f);
    GameFlag_Clear(0x171);
}
