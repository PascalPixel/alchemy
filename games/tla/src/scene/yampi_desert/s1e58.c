/// @file
/// Scene code of overlay 0x661, in ROM order, from s1e58's offset up to the next object the overlay links.

#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene.h"
#include "call.h"
#include "task.h"
#include "event_work.h"
#include "scene_tables.h"
#include "fixed_math.h"
#include "version.h"

void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);

void Func_02001cc4(struct Actor *object, s32 dx, s32 dz);

void Func_02001e58(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 speed = 0x18000;
  struct MapWork *map;
  struct MapCell *here, *ahead, *under;
  s32 index, column, row;
  struct MapCell *cells;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0) {
    Actor_SetAnimationSpeed(leader, 0);
    Map_EnableUpdateCallback();
    return;
  }
  map = Ram_HeapSlots->mapWork;
  index = Func_020041e0(leader);
  column = leader->x / 0x100000;
  row = (leader->z + 0x40000) / 0x100000;
  cells = map->layers[0].cells;
  here = cells + index;
  ahead = cells + (column + row * 128);
  under = map->layers[1].cells + index;
  Actor_SetAnimationSpeed(leader, 16);
  Map_DisableUpdateCallback();
  if ((u8)(under->collisionCode - 10) <= 4) {
    if (gMainRoutineWork.renderMode == 5)
      speed = 0;
    else
      Actor_SetSingleChildFlags(
          Actor_GetById(gMainRoutineWork.currentOwner), 1);
  }
  if (ahead->collisionCode == 255)
    speed = 0;
  switch (here->collisionCode) {
  case 10:
    if (gInput & 64) {
      Func_02001cc4(leader, 0, -speed >> 2);
      return;
    }
    break;
  case 11:
    if (gInput & 128) {
      Func_02001cc4(leader, 0, speed >> 2);
      return;
    }
    break;
  case 12:
    if (gInput & 32) {
      Func_02001cc4(leader, -speed >> 2, 0);
      return;
    }
    break;
  case 13:
    if (gInput & 16) {
      Func_02001cc4(leader, speed >> 2, 0);
      return;
    }
    break;
  }
  switch (under->collisionCode) {
  case 10:
    Func_02001cc4(leader, 0, -speed);
    break;
  case 11:
    Func_02001cc4(leader, 0, speed);
    break;
  case 12:
    Func_02001cc4(leader, -speed, 0);
    break;
  case 13:
    Func_02001cc4(leader, speed, 0);
    break;
  }
}

void Func_02002010(void) {
  struct FieldActor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x;
  s32 y;
  s32 z;

  y = leader->y;
  if (y > 0x100000)
    gMainRoutineWork.unk2c3 = 1;
  else
    gMainRoutineWork.unk2c3 = 0;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room3) {
    Actor_SetSpritePriority(18, leader->sprite->priority);
    x = leader->x;
    if (x > 0x1d80000) {
      z = leader->z;
      if (z <= 0x1e80000)
        return;
      if (leader->y > (s32)0xfff00000)
        gMainRoutineWork.unk2c3 = 1;
      else
        gMainRoutineWork.unk2c3 = 0;
    }
  }
}

void Func_020020a8(void) {
  struct FieldActor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  if (leader->y > -0x100000)
    gMainRoutineWork.unk2c3 = 1;
  else
    gMainRoutineWork.unk2c3 = 0;
}

void Func_020020e4(void) {
  struct FieldActor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 index = Func_020041e0(leader);
  struct MapCell *cell = map->layers[0].cells + index;
  struct MainRoutineWork *party;
  s32 y;

  if (cell->collisionCode == 20) {
    y = leader->y;
    if (y > 0x100000) {
      gMainRoutineWork.unk2c3 = 1;
      return;
    }
  }
  party = &gMainRoutineWork;
  party->unk2c3 = 0;
}

void Func_02002140(void) {
  struct FieldActor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 x = leader->x >> 20;
  u32 z = leader->z >> 20;

  if (x > 12 && z > 10 && x <= 15 && z <= 13)
    GameFlag_SetBit(263);
  else
    GameFlag_ClearBit(263);
}

void Func_02002180(void) {
  if (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 358) == 0 &&
      gFrameCount % 5 == 0) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->renderMode != 5 || gInput == 0)
      Func_020041f8(596);
  }
}

void Func_020021d4(void) {
  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->unk16a = 0;
  gData_02004808 = 0;
}

s32 Func_020021f0(struct FieldActor *object) {
  s32 x = object->targetX;

  if (x == ACTOR_NO_TARGET && object->targetZ == x)
    return 1;
  return 0;
}

void Func_02002208(struct Actor *object) {
  u8 *state = (u8 *)object + 98;

  switch (*state) {
  case 0:
    Call4((void (*)())Func_02003fe0, (s32)object, 140 << 17, 0, 200 << 16);
    *state = 1;
    break;
  case 1:
    if (Func_020021f0(object))
      *state = 2;
    break;
  case 2:
    Call4((void (*)())Func_02003fe0, (s32)object, 140 << 17, 0, 132 << 17);
    *state = 3;
    break;
  case 3:
    if (Func_020021f0(object))
      *state = 4;
    break;
  case 4:
    Call4((void (*)())Func_02003fe0, (s32)object, 184 << 16, 0, 132 << 17);
    *state = 5;
    break;
  case 5:
    if (Func_020021f0(object))
      *state = 6;
    break;
  case 6:
    Call4((void (*)())Func_02003fe0, (s32)object, 184 << 16, 0, 248 << 16);
    *state = 7;
    break;
  case 7:
    if (Func_020021f0(object)) {
      if (GameFlag_Test(0x203)) {
        object->update = NULL;
        gData_02004808 = 200;
      } else {
        *state = 8;
      }
    }
    break;
  case 8:
    Call4((void (*)())Func_02003fe0, (s32)object, 184 << 16, 0, 200 << 16);
    *state = 9;
    break;
  case 9:
    if (Func_020021f0(object))
      *state = 0;
    break;
  }
}

void Func_02001c5c(void);

/// Readies actors 16 and 17 and the room's callbacks; actor 17 becomes the
/// followed actor (gData_0200480c).
void Func_02002318(void) {
  struct Actor *leader = (struct Actor *)Actor_GetById(16);
  struct Actor *object;
  struct FieldActor **followed;

  Scene_TaskAdd(Func_02002180, 0x480);
  /* FAKEMATCH(temporary): a pointer to gData_0200480c, taken here, loads its
   * address before the call that fetches actor 17, as in the native; as the
   * global named at each use, 2 of 66 instructions differ in tla-en. */
  followed = &gData_0200480c;
  object = (struct Actor *)Actor_GetById(17);
  object->priorityFlags |= 2;
  *followed = (struct FieldActor *)object;
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  Func_020040f8(17);
  (*followed)->motionFlags = 3;
  Actor_SetSpritePriority(17, 3);
  Scene_SetSpeed(16, 0x19999, 0xcccc);
  Actor_SetPartAttribute((struct FieldActor *)leader, 15);
  Actor_SetSpritePriority(16, 3);
  Func_020040f8(16);
  Actor_GetById(16)->motionFlags = 3;
  if (!GameFlag_Test(0x109)) {
    *((u8 *)leader + 98) = 0;
    leader->update = Func_02002208;
  }
  Func_020021d4();
  Task_Add(Func_02001c5c, TASK_PASS_VBLANK);
}

/// Sends the object toward a point and moves to the next state.
#define WALK(state_, x, z)                                                     \
  case state_:                                                                 \
    Call4((void (*)())Func_02003fe0, (s32)object, x, 0, z);                    \
    *state = state_ + 1;                                                       \
    break;

/// Waits for the object to arrive before moving to the given state.
#define ARRIVE(state_, next)                                                   \
  case state_:                                                                 \
    if (Func_020021f0(object))                                                 \
      *state = next;                                                           \
    break;

void Func_020023cc(struct Actor *object) {
  u8 *state = (u8 *)object + 98;

  switch (*state) {
    WALK(0, 156 << 17, 166 << 18)
    ARRIVE(1, 2)
    WALK(2, 156 << 17, 182 << 18)
    ARRIVE(3, 4)
    WALK(4, 246 << 16, 182 << 18)
  case 5:
    if (Func_020021f0(object)) {
      if (GameFlag_Test(0x200)) {
        object->update = NULL;
        gData_02004808 = 201;
      } else {
        *state = 6;
      }
    }
    break;
    WALK(6, 184 << 16, 182 << 18)
    ARRIVE(7, 8)
    WALK(8, 184 << 16, 178 << 18)
    ARRIVE(9, 10)
    WALK(10, 140 << 17, 178 << 18)
    ARRIVE(11, 12)
    WALK(12, 140 << 17, 190 << 18)
    ARRIVE(13, 14)
    WALK(14, 232 << 16, 190 << 18)
    ARRIVE(15, 16)
    WALK(16, 232 << 16, 186 << 18)
  case 17:
    if (Func_020021f0(object)) {
      if (!GameFlag_Test(0x200))
        *state = 18;
      else
        *state = 32;
    }
    break;
    WALK(18, 232 << 16, 166 << 18)
    ARRIVE(19, 0)
  case 32:
    Func_020021d4();
    Call4((void (*)())Func_02003fe0, (s32)object, 148 << 17, 0, 186 << 18);
    *state = 33;
    break;
    ARRIVE(33, 34)
    WALK(34, 148 << 17, 190 << 18)
    ARRIVE(35, 36)
    WALK(36, 232 << 16, 190 << 18)
    ARRIVE(37, 38)
    WALK(38, 232 << 16, 186 << 18)
    ARRIVE(39, 32)
  }
}

/// Func_02002318's counterpart for actors 21 and 22: actor 22 becomes the
/// followed actor, and until flag 0x109 actor 21 is placed and given the
/// update Func_020023cc.
void Func_020026a0(void) {
  struct Actor *leader = (struct Actor *)Actor_GetById(21);
  struct Actor *object;
  struct FieldActor **followed;

  Scene_TaskAdd(Func_02002180, 0x480);
  /* FAKEMATCH(temporary): a pointer to gData_0200480c, taken here, loads its
   * address before the call that fetches actor 22, as in the native and in
   * Func_02002318; as the global named at each use, 2 of 74 instructions
   * differ in tla-en. */
  followed = &gData_0200480c;
  object = (struct Actor *)Actor_GetById(22);
  object->priorityFlags |= 2;
  *followed = (struct FieldActor *)object;
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  Func_020040f8(22);
  (*followed)->motionFlags = 3;
  Actor_SetSpritePriority(22, 3);
  Scene_SetSpeed(21, 0x19999, 0xcccc);
  Actor_SetPartAttribute((struct FieldActor *)leader, 15);
  Actor_SetSpritePriority(21, 3);
  Func_020040f8(21);
  Actor_GetById(21)->motionFlags = 3;
  if (!GameFlag_Test(0x109)) {
    Scene_Call4098(21, 232 << 16, 664 << 16, 0);
    *((u8 *)leader + 98) = 0;
    leader->update = Func_020023cc;
  }
  Func_020021d4();
  Task_Add(Func_02001c5c, TASK_PASS_VBLANK);
}

/// Sends the object toward a point and moves to the next state.
#define WALK(state_, x, z)                                                     \
  case state_:                                                                 \
    Call4((void (*)())Func_02003fe0, (s32)object, x, 0, z);                    \
    *state = state_ + 1;                                                       \
    break;

/// As WALK, after Func_020021d4.
#define WALK_ON(state_, x, z)                                                  \
  case state_:                                                                 \
    Func_020021d4();                                                           \
    Call4((void (*)())Func_02003fe0, (s32)object, x, 0, z);                    \
    *state = state_ + 1;                                                       \
    break;

/// Waits for the object to arrive before moving to the given state.
#define ARRIVE(state_, next)                                                   \
  case state_:                                                                 \
    if (Func_020021f0(object))                                                 \
      *state = next;                                                           \
    break;

/// Waits for the object to arrive, then turns off by a flag.
#define ARRIVE_OR(state_, flag, turn, next)                                    \
  case state_:                                                                 \
    if (Func_020021f0(object)) {                                               \
      if (GameFlag_Test(flag))                                                 \
        *state = turn;                                                         \
      else                                                                     \
        *state = next;                                                         \
    }                                                                          \
    break;

void Func_02002764(struct Actor *object) {
  u8 *state = (u8 *)object + 98;

  switch (*state) {
    WALK(0, 156 << 17, 132 << 17)
    ARRIVE(1, 2)
    WALK(2, 156 << 17, 172 << 17)
    ARRIVE(3, 4)
    WALK(4, 148 << 17, 172 << 17)
    ARRIVE_OR(5, 0x202, 32, 6)
    WALK(6, 168 << 16, 172 << 17)
    ARRIVE(7, 8)
    WALK(8, 168 << 16, 148 << 17)
    ARRIVE(9, 10)
    WALK(10, 184 << 16, 148 << 17)
    ARRIVE_OR(11, 0x201, 48, 12)
    WALK(12, 140 << 17, 148 << 17)
    ARRIVE(13, 14)
    WALK(14, 140 << 17, 164 << 17)
  case 15:
    if (Func_020021f0(object)) {
      if (GameFlag_Test(0x202)) {
        object->update = NULL;
        gData_02004808 = 202;
      } else {
        *state = 16;
      }
    }
    break;
    WALK(16, 140 << 17, 180 << 17)
    ARRIVE(17, 18)
    WALK(18, 200 << 16, 180 << 17)
    ARRIVE(19, 20)
    WALK(20, 200 << 16, 156 << 17)
    ARRIVE_OR(21, 0x201, 34, 22)
    WALK(22, 200 << 16, 132 << 17)
    ARRIVE(23, 0)
    WALK_ON(32, 148 << 17, 156 << 17)
    ARRIVE(33, 34)
    WALK_ON(34, 164 << 17, 156 << 17)
    ARRIVE(35, 36)
    WALK(36, 164 << 17, 188 << 17)
    ARRIVE(37, 38)
    WALK(38, 148 << 17, 188 << 17)
    ARRIVE(39, 32)
    WALK_ON(48, 184 << 16, 172 << 17)
    ARRIVE(49, 50)
    WALK(50, 168 << 16, 172 << 17)
    ARRIVE(51, 52)
    WALK(52, 168 << 16, 148 << 17)
    ARRIVE(53, 54)
    WALK(54, 184 << 16, 148 << 17)
    ARRIVE(55, 48)
  }
}

/// Func_02002318's counterpart for actors 21 and 22 in Room 5: after flag
/// 0x88b actor 21 is placed facing 0x4000; otherwise, until flag 0x109, it
/// is placed and given the update Func_02002764.
void Func_02002b84(void) {
  struct Actor *leader = (struct Actor *)Actor_GetById(21);
  struct Actor *object;
  struct FieldActor **followed;
  s32 angle;

  /* FAKEMATCH(temporary): a pointer to gData_0200480c, taken here, loads its
   * address before the call that fetches actor 22, as in the native; as the
   * global named at each use, 2 of 89 instructions differ in tla-en. */
  followed = &gData_0200480c;
  object = (struct Actor *)Actor_GetById(22);
  object->priorityFlags |= 2;
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  *followed = (struct FieldActor *)object;
  Func_020040f8(22);
  Actor_SetSpritePriority(22, 3);
  (*followed)->motionFlags = 3;
  Scene_SetSpeed(21, 0x19999, 0xcccc);
  Actor_SetPartAttribute((struct FieldActor *)leader, 15);
  Actor_SetSpritePriority(21, 3);
  Func_020040f8(21);
  Actor_GetById(21)->motionFlags = 3;
  if (GameFlag_Test(0x88b)) {
    angle = 0x4000;
    Scene_Call4098(21, 232 << 16, 168 << 16, angle);
  } else {
    Scene_TaskAdd(Func_02002180, 0x480);
    if (!GameFlag_Test(0x109)) {
      Scene_Call4098(21, 200 << 16, 264 << 16, 0);
      *((u8 *)leader + 98) = 0;
      leader->update = Func_02002764;
    }
  }
  Func_020021d4();
  Task_Add(Func_02001c5c, TASK_PASS_VBLANK);
}

void Func_02002c6c(void) {
  if (*(s16 *)((u8 *)Ram_HeapSlots->effectWork + 0x7c4) != 0)
    Func_02003ff0(73, 76, 78, 11, 1, 1);
}

void Func_02002ca0(void) {
  Func_02004190(136, 1);
  Scene_TaskAdd(Func_02002c6c, 0x480);
  Func_02004198(21, -1);
  Func_020041b0();
  Field_DispatchTypeHandler(1);
  Func_020041a0();
  Task_RemoveFar(Func_02002c6c);
  Func_020041a8();
}

void Func_02002ce0(s32 id) {
  Func_020040f8(id);
  Actor_SetPartAttribute(Actor_GetById(id), 15);
  Actor_SetSpritePriority(id, 1);
  Actor_GetById(id)->priorityFlags |= 2;
}

void Func_02002d10(void) {
  if (GameFlag_Test(265)) {
    if (GameFlag_Test(0x20d) == 0)
      Func_0200163c();
  } else {
    GameFlag_SetBit(0x20d);
  }
}

void Func_02002d40(void) {
  Func_02002ce0(11);
  Func_02002ce0(12);
  Func_02002d10();
}

void Func_02002d54(void) {
  u32 i;

  for (i = 0; i <= 6; i++)
    Func_02002ce0(i + 9);
  Func_02002d10();
}

void Func_02002d6c(struct FieldActor *object, s32 x, s32 z, s32 mode) {
  struct MainRoutineWork *party;
  s32 zero;
  u8 *flags;

  Func_02004040();
  Func_020041b8(0);
  flags = &object->motionFlags;
  zero = 0;
  party = &gMainRoutineWork;
  *flags = zero;
  Actor_SetSpeedParameters(party->currentOwner, 0x10000, 0x8000);
  Actor_SetPositionAndReset(party->currentOwner, x, z);
  Func_020040e8(party->currentOwner, 0xc000);
  Actor_SetSingleChildFlags(Actor_GetById(party->currentOwner), 0);
  Actor_SetModeById(party->currentOwner, 13);
  Func_02003fe0(object, object->x, object->y - 0xa0000, object->z);
  Func_02003fe8(object);
  Actor_SetModeById(party->currentOwner, 10);
  Func_020041f8(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004138(mode);
}

void Func_02002e14(void) {
  struct FieldActor *object;
  s32 x;
  s32 z;

  if (GameFlag_Test(0x88d)) {
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    x = object->x >> 20;
    z = object->z >> 20;
    if (x == 8) {
      if (z == 31 && !(gInput & 0x80))
        return;
      if (z == 33 && !(gInput & 0x40))
        return;
    } else {
      if (z != 32)
        return;
      if (x == 7 && !(gInput & 0x10))
        return;
      if (x == 9 && !(gInput & 0x20))
        return;
    }
    Func_02002d6c(object, 136, 516, 5);
  }
}

void Func_02002e98(void) {
  Func_02002d6c(Actor_GetById(gMainRoutineWork.currentOwner), 408, 432, 2);
}

void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Func_02000b84(s32 x, s32 y, s32 z);
void Func_02003fd8(void);
void Func_02004108(s32 actor, s32 a);
void Func_02004140(s32 a, s32 b);

void Func_02002ebc(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *leader = Actor_GetById(party->currentOwner);
  struct Actor *actor8 = Actor_GetById(8);
  /* FAKEMATCH(wrapper): the camera, walk and speed calls forwarded through
   * call.h's Call3 and Call4; as direct calls, 28 of 187 instructions differ
   * in tla-en. */

  Func_02004040();
  Func_020041b8(0);
  Func_020041f8(78);
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call4(Camera_MoveWithinMapBounds, PIXELS(232), -1, PIXELS(184), 0);
  Func_02003fd8();
  wait(1);
  Actor_SetSingleChildFlags(Actor_GetById(party->currentOwner), 0);
  leader->motionFlags = 0;
  leader->y = PIXELS(130);
  leader->gravity = Q16_ONE / 2;
  leader->bounce = 0;
  Actor_SetSingleChildFlags(Actor_GetById(8), 0);
  {
    // The angle is held in a local; as a literal argument, its constant is
    // built last.
    s32 angle = TRIG_TURN / 4;

    Call4(Func_02004098, 8, PIXELS(232), PIXELS(148), angle);
  }
  actor8->motionFlags = 0;
  actor8->y = PIXELS(130);
  actor8->gravity = Q16_ONE / 2;
  actor8->bounce = 0;
  wait(1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x203;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_020041f8(204);
  leader->motionFlags = 3;
  Actor_SetSingleChildFlags(Actor_GetById(party->currentOwner), 1);
  FieldEvent_Wait(24);
  Func_02000b84(leader->x, leader->y, leader->z);
  Func_020041f8(188);
  Func_02004108(party->currentOwner, 0x101);
  Motion_SetModeAndWaitAnimation(party->currentOwner, 49);
  Func_020041f8(204);
  actor8->motionFlags = 3;
  Actor_SetSingleChildFlags(Actor_GetById(8), 1);
  FieldEvent_Wait(24);
  Func_02000b84(actor8->x, actor8->y, actor8->z);
  Func_020041f8(188);
  Actor_ArmCallback(party->currentOwner, TRIG_TURN * 3 / 4, 0);
  FieldEvent_Wait(40);
  Func_020041f8(147);
  Actor_SetModeById(8, 3);
  FieldEvent_Wait(20);
  actor8->velocityY = Q16_ONE * 8;
  actor8->gravity = 0xe666;
  Call3(Actor_SetSpeedParameters, 8, 0x19999, 0xcccc);
  Actor_SetPositionAndCommit(8, 232, 184);
  party->battleRule24b = 2;
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov661Room10, 2);
  Func_02004140(10, 0);
}

void Func_0200316c(void);
void Func_020031b4(void);
void Func_020032f4(void);
void Func_02003364(void);
void Func_02003404(void);
void Func_020035b8(void);
void Func_020035d8(void);
void Func_020036d0(void);
void Func_0200371c(void);
void Func_020037d4(void);
void Func_020037e8(void);

/// Clears the followed actor and runs the entered room's setup, as overlay
/// 654's Func_02002ec4 does; Room7 has none.
s32 Func_02003094(void) {
  s16 scene;

  Func_020021d4();
  gData_0200480c = 0;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov661Room1)
    Func_0200316c();
  else if (scene == (s32)&SceneId_Ov661Room2)
    Func_020031b4();
  else if (scene == (s32)&SceneId_Ov661Room3)
    Func_020032f4();
  else if (scene == (s32)&SceneId_Ov661Room4)
    Func_02003364();
  else if (scene == (s32)&SceneId_Ov661Room5)
    Func_02003404();
  else if (scene == (s32)&SceneId_Ov661Room6)
    Func_020035b8();
  else if (scene == (s32)&SceneId_Ov661Room8)
    Func_020035d8();
  else if (scene == (s32)&SceneId_Ov661Room9)
    Func_020036d0();
  else if (scene == (s32)&SceneId_Ov661Room10)
    Func_0200371c();
  else if (scene == (s32)&SceneId_Ov661Room11)
    Func_020037d4();
  else if (scene == (s32)&SceneId_Ov661Room12)
    Func_020037e8();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_0200316c(void) {
  Func_02003820(gData_02004768);
  Func_02004180(0);
  if (gMainRoutineWork.entrance == 1) {
    GameFlag_SetBit(0xa29);
    GameFlag_ClearBit(0xa2a);
    GameFlag_ClearBit(0xa2b);
  }
}

/// Clears actors 8 to 14 after flag 0x8b3, or places them after flag 0x8f0.
void Func_020031b4(void) {
  s32 angleA;
  s32 angleB;
  s32 angle;

  if (GameFlag_Test(0x8b3)) {
    Func_02004090(8, 0, 0);
    Func_02004090(9, 0, 0);
    Func_02004090(10, 0, 0);
    Func_02004090(11, 0, 0);
    Func_02004090(12, 0, 0);
    Func_02004090(13, 0, 0);
    Func_02004090(14, 0, 0);
  } else if (GameFlag_Test(0x8f0)) {
    // The angles go through locals, as overlay 695 writes them (with
    // every angle a constant, differ 38; with the single-use ones
    // constants, they are built after the positions, differ 12).
    angleA = 0xb000;
    Scene_Call4098(8, 520 << 16, 242 << 16, angleA);
    angle = 0x8000;
    Scene_Call4098(9, 533 << 16, 271 << 16, angle);
    angle = 0x3000;
    Scene_Call4098(10, 435 << 16, 232 << 16, angle);
    angle = 0x5000;
    Scene_Call4098(11, 467 << 16, 248 << 16, angle);
    angleB = 0xd000;
    Scene_Call4098(12, 462 << 16, 340 << 16, angleB);
    Scene_Call4098(13, 482 << 16, 340 << 16, angleB);
    Scene_Call4098(14, 489 << 16, 320 << 16, angleA);
  }
  if (GameFlag_Test(0x88d))
    Func_02001594();
  Task_Add(Func_02002010, TASK_PASS_VBLANK);
}

void Func_020032c4(void) {
  s32 variant = Actor_GetById(gMainRoutineWork.currentOwner)->sprite->priority;

  Actor_SetSpritePriority(13, variant);
  Actor_SetSpritePriority(14, variant);
}

void Func_020032f4(void) {
  Func_02003820(gData_0200478e);
  if (GameFlag_Test(0x20b)) {
    Func_020006ac();
    wait(10);
  }
  Scene_TaskAdd(Func_02002010, 0x480);
  Scene_TaskAdd(Func_020032c4, 0x480);
}

void Func_02003338(s32 id) {
  Actor_GetById(id)->priorityFlags |= 2;
  Actor_SetSpritePriority(id, 3);
  Func_020040f8(id);
  Actor_SetModeById(id, 2);
}

void Func_02003364(void) {
  Func_02004180(0);
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 4) {
    if (Scene_FlagTest(0x889))
      Func_02004090(16, 0, 0);
    else
      Func_02002318();
  }
  Func_02003820(gData_020047d2);
  if (Scene_FlagTest(0x20b)) {
    Func_020006ac();
    wait(10);
  }
  Func_02003338(12);
  Func_02003338(13);
  Func_02003338(14);
  Func_02003338(15);
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2)
    Task_Add(Func_020020a8, TASK_PASS_VBLANK);
}

/// Room 5's setup: from entrances other than 1 and 2 it places actors 11 to
/// 20 and copies the map cells flags 0x88c and 0x8ed select.
void Func_02003404(void) {
  Func_02004180(0);
  Func_02003820(gData_020047ec);
  Func_02003338(11);
  Func_02003338(12);
  Func_02003338(13);
  Func_02003338(14);
  Func_02003338(15);
  Func_02003338(16);
  Func_02003338(17);
  Func_02003338(18);
  Func_02003338(19);
  Func_02003338(20);
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2) {
    if (GameFlag_Test(0x889) && !GameFlag_Test(0x88a))
      Func_020026a0();
  } else {
    Scene_Call4090(11, 200 << 16, 248 << 16);
    Scene_Call4090(12, 168 << 16, 280 << 16);
    Scene_Call4090(13, 152 << 16, 344 << 16);
    Scene_Call4090(14, 184 << 16, 360 << 16);
    Scene_Call4090(15, 328 << 16, 264 << 16);
    Scene_Call4090(16, 296 << 16, 296 << 16);
    Scene_Call4090(17, 344 << 16, 312 << 16);
    Scene_Call4090(18, 312 << 16, 360 << 16);
    Scene_Call4090(19, 280 << 16, 376 << 16);
    Func_02004090(20, 328 << 16, 392 << 16);
    if (GameFlag_Test(0x88c)) {
      if (GameFlag_Test(0x8ed)) {
        Func_02003ff0(68, 77, 13, 9, 3, 3);
        Scene_Call4000(68, 77, 3, 3, 13, 9);
      } else {
        Func_02003ff0(74, 71, 77, 9, 3, 3);
        Scene_Call4000(74, 71, 3, 3, 13, 9);
      }
      Scene_Call4000(63, 77, 3, 3, 13, 73);
    } else if (GameFlag_Test(0x88a)) {
      Func_02002b84();
    }
  }
}

/// A halfword of gMainRoutineWork that has no field name yet.
#define MAIN_ROUTINE_WORK_U16(offset) (*(u16 *)((u8 *)&gMainRoutineWork + (offset)))

void Func_020035b8(void) {
  Func_02004180(0);
  if (GameFlag_Test(0x302) == 0)
    Actor_SetWideSprite(64, 0);
}

static inline void Scene_StoreU16(u16 *field, s32 value) {
  /* FAKEMATCH(wrapper): through the inline the value is a word constant built
   * after the field's address (movs), where stored directly it loads from the
   * literal pool; as the halfword constant stored directly, 24 of 100
   * instructions of Func_020035d8 differ in tla-en. */
  *field = value;
}

/// Room 8's setup. Entered at entrance 4 (not in the Japanese edition), it
/// sets gMainRoutineWork's pair at 0x260 to the room whose flag is set.
void Func_020035d8(void) {
  struct Actor *object = (struct Actor *)Actor_GetById(64);
  s32 scene;

  Func_020041c8(137);
  Func_020041d0();
  Scene_Call41d8(0, 524, 8, 9);
  if (object != NULL)
    Func_02004090(64, 0, 0);
  object = (struct Actor *)Actor_GetById(10);
  object->scaleX = 0xb333;
  object->scaleY = 0xb333;
  object->radius = 8;
  Func_02002d40();
  Task_Add(Func_02001e58, TASK_PASS_VBLANK);
  if (gMainRoutineWork.entrance == 2) {
    GameFlag_ClearBit(0xa29);
    GameFlag_SetBit(0xa2a);
    GameFlag_ClearBit(0xa2b);
  }
#if TLA_PATCH_1
  else if (gMainRoutineWork.entrance == 4) {
    if (GameFlag_Test(0xa29)) {
      scene = (s32)&SceneId_Ov661Room1;
      MAIN_ROUTINE_WORK_U16(0x260) = scene;
      Scene_StoreU16(&MAIN_ROUTINE_WORK_U16(0x262), 1);
    } else if (GameFlag_Test(0xa2a)) {
      scene = (s32)&SceneId_Ov661Room8;
      MAIN_ROUTINE_WORK_U16(0x260) = scene;
      Scene_StoreU16(&MAIN_ROUTINE_WORK_U16(0x262), 2);
    } else if (GameFlag_Test(0xa2b)) {
      scene = (s32)&SceneId_Ov661Room9;
      MAIN_ROUTINE_WORK_U16(0x260) = scene;
      Scene_StoreU16(&MAIN_ROUTINE_WORK_U16(0x262), 2);
    }
  }
#endif
}

void Func_020036d0(void) {
  if (Actor_GetById(64) != 0)
    Func_02004090(64, 0, 0);
  if (gMainRoutineWork.entrance == 2) {
    GameFlag_ClearBit(0xa29);
    GameFlag_ClearBit(0xa2a);
    GameFlag_SetBit(0xa2b);
  }
}

void Func_02001bec(void);
void Func_02003da0(s32 unused, s32 value);
void Func_02003e3c(s32 a, s32 b, s32 c, s32 d, s32 e);

/// Room 10's setup.
void Func_0200371c(void) {
  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  GameFlag_SetBit(0x144);
  if (gMainRoutineWork.entrance == 1 && !GameFlag_Test(0x88c)) {
    Func_02002ebc();
  } else {
    if (gMainRoutineWork.entrance == 2 && !GameFlag_Test(0x88c))
      Func_02001bec();
  }
  if (GameFlag_Test(0x8ed)) {
    Func_02003ff0(35, 39, 13, 11, 3, 3);
    Scene_Call4090(9, 232 << 16, 200 << 16);
  }
  Func_02003da0(0, 240);
  Func_02003e3c(0, 9, 0, 0, 64);
  wait(10);
}

void Func_020037d4(void) {
  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
}

void Func_020037e8(void) {
  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Func_020041c8(137);
  Func_02003820(gData_02004800);
  Func_02002d54();
  Scene_TaskAdd(Func_02001e58, 0x480);
}

void Func_02003820(const void *script) {
  const u16 *entry;

  if (*(const u16 *)script == 0xffff)
    return;
  entry = script;
loop:
  {
    s32 id = entry[0];
    struct FieldActor *object = Actor_GetById(id);
    s32 kind = (s16)entry[1];
    s32 flag = (s16)entry[2];

    object->collisionFlags &= 0xfe;
    Actor_SetSingleChildFlags(object, 0);
    Func_02003910(id, (u16)kind, (u16)flag, object->terrainId);
    entry += 3;
  }
  if (entry[0] != 0xffff)
    goto loop;
}
