/// @file
/// Scene code of overlay 0x67c, in ROM order, from s2bd4's offset up to the next object the overlay links.

#include "local.h"
#include "scene_ids.h"
#include "system/frame.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Func_02002bd4(struct Actor *object) {
  struct Actor *child = FieldActor_Create(
      14, object->x, object->y + 0x40000, object->z + 0x40000);
  struct FieldSprite *source = object->animation;

  if (child != 0) {
    struct FieldSprite *sprite = child->animation;

    child->terrainHeight = object->terrainHeight;
    Func_02003ccc(child, gData_0200411c);
    child->motionFlags = 0;
    if (sprite != 0) {
      Animation_ApplyChildArgument(sprite, 1);
      sprite->nextFrame = 0;
      sprite->priority = source->priority;
    }
  }
}

void Func_02002c3c(struct Actor *object) {
  s32 phase = gFrameCount & 15;

  if (phase == 0) {
    struct Actor *child =
        Import_ActorCreate_(0x10d, object->x, object->y, object->z);

    if (child != 0) {
      struct FieldSprite *sprite = child->animation;

      Func_02003ccc(child, gData_02004128);
      child->motionFlags = 0;
      child->y = -0x78000;
      child->terrainId = 1;
      if (sprite != 0) {
        Animation_ApplyChildArgument(sprite, 2);
        sprite->nextFrame = 0;
        sprite->priority = 2;
      }
    }
  }
}

struct SlideWork {
  u8 unk000[0x1cc];
  struct Actor *camera;
};

s32 Func_02003d2c(struct Actor *object, s32 *position);

void Func_02002ca4(void) {
  struct Actor *center;
  s32 id;
  struct Actor *actor;
  s32 position[3];
  s32 hit;

  {
    struct SlideWork *work = Ram_HeapSlots->eventWork;

    id = gMainRoutineWork.currentOwner;
    center = work->camera;
  }
  actor = Actor_Get(id);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67bRoom1 &&
      actor->terrainHeight == 0)
    return;
  position[0] = actor->x;
  position[1] = actor->y;
  position[2] = actor->z + 0x20000;
  hit = Func_02003d2c(actor, position);
  if ((gFrameCount & 4) == 0)
    Func_02002bd4(actor);
  if (hit < 0) {
    /* FAKEMATCH(wrapper): Call2 kept; as a direct call, 2 of 151 instructions
     * differ in tla-en. */
    Actor_SetAttachedEffect(id, 258);
    FieldActor_SetMoveTarget(actor, actor->x, actor->y, actor->z + 0x80000);
    Func_02003cc4(actor, 49);
    Actor_WaitForMove(actor);
    do {
      System_Wait(1);
    } while (actor->y != actor->terrainHeight);
    Func_02002bd4(actor);
    Func_02003cc4(actor, 49);
    System_Wait(3);
    return;
  }
  position[0] = actor->x;
  position[1] = actor->y;
  position[2] = actor->z + 0x80000;
  hit = Func_02003d2c(actor, position);
  if (hit > 0)
    return;
  position[0] = actor->x + 0x5b333;
  position[1] = actor->y;
  position[2] = actor->z + 0x5b333;
  hit = Func_02003d2c(actor, position);
  if (hit > 0)
    return;
  position[0] = actor->x - 0x5b333;
  position[1] = actor->y;
  position[2] = actor->z + 0x5b333;
  hit = Func_02003d2c(actor, position);
  if (hit > 0)
    return;
  center->z += 0x20000;
  actor->z += 0x20000;
  actor->angle = 0x4000;
}

void Func_02002e04(s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object != 0) {
    u8 *meta = ResourceMetadata_Register(object->animation, 0x17d);

    meta[5] = 0;
    meta[6] = 0;
    Func_02003cc4(object, 0);
    Func_02003cc4(object, 2);
  }
}

void Func_02002e34(s32 entrance) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 i;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  leader->motionFlags = 0;
  leader->priorityFlags |= 0x20;
  Event_PlaySound(215);
  Func_02003cc4(leader, 18);
  Event_PlaySound(612);
  for (i = 0; i < 60; i++) {
    s32 angle;

    if (i == 30)
      Event_FinishScreenTransition();
    leader->y -= 0x4000;
    angle = leader->angle;
    leader->angle = angle + 0x1000;
    if ((i & 7) == 0)
      Func_02002bd4(Actor_Get(15));
    System_Wait(1);
  }
  Func_02003d84();
  FieldEvent_SetExitRequest(entrance);
}

void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 flags,
                   s32 *params);

/// On odd frames, spawns a drifting effect above the object, swaying with the
/// frame count; Func_02002030 of Aqua Rock is the same function.
s32 Func_02002ed4(struct Actor *object) {
  s32 params[10];

  if (gFrameCount & 1) {
    s32 drift;
    s32 x;
    s32 y;

    params[1] = 7;
    if ((gFrameCount & 2) == 0)
      params[1] = 5;
    params[2] = 0xcccc;
    params[3] = 0xcccc;
    params[0] = 0;
    drift = ((u32)(Random16Far() << 3) >> 16) * 0x3333;
    x = object->x;
    y = object->y;
    Func_0200015c(x + ((8 - (gFrameCount & 15)) << 16), y + 0x1a0000, object->z,
                  0, -drift, 0, 0xb0000, params);
  }
  return 0;
}

void Func_02002f60(s32 entrance) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 *owner = &gMainRoutineWork.currentOwner;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(228);
  leader->update = (void (*)(void))Func_02002ed4;
  leader->motionFlags = 0;
  leader->speedLimit = 0x3333;
  Actor_SetAnimation(*owner, 2);
  Actor_SetDestinationOffset(*owner, 0, -8);
  FieldActor_WaitForMoveById(*owner);
  Actor_SetSpriteColor(Actor_Get(*owner), 9);
  Actor_SetSingleChildFlags(Actor_Get(*owner), 0);
  leader->update = 0;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(entrance);
  Func_02003d84();
}

void Func_02002ffc(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 flag = Event_GetFlag(0x109);
  s32 *owner = &gMainRoutineWork.currentOwner;
  s32 offset;

  if (flag == 0) {
    Func_02003d7c();
    FieldEvent_ResolvePendingActions(0);
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
    leader->motionFlags = flag;
    Actor_SetPosition(*owner, ((s16 *)&leader->x)[1] << 16,
                                           (((s16 *)&leader->z)[1] << 16) -
                                               0x100000);
    Actor_SetSpriteColor(Actor_Get(*owner), 9);
    Actor_SetSingleChildFlags(Actor_Get(*owner), 0);
    Event_StartScreenTransition();
    Event_PlaySound(228);
    leader->update = (void (*)(void))Func_02002ed4;
    Actor_SetSpeed(*owner, 0x6666, 0x3333);
    Actor_WalkByAndWait(*owner, 0, 8);
    Actor_SetSpriteColor(Actor_Get(*owner), 0);
    Actor_SetSingleChildFlags(Actor_Get(*owner), 1);
    ((struct FieldSprite *)leader->animation)->priority = 1;
    offset = 10;
    Actor_WalkByAndWait(*owner, 0, offset);
    leader->motionFlags = 3;
    leader->update = (void (*)(void))flag;
    Func_02003ed4();
    Event_WaitTransitionFrames();
    Func_02003d84();
  }
}
