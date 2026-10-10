#include "battle.h"
#define FIELD_STAGED_ACTOR_IMPORTS
#include "field_event.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "haidia.h"
#include "edition.h"
#include "types.h"
#include "field_scene.h"
#include "field_effect.h"




struct FieldActor *OverlayObject_CreateConfigured(s32 x, s32 y, s32 z, s32 kind);

void SceneState_SetValues8_3_4(void)
{
    BattleFx_RunPageEffectForSlot(8, 3, 4);
}

void SceneActor_WaitActorDescent(struct FieldActor *actor)
{
    s32 frames = 60;

    while (frames != 0) {
        Engine_TaskWait(1);
        frames--;
        if (actor->y.fixed <= actor->terrain_height)
            break;
    }
    actor->y.fixed = actor->terrain_height;
}

s32 SceneActor_FaceActorZero(struct FieldActor *obj)
{
    struct FieldActor *target = Object_GetById(ACTOR_PARTY_LEADER);
    s32 dz = target->z.fixed - obj->z.fixed;
    s32 dx = target->x.fixed - obj->x.fixed;

    obj->facing = (s16)ArcTan2(dz, dx);
    return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_HaidiaDou1) {
        return gHaidiaDouEntrances1;
    }
    if (scene == (s32)&SceneId_HaidiaDou2) {
        return gHaidiaDouEntrances2;
    }
    if (scene == (s32)&SceneId_HaidiaDou3) {
        return gHaidiaDouEntrances3;
    }
    return gHaidiaDouEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return HaidiaDou_Exits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_HaidiaDou1) {
        return gHaidiaDouPlacements1;
    }
    if (scene == (s32)&SceneId_HaidiaDou2) {
        return gHaidiaDouPlacements2;
    }
    if (scene == (s32)&SceneId_HaidiaDou3) {
        return gHaidiaDouPlacements3;
    }
    return gHaidiaDouPlacements1;
}

void FieldScene_ConfigureRegionAtRow15(void)
{
    Engine_MapCopyCells(16, 15, 1, 1, 15, 15);
}

void FieldScene_ConfigureRegionAtRow17(void)
{
    Engine_MapCopyCells(16, 17, 1, 1, 15, 15);
}

void HaidiaDou_SinkPillarColumn23(void)
{
    struct EffectOptions options;
    struct EffectOptions *o;
    struct FieldActor *left;
    struct FieldActor *right;
    u32 i;

    Engine_EventBegin();
    if (Object_GetById(9)->x.fixed >> 20 == 23) {
        Actor_MoveToAndWait(0, 360, 664);
        Engine_ActorFaceDirection(0, 0xe000, 10);
        Object_GetById(9)->x.fixed += 0x20000;
        left = OverlayObject_CreateConfigured(Object_GetById(9)->x.fixed, 0,
                                              Object_GetById(9)->z.fixed + 0x340000, 241);
        right = OverlayObject_CreateConfigured(Object_GetById(9)->x.fixed + 0x100000, 0,
                                               Object_GetById(9)->z.fixed + 0x340000, 241);
        Object_GetById(9)->motion_flags = 0;
        o = &options;
        o->start_scale_x = 0x9999;
        o->start_scale_y = 0x9999;
        o->palette = 7;
        Engine_AudioPlayCue(216);
        for (i = 0; i < 68; i++) {
            s32 x = (((u32)(Engine_RandomNext() * 17) >> 16) << 16) + 0x1700000;
            s32 z = (((u32)(Engine_RandomNext() * 14) >> 16) << 16) + 0x2700000;

            Effect_Spawn(x, 0, z, 0, 0, 0, 0x90000, o);
            Object_GetById(9)->y.fixed -= 0x8000;
            Engine_EventWait(1);
        }
        Map_CopyCellAttributes(23, 41, 1, 1, 23, 39);
        Object_GetById(9)->priority_flags |= 2;
        Engine_GameFlagSet(0x200);
        Object_GetById(9)->y.fixed = -0x80000;
        Engine_ActorSetAnimation(9, 2);
        Engine_ObjectDispatchRelease(left);
        Engine_ObjectDispatchRelease(right);
        Engine_EventWait(30);
    }
    Engine_EventEnd();
}

void HaidiaDou_SinkPillarColumn27(void)
{
    struct EffectOptions options;
    struct EffectOptions *o;
    struct FieldActor *left;
    struct FieldActor *right;
    u32 i;

    Engine_EventBegin();
    if (Object_GetById(10)->x.fixed >> 20 == 27) {
        left = OverlayObject_CreateConfigured(Object_GetById(10)->x.fixed - 0x80000, 0,
                                              Object_GetById(10)->z.fixed + 0x340000, 241);
        right = OverlayObject_CreateConfigured(Object_GetById(10)->x.fixed + 0x80000, 0,
                                               Object_GetById(10)->z.fixed + 0x340000, 241);
        Object_GetById(9)->motion_flags = 0;
        o = &options;
        o->start_scale_x = 0x9999;
        o->start_scale_y = 0x9999;
        o->palette = 7;
        Engine_AudioPlayCue(216);
        for (i = 0; i < 68; i++) {
            s32 x = (((u32)(Engine_RandomNext() * 17) >> 16) << 16) + 0x1b00000;
            s32 z = (((u32)(Engine_RandomNext() * 14) >> 16) << 16) + 0x2900000;

            Effect_Spawn(x, 0, z, 0, 0, 0, 0x90000, o);
            Object_GetById(10)->y.fixed -= 0x8000;
            Engine_EventWait(1);
        }
        Map_CopyCellAttributes(31, 39, 2, 1, 27, 41);
        Object_GetById(10)->priority_flags |= 2;
        Engine_GameFlagSet(0x201);
        Object_GetById(10)->y.fixed = -0x80000;
        Engine_ActorSetAnimation(10, 2);
        Engine_ObjectDispatchRelease(left);
        Engine_ObjectDispatchRelease(right);
        Engine_EventWait(30);
    }
    Engine_EventEnd();
}

void FieldScene_RunInitBracketThenSequence(void)
{
    Engine_EventBegin();
    StagedActor_AdvancePair();
    Engine_EventEnd();
    HaidiaDou_SinkPillarColumn27();
}

s32 FieldScene_RunPrimarySequence(s32 a0)
{

    union FieldCoordinate to[3];
    struct FieldActor *leader;
    u8 *flags;
    s32 saved;

    leader = Object_GetById(ACTOR_PARTY_LEADER);
    flags = &leader->motion_flags;
    saved = *flags;
#if EDITION_INTERNATIONAL
    to[0].fixed = (leader->x.fixed & -0x100000) + 0x80000;
    to[1].fixed = leader->y.fixed;
    to[2].fixed = (leader->z.fixed & -0x100000) + 0x280000;
#else
    to[0].fixed = leader->x.fixed;
    to[1].fixed = leader->y.fixed;
    to[2].fixed = leader->z.fixed + 0x200000;
#endif
    if (Object_CheckMovementCollision(leader, to) == 0) {
        Engine_EventBegin();
        Object_SetMode(leader, 6);
        Engine_TaskWait(6);
        Engine_AudioPlayCue(152);
        Object_SetMode(leader, 7);
        leader->speed = 0x30000;
        leader->acceleration = 0x20000;
        leader->velocity_y = 0x40000;
        *flags &= 126;
        Engine_ActorSetSpriteFlags(leader, 0);
        Engine_ActorMoveToAndWait(0, to[0].part.pixel, to[2].part.pixel);
        Object_SetMode(leader, 6);
        Engine_ActorSetSpriteFlags(leader, 1);
        *flags = saved;
        Engine_EventEnd();
        return 1;
    }
    return 0;
}

void FieldScene_RunScene3a6SequenceA(void)
{

    u32 i;
    struct FieldActor *record;

    if (GameFlag_IsSet(0x200) == 0) {
        GameFlag_Set(0x200);
        Engine_EventBegin();
        Camera_SetSpeed(0x10000, 0x2000);
        Engine_CameraFollowActor(8, 1);
        Engine_CameraWaitForMove();
        Engine_EventWait(60);
        Actor_FaceDirection(8, 0xc000, 20);
        Actor_SetAttachedEffect(8, 0x102);
        Engine_ActorRunRepeatedMotion(8, 2);
        Engine_EventWait(20);
        Actor_SetSpeed(8, 0x10000, 0x8000);
        Actor_WalkToAndWait(8, 0x318, 248);
        Engine_AudioPlayCue(152);
        record = Actor_Get(8);
        record->velocity_y = 0x80000;
        Actor_WalkToAndWait(8, 0x318, 0x118);
        Engine_EventWait(20);
        Actor_FaceDirection(8, 0xc000, 20);
        Engine_EventWait(30);
        Engine_EventEnd();
    }
}

void FieldScene_RunScene3a6SequenceB(void)
{

    u32 i;
    struct FieldActor *record;

    if (GameFlag_IsSet(0x200) != 0) {
        if (GameFlag_IsSet(0x201) == 0) {
            GameFlag_Set(0x201);
            GameFlag_Set(0x302);
            Engine_EventBegin();
            Actor_SetAttachedEffect(8, 0x102);
            Engine_ActorRunRepeatedMotion(8, 2);
            Engine_EventWait(20);
            Actor_SetSpeed(8, 0x20000, 0x10000);
            Actor_WalkToAndWait(8, 0x2f8, 0x118);
            Actor_WalkToAndWait(8, 0x2f8, 0x138);
            Actor_WalkToAndWait(8, 0x318, 0x138);
            Engine_EventWait(10);
            Actor_FaceDirection(8, 0xc000, 20);
            record = Actor_Get(8);
            record->update = (void (*)(union FieldObject *))SceneActor_FaceActorZero;
            Engine_EventEnd();
        }
    }
}

void FieldScene_RunScene3a6SequenceC(void)
{

    struct FieldActor *rec8;
    struct FieldActor *record;
    s32 idx;
    s32 tbl;
    s32 idx4;
    s32 off24a;
    struct EventWork *p5;

    p5 = gEventWork;
    if (Engine_GameFlagIsSet(0x302) != 0) {
        off24a = 0x24a;
        if (*(s16 *)((s32)&gGameState + off24a) != 8) {
            idx = p5->touched_trigger;
            rec8 = Object_GetById(8);
            record = Object_GetById(ACTOR_PARTY_LEADER);
            rec8->speed = record->speed;
            rec8 = Object_GetById(8);
            record = Object_GetById(ACTOR_PARTY_LEADER);
            rec8->acceleration = record->acceleration;
            idx -= 45;
            tbl = (s32)HaidiaDou_WalkTargets;
            idx <<= 3;
            idx4 = idx + 4;
            Engine_ActorWalkTo(8, *(s32 *)(tbl + idx), *(s32 *)(tbl + idx4));
        }
    }
}

void FieldScene_RunActor8ZeroStep(void)
{
    Engine_EventBegin();
    Engine_ActorSetAnimation(8, 0);
    Engine_EventEnd();
}
