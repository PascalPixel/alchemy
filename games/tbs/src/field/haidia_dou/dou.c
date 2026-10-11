#define NULL ((void *)0)
#include "fieldobj.h"
#include "mapcopy.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "create_configured_overlay_object.h"
#include "staged_actor.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "field_effect.h"
#include "scene_ids.h"

void HaidiaDou_ApplyEntryState();
void HaidiaDou_SinkPillarColumn27();
void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void SceneActor_PositionPair();

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

struct Rec_3a6 {
    u8 pad00[8];
    s32 f8;
    u8 pad0c[4];
    s32 f16;
    u8 pad14[14];
    u8 f34;
    u8 f35;
};

extern const u32 HaidiaDou_Exits[];
extern const s32 HaidiaDou_WalkTargets[];

s32 SceneActor_FaceActorZero(struct FieldActor *obj);

void Engine_TaskWait(s32 frames);
void Engine_EventWait(s32 frames);
void Engine_ActorWaitForMove(s32 actor);
void Engine_ActorSetAnimation(s32 actor, s32 mode);
void Engine_ActorSetSpeed(s32 actor, s32 speed, s32 acceleration);
void Engine_ActorSetDestinationOffset(s32 actor, s32 dx, s32 dz);

void Object_CommitPosition(void *);
void BattleFx_RunPageEffectForSlot(s32, s32, s32);
s32 ArcTan2(s32 dz, s32 dx);
void HaidiaDou_SinkPillarColumn27();
s32 Object_CheckMovementCollision();
void SceneActor_WaitActorDescent(struct FieldActor *actor);
void SceneActor_PositionPair();
void SceneActor_PlaceAtTile(s32 width, s32 top, s32 bottom);

s32 StagedActor_FillGridAttributeRectangle(u32 layer, s32 x, s32 z, u32 width, u32 height, s32 value);

void SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
    work->sprite->priority = mode;
}

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct FieldActor *obj = Engine_ObjectCreate(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        struct FieldSprite *object = obj->sprite;

        object->priority = 0;
        obj->motion_flags = 0;
        obj->collision_flags = 8;
        Engine_ActorSetSpriteFlags(obj, 0);
        ObjectGroup_SetChildValue(obj, 14);
        Engine_ObjectSetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredB(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct FieldActor *result = Engine_ObjectCreate(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        struct FieldSprite *object = result->sprite;

        object->priority = 1;
        result->motion_flags = 0;
        result->collision_flags = 8;
        Engine_ActorSetSpriteFlags(result, 0);
        ObjectGroup_SetChildValue(result, 15);
        result->priority_flags = (result->priority_flags & 0xfe) | 2;
        return result;
    }
    return NULL;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/move.c"
#include "../common/effect/spawnov.c"
#include "../common/object/staged_actor.c"

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

    obj->facing = ArcTan2(dz, dx);
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

void HaidiaDou_RunProbedColumnScene(void)
{
    struct FieldActor *record;
    s32 v6;
    s32 v5;
    s32 two;
    struct StagedActorProbe probe;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&probe) != 0) {
        SceneActor_MoveAndRedraw(probe);
        if ((probe.position_x >> 20) == 17) {
            Engine_ActorSetAnimation(probe.actor_slot, 3);
            v6 = 0;
            Object_GetById(probe.actor_slot)->motion_flags = v6;
            record = Object_GetById(probe.actor_slot);
            record->bounce = v6;
            Engine_ActorSetDestinationOffset(probe.actor_slot, -12, 0);
            Engine_ActorWaitForMove(probe.actor_slot);
            Engine_ActorSetAnimation(probe.actor_slot, 3);
            Engine_ActorSetSpritePriority(10, 3);
            Object_GetById(probe.actor_slot)->motion_flags = 3;
            Engine_ActorSetDestinationOffset(probe.actor_slot, -6, 0);
            SceneActor_WaitActorDescent(Object_GetById(probe.actor_slot));
            Engine_ActorSetAnimation(probe.actor_slot, 8);
            {
                struct FieldActor *obj = Object_GetById(probe.actor_slot);

                two = 2;
                obj->priority_flags = two;
            }
            v5 = 4;
            StagedActor_FillGridAttributeRectangle(0, (probe.position_x >> 20), ((probe.position_z >> 20) - 2), 1, v5, v6);
            StagedActor_FillGridAttributeRectangle(2, (probe.position_x >> 20), ((probe.position_z >> 20) - 2), 1, v5, v6);
            StagedActor_FillGridAttributeRectangle(2, 16, 18, 1, two, v6);
            StagedActor_FillGridAttributeRectangle(0, 16, 16, 1, v5, v6);
            Engine_GameFlagSet(0x203);
            Engine_AudioPlayCue(240);
        }
    }
    Engine_EventEnd();
}

void HaidiaDou_ToppleActorNine(void)
{

    u32 i;
    struct FieldActor *record;
    s32 zero;

    Engine_EventBegin();
    Engine_EventWait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x1999);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
    Engine_EventWait(15);
    Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 8, 0);
    Engine_EventWait(4);
    Event_PlayCue(0x120);
    Event_PlayCue(239);
    Actor_SetSpeed(9, 0x8000, 0x1999);
    Engine_ActorSetAnimation(9, 2);
    zero = 0;
    Object_GetById(9)->motion_flags = zero;
    record = Actor_Get(9);
    record->bounce = zero;
    Engine_ActorSetDestinationOffset(9, 12, 0);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_ActorWaitForMove(9);
    Event_PlayCue(0x120);
    Event_PlayCue(213);
    Engine_ActorSetAnimation(9, 3);
    Object_GetById(9)->motion_flags = 3;
    Engine_ActorSetDestinationOffset(9, 6, 0);
    SceneActor_WaitActorDescent(Actor_Get(9));
    Engine_ActorSetAnimation(9, 8);
    Engine_ActorSetSpritePriority(9, 3);
    Object_GetById(9)->priority_flags = 2;
    StagedActor_FillGridAttributeRectangle(0, 12, 16, 1, 4, 0);
    StagedActor_FillGridAttributeRectangle(0, 13, 16, 1, 4, 0);
    GameFlag_Set(0x202);
    Event_PlayCue(240);
    Engine_EventEnd();
}

void HaidiaDou_RunLoweredActorScene(void)
{
    s32 record;
    struct StagedActorProbe probe;

    Engine_EventBegin();
    if (StagedActor_FindClearPosition(&probe) != 0) {
        SceneActor_MoveAndRedraw(probe);
        Engine_ActorSetAnimation(11, 3);
        Actor_SetSpeed(11, 0x4000, 0x8000);
        Engine_ActorSetDestinationOffset(11, 0, -16);
        Engine_EventWait(45);
        Engine_AudioPlayCue(240);
        Engine_ActorSetAnimation(11, 8);
        {
            struct FieldActor *obj = Object_GetById(11);
            s32 two = 2;
            s32 zero = 0;

            obj->priority_flags = two;
            StagedActor_FillGridAttributeRectangle(0, 13, (probe.position_z >> 20) - 1, 4, two, zero);
        }
        if ((probe.position_z >> 20) == 20) {
            Engine_GameFlagSet(0x205);
        } else {
            Engine_GameFlagSet(0x204);
            {
                s32 column = 14;

                Engine_MapCopyCellAttributes(14, 17, 2, 1, column, 16);
                Engine_MapCopyCellAttributes(14, 13, 1, 1, column, 15);
            }
        }
    }
    Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_HaidiaDou1) {
        return gHaidiaDouEvents1;
    }
    if (scene == (s32)&SceneId_HaidiaDou2) {
        return gHaidiaDouEvents2;
    }
    if (scene == (s32)&SceneId_HaidiaDou3) {
        return gHaidiaDouEvents3;
    }
    return gHaidiaDouEventsOther;
}

s32 HaidiaDou_RunSceneScript(void)
{
    s32 *request = &gEventWork->start_transition;

    *request = 0x204;
    if (gGameState.scene == (s32)&SceneId_HaidiaDou1) {
        *request = 0x100;
        Engine_TaskWait(1);
        Engine_ActorSetSpritePriority(11, 3);
        Engine_ActorSetSpritePriority(12, 3);
        Engine_GameFlagClear(0x12f);
    }
    HaidiaDou_ApplyEntryState();
    return 0;
}

void HaidiaDou_PlayCueAndRequestTouchedExit(void)
{
    s32 val = gEventWork->touched_trigger;

    Engine_AudioPlayCue(123);
    Engine_EventRequestExit(val);
}

void DialogueLayout_ConfigureRowsByFlag301(void)
{
    Engine_MapCopyCellAttributes(0, 34, 13, 3, 23, 34);

    if (Engine_GameFlagIsSet(0x301) != 0) {
        SceneActor_PlaceAtTile(11, 35, 35);
        Engine_MapCopyCellAttributes(24, 34, 1, 3, 23, 34);
    } else {
        SceneActor_PlaceAtTile(11, 23, 35);
        Engine_MapCopyCellAttributes(24, 34, 1, 3, 35, 34);
    }
}

void SceneActor_PositionPair(s32 a0, s32 a1, s32 a2)
{

    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Object_GetById(gGameState.selected_actor);
    q = Object_GetById(a0);
    Engine_EventBegin();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        ((void (*)(Obj *, s32, s32, s32))Engine_ObjectSetPosition)(p, x, p->f0c, y);
    }
    Object_SetMode(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        ((void (*)(Obj *, s32, s32, s32))Engine_ObjectSetPosition)(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Object_SetMode(q, 4);
    } else {
        Object_SetMode(q, 3);
    }
    Object_CommitPosition(p);
    Engine_EventEnd();
}

void FieldScene_RunShiftAndSetFlag301(void)
{

    Engine_AudioPlayCue(241);
    SceneActor_PositionPair(11, 112, 0);
    SceneActor_PositionPair(11, 80, 0);
    Engine_GameFlagSet(0x301);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureRowsByFlag301();
    Engine_AudioPlayCue(0x121);
}

void FieldScene_RunActor11Transition301(void)
{

    Engine_AudioPlayCue(241);
    SceneActor_PositionPair(11, -112, 0);
    SceneActor_PositionPair(11, -80, 0);
    Engine_GameFlagClear(0x301);
    Engine_TaskWait(2);
    DialogueLayout_ConfigureRowsByFlag301();
    Engine_AudioPlayCue(0x121);
}

void SceneActor_PlaceAtTile(s32 id, s32 x, s32 y)
{
    struct Rec_3a6 *rec = Object_GetById(id);

    if (rec != 0) {
        Engine_ActorSetSpritePriority(id, 3);
        rec->f34 = 2;
        rec->f35 |= 2;
        rec->f8 = (x << 20) + 0x80000;
        rec->f16 = (y << 20) + 0x80000;
    }
}

void HaidiaDou_ApplyEntryState(void)
{
    s32 flag;

    if (gGameState.scene == (s32)&SceneId_HaidiaDou2) {
        switch (gGameState.entrance) {
        case 1:
        case 2:
        case 3:
        case 4:
            Engine_ActorSetSpritePriority(15, 3);
            Engine_ActorSetSpritePriority(13, 3);
            OverlayObject_CreateConfiguredB(0x780000, 0, 0xe80000, 223);
            break;
        case 5:
        case 6:
        case 7:
            if (Engine_GameFlagIsSet(0x70) != 0) {
                break;
            }
            if (Engine_GameFlagIsSet(0x302) == 0) {
                break;
            }
            Engine_GameFlagSet(0x200);
            if (gGameState.entrance == 5) {
                Engine_GameFlagSet(0x201);
            }
            Engine_TaskWait(1);
            if (Engine_GameFlagIsSet(0x109) != 0) {
                break;
            }
            Actor_SetPosition(8, 0x3180000, 0x1180000);
            Object_GetById(8)->update = SceneActor_FaceActorZero;
            break;
        case 8:
        case 9:
        case 10:
            OverlayObject_CreateConfiguredB(0x2820000, 0, 0x2280000, 20);
            Map_CopyCellAttributes(23, 34, 13, 3, 0, 34);
            DialogueLayout_ConfigureRowsByFlag301();
            if (Engine_GameFlagIsSet(0x200) != 0) {
                Map_CopyCellAttributes(23, 41, 1, 1, 23, 39);
            }
            if (Engine_GameFlagIsSet(0x201) != 0) {
                Map_CopyCellAttributes(31, 39, 2, 1, 27, 41);
            }
            break;
        }
    } else if (gGameState.scene == (s32)&SceneId_HaidiaDou3) {
        switch (gGameState.entrance) {
        case 1:
        case 2:
        case 3:
            if (Engine_GameFlagIsSet(0x202) != 0) {
                StagedActor_FillGridAttributeRectangle(0, 12, 16, 1, 4, 0);
                StagedActor_FillGridAttributeRectangle(0, 13, 16, 1, 4, 0);
            } else {
                FieldScene_RedrawActorFootprint(9);
            }
            if (Engine_GameFlagIsSet(0x203) != 0) {
                StagedActor_FillGridAttributeRectangle(2, 16, 16, 1, 4, 0);
                StagedActor_FillGridAttributeRectangle(0, 16, 16, 1, 4, 0);
            } else {
                FieldScene_RedrawActorFootprint(10);
            }
            flag = Engine_GameFlagIsSet(0x205);
            if (flag != 0) {
                StagedActor_FillGridAttributeRectangle(0, 13, 19, 4, 2, 0);
            } else if (Engine_GameFlagIsSet(0x204) != 0) {
                StagedActor_FillGridAttributeRectangle(0, 13, 15, 4, 2, flag);
                Engine_MapCopyCellAttributes(14, 17, 2, 1, 14, 16);
                Engine_MapCopyCellAttributes(14, 13, 1, 1, 14, 15);
            } else {
                FieldScene_RedrawActorFootprint(11);
                Engine_ActorSetSpritePriority(11, 3);
            }
            break;
        }
    }
}

void ActorPresentation_AdvanceActorElevenStates(void)
{
    Engine_ActorSetAnimation(11, 1);
    Engine_ActorSetAnimation(11, 2);
}
