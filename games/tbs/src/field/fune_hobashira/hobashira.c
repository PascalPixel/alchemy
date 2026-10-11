#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "fune.h"
#include "types.h"
#include "field_event.h"
#include "field_service.h"
#include "text/msg_ids.h"

struct SwayActor {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    u8 unknown_10[0x3c];
    s32 drift;
    u8 unknown_50[0x14];
    s16 x_state;
    s16 y_state;
};

void battle_owner_69(void);
void FieldScene_RunPrimarySequence(void);

struct FieldGlobals {
    s32 **map;
    u8 unknown_04[0x48];
    struct EventWork *event;
};



s32 FuneHobashira_DriftY __attribute__((section(".bss")));
s32 FuneHobashira_DriftX __attribute__((section(".bss")));
s32 FuneHobashira_SwayY __attribute__((section(".bss")));
s32 FuneHobashira_SwayUnused __attribute__((section(".bss")));
s32 FuneHobashira_CameraCenter[2] __attribute__((section(".bss")));
s32 FuneHobashira_CameraOrigin[2] __attribute__((section(".bss")));
s32 FuneHobashira_SwayX __attribute__((section(".bss")));

s32 Object_PlaceFromCameraOffset(struct FieldActor *object)
{
    s32 *position = *((s32 **)gMapWork[0]);
    s32 *origin = FuneHobashira_CameraOrigin;
    s32 *center = FuneHobashira_CameraCenter;
    s32 q0 = *position++;
    s32 q1 = *position;

    object->x.fixed = center[0] + (q0 - origin[0]);
    object->y.fixed = center[1] + (q1 - origin[1]) / 2;
    object->sprite->rotation += 0x600;
    return 0;
}

s32 SceneActor_SetFacingFromSample(struct FieldActor *object)
{
    u32 sample = (u32)(((u32 (*)(void))Engine_RandomNext)() << 6) >> 16;

    if (sample == 6) {
        s32 value = 0xd000;

        object->facing = value;
    } else if (sample == 9) {
        s32 value = 0xb000;

        object->facing = value;
    }

    return 1;
}

void FuneHobashira_GrowActorScale(struct FieldActor *o)
{
    if (o->scale_x < 0x10000) {
        o->scale_x += 160;
        o->scale_y += 160;
    }
}

s32 FuneHobashira_SwayActor(struct SwayActor *actor)
{
    s16 *state = &actor->x_state;
    s32 next;

    if (*state == 9) {
        actor->drift = 0;
    } else if (*state != 0) {
        actor->drift -= (u32)(((u32 (*)(void))Engine_RandomNext)() << 11) >> 16;
        if (actor->drift < -0xc000) {
            next = 0;
            *state = next;
        }
    } else {
        actor->drift += (u32)(((u32 (*)(void))Engine_RandomNext)() << 11) >> 16;
        if (actor->drift > 0xc000) {
            next = 1;
            *state = next;
        }
    }
    if (actor->x > 0x280000 && actor->x < 0x1400000) {
        actor->x += actor->drift;
    }
    state = &actor->y_state;
    if (*state == 9) {
        actor->y = 0;
    } else if (*state != 0) {
        actor->y -= (u32)(((u32 (*)(void))Engine_RandomNext)() * 3 << 14) >> 16;
        if (actor->y < 0) {
            next = 0;
            *state = next;
        }
    } else {
        actor->y += (u32)(((u32 (*)(void))Engine_RandomNext)() * 3 << 14) >> 16;
        if (actor->y > 0x100000) {
            next = 1;
            *state = next;
        }
    }
    return 1;
}

s32 OverlayObject_ShrinkScaleThenStop(struct FieldActor *o)
{
    u8 *t = (u8 *)o->sprite;

    t[9] |= 12;
    o->speed = 0x20000;
    o->acceleration = 0x10000;
    if (o->scale_x > 0x1000) {
        o->scale_x -= 0x400;
        o->scale_y -= 0x400;
    } else {
        o->x.fixed = 0;
        o->y.fixed = 0;
        o->z.fixed = 0;
        o->velocity_x = 0;
        o->velocity_y = 0;
        o->velocity_z = 0;
    }
    return 1;
}

u8 *FuneHobashira_GetEntrances(void)
{
    return FuneHobashira_SceneTableA;
}

u8 *FuneHobashira_GetRegions(void)
{
    return FuneHobashira_SceneTableB;
}

u8 *FuneHobashira_GetExits(void)
{
    return FuneHobashira_SceneTableC;
}

u8 *FuneHobashira_GetPlacements(void)
{
    return FuneHobashira_SceneTableD;
}

u8 *FuneHobashira_GetEvents(void)
{
    return FuneHobashira_SceneTableE;
}

void FuneHobashira_UpdateGridPlacement(void)
{
    FieldEffect_UpdateGridPlacement();
}

void FieldScene_RunActor232SceneWhenFlag923Or922(void)
{
    if (Engine_GameFlagIsSet(FLAG_MAST_923) != 0 || Engine_GameFlagIsSet(FLAG_MAST_922) != 0) {
        Engine_EventBegin();
        Engine_ItemShowFound(ITEM_ANCHOR_CHARM, 3);
        Engine_PartyGiveItem(ITEM_ANCHOR_CHARM, 0);
        Engine_GameFlagSet(FLAG_MAST_924);
        Engine_EventEnd();
    }
}

s32 FuneHobashira_ApplyEntryState(void)
{
    struct FieldActor *leader;

    Engine_GameFlagSet(0x144);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if ((Engine_GameFlagIsSet(0x927) != 0 || Engine_GameFlagIsSet(0x928) != 0)
        && Engine_GameFlagIsSet(0x93e) == 0 && Engine_GameFlagIsSet(0x8a0) == 0) {
        FuneHobashira_SwayX = (u16)((u32 (*)(void))Engine_RandomNext)();
        FuneHobashira_SwayY = (u16)((u32 (*)(void))Engine_RandomNext)();
        Engine_TaskAddCallback(FuneHobashira_UpdateSway, 0xc80);
    }
    if (Engine_GameFlagIsSet(0x925) != 0 && Engine_GameFlagIsSet(0x93e) == 0) {
        Actor_SetPosition(8, 0xa40000, 0x1480000);
    }
    switch (gGameState.entrance) {
    case 1:
        if (Engine_GameFlagIsSet(0x109) == 0) {
            leader = Object_GetById(ACTOR_PARTY_LEADER);
            Engine_EventBegin();
            battle_owner_69();
            leader->y.fixed = 0x380000;
            Camera_MoveTo(-1, -1, -1, 0);
            Engine_TaskWait(1);
            Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
            Engine_MapRedraw();
            Engine_TaskWait(1);
            Engine_EventEnd();
        }
        break;
    case 10:
        if (Engine_GameFlagIsSet(0x928) != 0) {
            FuneHobashira_SetupVisibleLeaderLookout();
        } else {
            FuneHobashira_SetupHiddenLeaderLookout();
        }
        break;
    case 11:
        gGameState.saved_scene = (s32)&SceneId_FuneHeya;
        gGameState.saved_entrance = 30;
        Scene_RunFourActorStagingSequence();
        break;
    case 12:
        gGameState.saved_scene = (s32)&SceneId_FuneHeya;
        gGameState.saved_entrance = 30;
        FieldScene_RunActorNinePresentationCycles();
        break;
    case 13:
        gGameState.saved_scene = (s32)&SceneId_FuneHeya;
        gGameState.saved_entrance = 30;
        FieldScene_RunPrimarySequence();
        break;
    case 14:
        FieldScene_RunSevenActorEnsemble();
        break;
    }
    return 0;
}

void FuneHobashira_SetupHiddenLeaderLookout(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    ((struct FieldActor *)Battle_GetWorkObject1e0())->motion_flags = 0;
    Camera_MoveTo(0xa40000, 0x400000, 0x1410000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    FuneHobashira_AnnounceClearCourse();
    Engine_EventEnd();
}

void FuneHobashira_SetupVisibleLeaderLookout(void)
{
    u32 i;
    struct FieldActor *record;

    Engine_EventBegin();
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xa40000, 0x1410000);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    FuneHobashira_AnnounceClearCourse();
    Engine_EventEnd();
}

void FuneHobashira_AnnounceClearCourse(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_WalkToAndWait(8, 164, 0x141);
    Actor_FaceDirection(8, 0xd000, 40);
    Actor_FaceDirection(8, 0xb000, 40);
    Actor_FaceDirection(8, 0xd000, 40);
    Actor_FaceDirection(8, 0x3000, 10);
    Actor_WalkToAndWait(8, 164, 0x14e);
    Engine_ActorJump(8, 4, 40);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventSetMessage((s32)MsgFuneShipsCourseClear);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(10);
}

void Scene_RunFourActorStagingSequence(void)
{
    s32 *placement = *(*(struct FieldGlobals *)gMapWork).map;
    struct FieldActor *actor;
    struct EventWork **event;
    s32 x;
    s32 zero = 0;
    s32 scale;
    s32 update;

    Engine_EventBegin();
    Event_CallWithLastActiveObjectId((s32)FuneHobashira_StagingObjects);
    Engine_TaskWait(1);
    Engine_ActorSetChildValue(0, 15);
    Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
    Engine_ActorEnableActionCallback(8, FuneHobashira_LookoutActions);
    event = &(*(struct FieldGlobals *)gMapWork).event;
    (*event)->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    FuneHobashira_CameraOrigin[0] = *placement++;
    FuneHobashira_CameraOrigin[1] = *placement;
    Actor_SetPosition(9, 0x500000, 0xd20000);
    x = 0x500000;
    Object_GetById(9)->motion_flags = zero;
    FuneHobashira_CameraCenter[0] = x;
    FuneHobashira_CameraCenter[1] = zero;
    Engine_ActorEnableActionCallback(9, FuneHobashira_FollowCameraActions);
    Engine_EventWait(20);
    Engine_AudioPlayCue(29);
    Engine_GameFlagSet(0x8f0);
    Engine_ActorStop(8);
    Engine_TaskWait(1);
    Actor_ShowEmote(8, 0x100, 0);
    Engine_ActorFaceDirection(8, 0xb000, 0);
    Engine_EventSetMessage((s32)MsgFuneAvast);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Actor_SetPosition(10, x, 0xd20000);
    Actor_SetPosition(11, x, 0xd20000);
    Engine_ActorSetPosition(12, x, 0xd20000);
    Engine_ActorSetSpritePriority(10, 3);
    Engine_ActorSetSpritePriority(11, 3);
    Engine_ActorSetSpritePriority(12, 3);
    Engine_ActorSetChildValue(10, 3);
    Engine_ActorSetChildValue(11, 3);
    Engine_ActorSetChildValue(12, 3);
    actor = Object_GetById(10);
    scale = 0x8000;
    update = (s32)FuneHobashira_GrowActorScale;
    actor->scale_y = scale;
    actor->scale_x = scale;
    actor->update = (void (*)(union FieldObject *))update;
    actor = Object_GetById(11);
    actor->scale_y = scale;
    actor->scale_x = scale;
    actor->update = (void (*)(union FieldObject *))update;
    actor = Object_GetById(12);
    actor->scale_y = scale;
    actor->scale_x = scale;
    actor->update = (void (*)(union FieldObject *))update;
    Engine_TaskWait(1);
    Actor_SetSpeed(10, 0x851e, 0x428f);
    Actor_SetSpeed(11, 0x7333, 0x3999);
    Actor_SetSpeed(12, 0x9999, 0x4ccc);
    Actor_SetDestination(10, 128, 345);
    Actor_SetDestination(11, 136, 330);
    Engine_ActorSetDestination(12, 156, 340);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(8, 2);
    Actor_WalkToAndWait(8, 164, 344);
    Engine_ActorJump(8, 4, 10);
    Engine_ActorJump(8, 6, 40);
    Engine_ActorStartRepeatedMotion(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 20);
    (*event)->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(11);
    Engine_EventEnd();
}

void FieldScene_RunActorNinePresentationCycles(void)
{
    u32 i;
    struct FieldActor *rec9;
    struct FieldActor *record;

    Engine_EventBegin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = Object_GetById(0);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_TaskWait(1);
    Event_CallWithLastActiveObjectId((s32)FuneHobashira_EnsembleObjects);
    Engine_TaskWait(1);
    OverlayObject_InitWithRandomFields(9);
    OverlayObject_InitWithRandomFields(10);
    OverlayObject_InitWithRandomFields(11);
    OverlayObject_InitWithRandomFields(12);
    OverlayObject_InitWithRandomFields(13);
    OverlayObject_InitWithRandomFields(14);
    OverlayObject_InitWithRandomFields(15);
    Engine_ActorEnableActionCallback(8, FuneHobashira_LookoutActions);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(120);
    rec9 = Object_GetById(9);
    Engine_ActorStop(9);
    rec9->target_x = ACTOR_NO_TARGET;
    rec9->target_y = ACTOR_NO_TARGET;
    rec9->target_z = ACTOR_NO_TARGET;
    rec9->velocity_x = 0;
    rec9->velocity_y = 0;
    rec9->velocity_z = 0;
    rec9->rise_base_y = 0;
    Engine_EventWait(20);
    Actor_SetSpeed(9, 0x80000, 0x40000);
    FieldObject_SetPosition(rec9, 0xa40000, 0x900000, 0x1410000);
    Object_CommitPosition(rec9);
    FieldObject_SetPosition(rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition(rec9);
    FieldObject_SetPosition(rec9, 0xcc0000, 0x7c0000, 0x1410000);
    Object_CommitPosition(rec9);
    FieldObject_SetPosition(rec9, 0x900000, 0, 0xa90000);
    Engine_ActorStop(8);
    Engine_TaskWait(1);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_ShowEmote(8, 0x103, 60);
    Actor_SetSpeed(9, 0x20000, 0x10000);
    OverlayObject_InitWithRandomFields(9);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(8, FuneHobashira_LookoutActions);
    Engine_EventWait(120);
    Engine_ActorStop(9);
    rec9->target_x = ACTOR_NO_TARGET;
    rec9->target_y = ACTOR_NO_TARGET;
    rec9->target_z = ACTOR_NO_TARGET;
    rec9->velocity_x = 0;
    rec9->velocity_y = 0;
    rec9->velocity_z = 0;
    rec9->rise_base_y = 0;
    Engine_EventWait(20);
    Actor_SetSpeed(9, 0x80000, 0x40000);
    FieldObject_SetPosition(rec9, 0xa40000, 0x900000, 0x1410000);
    Object_CommitPosition(rec9);
    Actor_SetSpeed(9, 0x50000, 0x28000);
    FieldObject_SetPosition(rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition(rec9);
    FieldObject_SetPosition(rec9, 0xa40000, 0x720000, 0x1410000);
    Object_CommitPosition(rec9);
    FieldObject_SetPosition(rec9, 0xa40000, 0x680000, 0x1410000);
    Object_CommitPosition(rec9);
    FieldObject_SetPosition(rec9, 0xcc0000, 0x7c0000, 0x1410000);
    Object_CommitPosition(rec9);
    Engine_ObjectSetPosition(rec9, 0x900000, 0, 0xa90000);
    Engine_ActorStop(8);
    Engine_TaskWait(1);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_ShowEmote(8, 0x103, 60);
    Actor_SetSpeed(9, 0x20000, 0x10000);
    OverlayObject_InitWithRandomFields(9);
    Engine_ActorJump(8, 4, 20);
    Engine_ActorJump(8, 6, 40);
    Event_PlayCue(29);
    GameFlag_Set(0x8f0);
    Engine_EventSetMessage((s32)MsgFuneMonsters2);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_ShowEmote(8, 0x100, 0);
    Actor_WalkToAndWait(8, 164, 0x158);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(12);
    Engine_EventEnd();
}

void OverlayObject_InitWithRandomFields(s32 a)
{
    struct FieldActor *obj;
    u32 x;

    obj = Object_GetById(a);
    Engine_ActorSetSpritePriority(a, 1);
    obj->motion_flags = 0;
    obj->timer = ((u32 (*)(void))Engine_RandomNext)() >> 15;
    obj->work = ((u32 (*)(void))Engine_RandomNext)() >> 15;
    x = ((u32 (*)(void))Engine_RandomNext)();
    x <<= 2;
    x >>= 16;
    x <<= 16;
    x += 0x60000;
    obj->y.fixed = x;
    x = ((u32 (*)(void))Engine_RandomNext)();
    obj->rise_base_y = ((x * 3 << 13) >> 16) - 0x3000;
    obj->scale_x = 0x14000;
    obj->scale_y = 0x14000;
    Engine_ActorEnableActionCallback(a, FuneHobashira_DriftActions);
}

void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    struct FieldActor *actor;

    Engine_EventBegin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
    Engine_TaskWait(1);
    Event_CallWithLastActiveObjectId((s32)FuneHobashira_EnsembleObjects);
    Engine_TaskWait(1);
    Event_CallWithLastActiveObjectId((s32)FuneHobashira_LandingObjects);
    Engine_TaskWait(1);
    OverlayObject_InitWithRandomFields(9);
    OverlayObject_InitWithRandomFields(10);
    OverlayObject_InitWithRandomFields(11);
    OverlayObject_InitWithRandomFields(12);
    OverlayObject_InitWithRandomFields(13);
    OverlayObject_InitWithRandomFields(14);
    OverlayObject_InitWithRandomFields(15);
    Engine_ActorEnableActionCallback(8, FuneHobashira_LookoutActions);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(0x12c);
    Event_PlayCue(147);
    Engine_EventWait(100);
    Engine_ActorStop(9);
    Engine_ActorStop(10);
    Engine_ActorStop(11);
    Engine_ActorStop(12);
    Engine_ActorStop(13);
    Engine_ActorStop(14);
    Engine_ActorStop(15);
    Actor_SetSpeed(9, 0x30000, 0x18000);
    Actor_SetSpeed(10, 0x30000, 0x18000);
    Actor_SetSpeed(11, 0x30000, 0x18000);
    Actor_SetSpeed(12, 0x30000, 0x18000);
    Actor_SetSpeed(13, 0x30000, 0x18000);
    Actor_SetSpeed(14, 0x30000, 0x18000);
    Actor_SetSpeed(15, 0x30000, 0x18000);
    Actor_SetDestination(9, 0, 100);
    Actor_SetDestination(10, 60, 100);
    Actor_SetDestination(11, 120, 100);
    Actor_SetDestination(12, 180, 100);
    Actor_SetDestination(13, 240, 100);
    Actor_SetDestination(14, 0x140, 100);
    Actor_SetDestination(15, 0x17c, 100);
    Engine_EventWait(40);
    Actor_ShowEmote(8, 0x101, 0);
    Engine_EventWait(20);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(15, 0, 0);
    Engine_EventWait(100);
    actor = Object_GetById(18);
    actor->scale_x = 0x1999;
    actor->scale_y = 0x1999;
    Actor_SetPosition(18, 0xac0000, 0x1540000);
    Engine_ActorStop(8);
    Engine_TaskWait(1);
    Engine_ActorRunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x3000, 0);
    Event_PlayCue(29);
    GameFlag_Set(0x8f0);
    for (i = 0; i < 32; i++) {
        actor->scale_x += 0xccc;
        actor->scale_y += 0xccc;
        Engine_TaskWait(1);
    }
    Actor_ShowEmote(8, 0x101, 60);
    Engine_ActorRunRepeatedMotion(8, 2);
    Actor_WalkToAndWait(8, 168, 0x154);
    Actor_WalkToAndWait(8, 200, 0x154);
    Actor_FaceDirection(8, 0x8000, 0);
    actor = Object_GetById(17);
    actor->scale_x = 0x12666;
    actor->scale_y = 0x12666;
    actor->x.fixed = 0xac0000;
    actor->y.fixed = 0xa00000;
    actor->z.fixed = 0x1540000;
    actor->facing = 0;
    actor->bounce = 0x6666;
    actor->gravity = 0x30000;
    Engine_EventWait(20);
    Engine_ActorJump(8, 6, 20);
    Event_PlayCue(147);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(8, FuneHobashira_WaveActions);
    Engine_EventWait(80);
    Engine_ActorSetSpritePriority(17, 1);
    Actor_SetSpeed(17, 0x10000, 0x8000);
    actor->bounce = 0x1999;
    actor->gravity = 0xb333;
    Event_PlayCue(153);
    actor->velocity_y = 0x80000;
    Actor_SetDestination(17, 132, 0x168);
    Actor_SetDestination(18, 132, 0x168);
    Engine_EventWait(40);
    Actor_SetPosition(17, 0, 0);
    actor = Object_GetById(8);
    actor->scale_x = 0x10000;
    actor->scale_y = 0x10000;
    actor->facing = 0x5000;
    Engine_EventWait(40);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(13);
    Engine_EventEnd();
}

void FieldScene_RunSevenActorEnsemble(void)
{
    Engine_EventBegin();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
    Event_CallWithLastActiveObjectId((s32)FuneHobashira_EnsembleObjects);
    Engine_TaskWait(1);
    OverlayObject_InitWithRandomFields(9);
    OverlayObject_InitWithRandomFields(10);
    OverlayObject_InitWithRandomFields(11);
    OverlayObject_InitWithRandomFields(12);
    OverlayObject_InitWithRandomFields(13);
    OverlayObject_InitWithRandomFields(14);
    OverlayObject_InitWithRandomFields(15);
    Engine_ActorEnableActionCallback(8, FuneHobashira_LookoutActions);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(400);
    Engine_ActorStop(9);
    Engine_ActorStop(10);
    Engine_ActorStop(11);
    Engine_ActorStop(12);
    Engine_ActorStop(13);
    Engine_ActorStop(14);
    Engine_ActorStop(15);
    Actor_SetSpeed(9, 0x30000, 0x18000);
    Actor_SetSpeed(10, 0x30000, 0x18000);
    Actor_SetSpeed(11, 0x30000, 0x18000);
    Actor_SetSpeed(12, 0x30000, 0x18000);
    Actor_SetSpeed(13, 0x30000, 0x18000);
    Actor_SetSpeed(14, 0x30000, 0x18000);
    Actor_SetSpeed(15, 0x30000, 0x18000);
    Engine_ActorEnableActionCallback(9, FuneHobashira_EnsembleWalk9);
    Engine_ActorEnableActionCallback(10, FuneHobashira_EnsembleWalk10);
    Engine_ActorEnableActionCallback(11, FuneHobashira_EnsembleWalk11);
    Engine_ActorEnableActionCallback(12, FuneHobashira_EnsembleWalk12);
    Engine_ActorEnableActionCallback(13, FuneHobashira_EnsembleWalk13);
    Engine_ActorEnableActionCallback(14, FuneHobashira_EnsembleWalk14);
    Engine_ActorEnableActionCallback(15, FuneHobashira_EnsembleWalk15);
    Engine_EventWait(40);
    Engine_ActorStartRepeatedMotion(8, 3);
    Actor_SetAttachedEffect(8, 258);
    Engine_EventWait(120);
    Engine_ActorStartRepeatedMotion(8, 1);
    Actor_ShowEmote(8, 256, 60);
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_WalkToAndWait(8, 164, 344);
    Engine_ActorJump(8, 4, 10);
    Engine_ActorJump(8, 6, 20);
    Engine_EventSetMessage((s32)MsgFuneLandHo);
    Event_ShowMessageAndWait(8, 0, 20);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    gGameState.saved_scene = (s32)&SceneId_FuneHeya;
    gGameState.saved_entrance = 2;
    if (SceneData_GetDifferenceOfPairSums() == 11) {
        Engine_EventRequestExit(15);
    } else {
        Engine_EventRequestExit(14);
    }
    Engine_EventEnd();
}

void FuneHobashira_UpdateSway(void)
{
    s32 *pos = *((s32 **)gMapWork[0]);
    s32 dx = Engine_MathCos(FuneHobashira_SwayX);
    s32 dy = Engine_MathSin(FuneHobashira_SwayY);

    *pos++ += dx;
    dy <<= 2;
    *pos += dy;
    FuneHobashira_DriftX += dx;
    FuneHobashira_DriftY += dy;
    FuneHobashira_SwayX += (u32)(((u32 (*)(void))Engine_RandomNext)() * 3 << 7) >> 16;
    FuneHobashira_SwayY += (u32)(((u32 (*)(void))Engine_RandomNext)() << 9) >> 16;
    FuneHobashira_SwayX &= 0xffff;
    FuneHobashira_SwayY &= 0xffff;
}

s32 SceneData_GetDifferenceOfPairSums(void)
{
    s32 a;
    s32 b;

    a = SceneData_GetValueByFirstSetFlag(0);
    a += SceneData_GetValueByFirstSetFlag(2);
    b = SceneData_GetValueByFirstSetFlag(1);
    b += SceneData_GetValueByFirstSetFlag(3);
    return a - b;
}

s32 SceneData_GetValueByFirstSetFlag(u32 a)
{
    s32 base = 0;
    u32 i;

    switch (a) {
    case 0:
        base = 0x92c;
        break;
    case 1:
        base = 0x935;
        break;
    case 2:
        base = 0x917;
        break;
    case 3:
        base = 0x990;
        break;
    }
    for (i = 0; i <= 8; i++) {
        if (Engine_GameFlagIsSet(base + i) != 0)
            return FuneHobashira_FlagValues[i];
    }
    return 0;
}
