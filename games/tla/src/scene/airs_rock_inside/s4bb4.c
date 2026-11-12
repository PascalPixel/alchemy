/// @file
/// Scene code of overlay 0x664, in ROM order, from s4bb4's offset up to the next object the overlay links.

#include "airs_rock_inside.h"

extern u8 MsgCheckedThePsynergy[];
extern s32 gData_02006c10[];
extern s32 gData_02006d6c[];
extern s32 gData_02006e2c[];
extern s32 gData_02006eec[];
extern s32 gData_02006fa0[];
extern s32 gData_0200706c[];
extern s32 gData_020070fc[];
extern s32 gData_02007228[];
extern s32 gData_02007318[];
extern s32 gData_020073a8[];

void Func_02004bb4(void) {
  const u8 *message;

  Func_02005e14();
  Func_02005f2c(0);
  message = MsgCheckedThePsynergy;
  FieldMessage_Show(message, 1);
  Func_02005f94(126);
  Func_02005f34(999, 0);
  FieldEvent_Wait(10);
  GameFlag_ClearBit(322);
  FieldMessage_Show(message + 1, 1);
  Func_02005e1c();
}

/// The placement table for the current room.
s32 *Func_02004c00(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov664Room1)
    return gData_02006c10;
  if (scene == (s32)&SceneId_Ov664Room2)
    return gData_02006d6c;
  if (scene == (s32)&SceneId_Ov664Room3)
    return gData_02006e2c;
  if (scene == (s32)&SceneId_Ov664Room4)
    return gData_02006eec;
  if (scene == (s32)&SceneId_Ov664Room5)
    return gData_02006fa0;
  if (scene == (s32)&SceneId_Ov664Room6)
    return gData_0200706c;
  if (scene == (s32)&SceneId_Ov664Room7)
    return gData_020070fc;
  if (scene == (s32)&SceneId_Ov664Room8)
    return gData_02007228;
  if (scene == (s32)&SceneId_Ov664Room9)
    return gData_02007318;
  if (scene == (s32)&SceneId_Ov664Room10)
    return gData_020073a8;
  return gData_02006d6c;
}

#include "version.h"

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"

struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_02005d8c(struct Actor *object, s32 x, s32 y, s32 z);
s32 Func_02005dec(struct Actor *object);
void wait(s32 frames);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void Func_02005d94(struct Actor *object);
void Func_02005d7c(struct Actor *object);

void Func_02004cc8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *prepared, *lift;
#if TLA_PATCH_1
  u8 priorityFlags;
#endif
  s32 height;

  Func_02005e14();
  Func_02005f2c(0);
#if TLA_PATCH_1
  priorityFlags = leader->priorityFlags;
#endif
  prepared = OverlayObject_PrepareObject(0xd80000, -0x200000, 0x2200000, 286);
  leader->motionFlags = 0;
  Actor_SetSingleChildFlags(leader, 0);
  leader->speedLimit = 0x18000;
  leader->acceleration = 0x18000;
  lift = Import_ActorCreate(280, leader->x, leader->y - 0x80000, leader->z);
  Func_02005d5c(lift, 2);
  Actor_SetSingleChildFlags(lift, 0);
  lift->motionFlags = 0;
  lift->speedLimit = 0x16666;
  lift->acceleration = 0x16666;
  Func_02005d8c(leader, leader->x, leader->y + 0x500000, leader->z);
  Func_02005d8c(lift, lift->x, lift->y + 0x480000, lift->z);
  Func_02005f94(137);
  Event_StartScreenTransition();
  while (!Func_02005dec(leader)) {
    leader->facing = (u8)gFrameCount << 12;
    leader->priorityFlags ^= 2 << (gFrameCount & 1);
    wait(1);
  }
  Func_02005f94(288);
  FieldEvent_Wait(10);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 0);
  height = Map_GetTerrainHeight(0, lift->x, lift->z - 0x100000);
  leader->y = height;
  leader->terrainHeight = height;
  Func_02005d5c(leader, 6);
  wait(6);
  Func_02005f94(152);
  Func_02005d5c(leader, 7);
  leader->speedLimit = 0x20000;
  leader->acceleration = 0x8000;
  leader->velocityY = 0x40000;
  leader->motionFlags = 2;
  leader->priorityFlags = 1;
  Func_02005d8c(leader, leader->x, leader->y, leader->z - 0x100000);
  OverlayObject_WaitUntilIdle(leader);
  Func_02005d94(leader);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Func_02005d5c(lift, 5);
  wait(12);
  Func_02005d7c(lift);
  leader->motionFlags = 3;
  leader->terrainHeight = leader->y;
  Actor_SetSingleChildFlags(leader, 1);
#if TLA_PATCH_1
  leader->priorityFlags = priorityFlags;
#endif
  Func_02005d7c(prepared);
  wait(1);
  Event_WaitTransitionFrames();
  Func_02005e1c();
}

#include "field_sprite.h"

void Func_02000550(void);

void Func_02004eb0(void) {
  struct Actor *object;
  s32 i = 0;
  struct FieldSprite *sprite;

do {
  object = Import_ActorCreate(672, 0, 0, 0);
  sprite = object->sprite;
  sprite->priority = 1;
  ((u8 *)object)[0x63] = i << 4;
  { s32 zero = 0;
  object->motionFlags = zero;
  object->priorityFlags = zero; }
  object->acceleration = object->speedLimit = (i << 12) + 0x8000;
  object->update = Func_02000550;
  Actor_SetSingleChildFlags(object, 0);
  Actor_SetPartAttribute(object, 2);
  if (i & 1) {
    object->timer = 212;
    object->unk66 = 0;
  } else {
    object->timer = 116;
    object->unk66 = 0;
  }
  Animation_SetStateFlags(object, 1);
  i++;
} while (i != 8);
}

extern s32 gData_020073d8;

void Func_02004f3c(void) {
  struct Actor *actor = Actor_GetById(Func_02005f84());
  s32 level;

  actor->priorityFlags |= 32;
  if (actor->y + 0x200000 >= 0x1380000)
    level = 2;
  else if (actor->y + 0x200000 >= 0xd80000)
    level = 1;
  else
    level = 0;
  if (gData_020073d8 == level)
    return;
  switch (level) {
  case 2: {
    s32 a4 = 7, a5 = 39;
    Func_02005dac(64, 64, 6, 4, a4, a5);
  }
    Func_02005dac(70, 64, 6, 4, 26, 39);
    break;
  case 1: {
    s32 a4 = 7, a5 = 39;
    Func_02005dac(64, 68, 6, 4, a4, a5);
  }
    Func_02005dac(70, 68, 6, 4, 26, 39);
    break;
  case 0: {
    s32 a4 = 7, a5 = 39;
    Func_02005dac(64, 72, 6, 4, a4, a5);
  }
    Func_02005dac(70, 68, 6, 4, 26, 39);
    break;
  }
  gData_020073d8 = level;
}
