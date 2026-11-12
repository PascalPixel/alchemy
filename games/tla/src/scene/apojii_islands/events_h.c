/// @file
/// Scene code of overlay 0x679, in ROM order, from events_h's offset up to the next object the overlay links.

#include "local.h"
#include "scene_ids.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Func_02001140(struct Actor *object) {
  s32 phase = gFrameCount & 15;

  if (phase == 0) {
    struct Actor *child =
        Import_ActorCreate_(0x10d, object->x, object->y, object->z);

    if (child != 0) {
      struct FieldSprite *sprite = child->sprite;

      Func_02001c74(child, gData_02001ef4);
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

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct Actor *object;
};

void Func_020011a8(void) {
  struct EventWork1cc *work = Ram_HeapSlots->eventWork;
  s32 owner = gMainRoutineWork.currentOwner;
  struct Actor *anchor = work->object;
  struct Actor *leader = Actor_Get(owner);
  s32 position[3];
  s32 result;

  if (gMainRoutineWork.scene != (s32)&SceneId_Ov67bRoom1 ||
      leader->terrainHeight != 0) {
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z + 0x20000;
    result = Func_02001c9c(leader, position);
    if ((gFrameCount & 4) == 0)
      Func_020010d8(leader);
    if (result < 0) {
      /* FAKEMATCH(wrapper): Call2 kept; as a direct call, 2 of 151 instructions
       * differ in tla-en. */
      Actor_SetAttachedEffect(owner, 0x102);
      FieldActor_SetMoveTarget(leader, leader->x, leader->y, leader->z + 0x80000);
      Func_02001c6c(leader, 49);
      Actor_WaitForMove(leader);
      do {
        System_Wait(1);
      } while (leader->y != leader->terrainHeight);
      Func_020010d8(leader);
      Func_02001c6c(leader, 49);
      System_Wait(3);
    } else {
      position[0] = leader->x;
      position[1] = leader->y;
      position[2] = leader->z + 0x80000;
      result = Func_02001c9c(leader, position);
      if (result <= 0) {
        position[0] = leader->x + 0x5b333;
        position[1] = leader->y;
        position[2] = leader->z + 0x5b333;
        result = Func_02001c9c(leader, position);
        if (result <= 0) {
          position[0] = leader->x - 0x5b333;
          position[1] = leader->y;
          position[2] = leader->z + 0x5b333;
          result = Func_02001c9c(leader, position);
          if (result <= 0) {
            anchor->z += 0x20000;
            leader->z += 0x20000;
            leader->facing = 0x4000;
          }
        }
      }
    }
  }
}

void Func_02001308(s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object != 0) {
    u8 *meta = ResourceMetadata_Register(object->sprite, 0x17d);

    meta[5] = 0;
    meta[6] = 0;
    Func_02001c6c(object, 0);
    Func_02001c6c(object, 2);
  }
}

void Func_02001338(s32 entrance) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 i;

  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  leader->motionFlags = 0;
  leader->priorityFlags |= 0x20;
  Event_PlaySound(215);
  Func_02001c6c(leader, 18);
  Event_PlaySound(612);
  for (i = 0; i < 60; i++) {
    s32 angle;

    if (i == 30)
      Event_FinishScreenTransition();
    leader->y -= 0x4000;
    angle = leader->facing;
    leader->facing = angle + 0x1000;
    if ((i & 7) == 0)
      Func_020010d8(Actor_Get(15));
    System_Wait(1);
  }
  Func_02001ccc();
  FieldEvent_SetExitRequest(entrance);
}

/// Every other frame, emits particle 7 (5 on every other such frame) from the
/// object, 26 pixels up, swaying with the frame count and drifting by a random
/// amount.
s32 Func_020013d8(struct Actor *object) {
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

void Func_02001464(s32 entrance) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 *owner = &gMainRoutineWork.currentOwner;

  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(228);
  leader->update = (void (*)(void))Func_020013d8;
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
  Func_02001ccc();
}

void Func_02001500(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 flag = Event_GetFlag(0x109);
  s32 *owner = &gMainRoutineWork.currentOwner;
  s32 offset;

  if (flag == 0) {
    Func_02001cc4();
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
    leader->update = (void (*)(void))Func_020013d8;
    Actor_SetSpeed(*owner, 0x6666, 0x3333);
    Actor_WalkByAndWait(*owner, 0, 8);
    Actor_SetSpriteColor(Actor_Get(*owner), 0);
    Actor_SetSingleChildFlags(Actor_Get(*owner), 1);
    ((struct FieldSprite *)leader->sprite)->priority = 1;
    offset = 10;
    Actor_WalkByAndWait(*owner, 0, offset);
    leader->motionFlags = 3;
    leader->update = (void (*)(void))flag;
    Func_02001df4();
    Event_WaitTransitionFrames();
    Func_02001ccc();
  }
}
