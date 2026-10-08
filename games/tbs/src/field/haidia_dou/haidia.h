#ifndef ALCHEMY_HAIDIA_H
#define ALCHEMY_HAIDIA_H

#include "mapcopy.h"

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

#define NULL ((void *)0)

#include "create_configured_overlay_object.h"
#include "staged_actor.h"
#include "select_overlay_data_by_runtime_selector.h"

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

static __inline__ void Actor_SetMotionSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: a direct call changes HaidiaDou_ToppleActorNine from sub sp, sp, #8 to ldr r6, .L0 (117/118 assembly lines). */
    Engine_ActorSetSpeed(actor, speed, acceleration);
}

void Object_CommitPosition(Obj *);
void BattleFx_RunPageEffectForSlot(s32, s32, s32);
s32 ArcTan2(s32 dz, s32 dx);
void HaidiaDou_SinkPillarColumn27();
s32 Object_CheckMovementCollision();
void SceneActor_WaitActorDescent(struct FieldActor *actor);
void SceneActor_PositionPair();
void SceneActor_PlaceAtTile(s32 width, s32 top, s32 bottom);

s32 StagedActor_FillGridAttributeRectangle(u32 layer, s32 x, s32 z, u32 width, u32 height, s32 value);

#endif
