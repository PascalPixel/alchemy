/// @file
/// The scene code of overlay 0x668 that its scene tables belong to, in ROM order; the rest of the overlay is in eastern_alhafra/.

#define OWN_Actor_SetSingleChildFlags

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern u8 gData_02004d0c[];
extern u8 gData_02004dbc[];
extern u8 gData_02004fe4[];

void Func_02000fa4(void);
void Func_02003f48(s32, s32, s32, s32);
void Func_02004004(void);
void Func_020040c0(void);
void Func_02004868(void *, s32, s32);
s32 Func_02004a7c();
s32 Func_02004c24();
s32 Func_02004c8c();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

void *Func_0200004c(void) {
  if (Event_GetFlag(0x8ab))
    return gData_02004fe4;
  return gData_02004dbc;
}

void Func_0200006c(s32 mode, s32 id) {
  if (id == 0)
    Func_02004004();
  if (mode == 1) {
    Event_SetFlag(0x8a7);
    Func_02000fa4();
  }
  if (mode == 2)
    Func_02003f48(0x1f20000, 0x100000, 0x19c0000, 2);
  if (mode == 3)
    Func_02003f48(0x1f20000, 0x100000, 0x19c0000, 30);
  if (id == 0x3e7)
    Func_020040c0();
}

void Func_020000c8(s32 mode, s32 arg) {
  struct Actor *a;
  struct Actor *b;
  s32 i;

  Func_02004868(gData_02004d0c, mode, arg);
  if (mode == 1) {
    a = Actor_Get(19);
    b = Actor_Get(21);
    a->speedLimit = 0x40000;
    a->acceleration = 0x8000;
    b->acceleration = 0x8000;
    b->speedLimit = 0x40000;
    FieldActor_SetPositionAndResetMotion(a, 0x1e80000, a->y, 0x1380000);
    FieldActor_SetMoveTarget(a, 0x1b80000, 0, 0x1380000);
    FieldActor_SetPositionAndResetMotion(b, 0x1e80000, b->y, 0x1380000);
    FieldActor_SetMoveTarget(b, 0x1b80000, 0, 0x1780000);
    Func_02004c8c(1);
    Func_02004c8c(2);
    i = 0;
    do {
      Func_02004c24(1, 18, 19);
      Func_02004c24(2, 20, 21);
      System_Wait(1);
      Func_02004c8c(1);
      Func_02004c8c(2);
      if (Func_02004a7c(a) != 0 && Func_02004a7c(b) != 0)
        break;
      i++;
    } while (i < 60);
  }
}

void Func_02000190(s32 unused, s32 id) {
  struct Actor *leader;
  struct Actor *object;
  s32 leaderX;
  s32 x;
  s32 z;

  leader = Actor_Get(gMainRoutineWork.currentOwner);
  object = Actor_Get(id);
  object->motionFlags = 0;
  if (id == 10) {
    leaderX = leader->x >> 20;
    x = object->x >> 20;
    z = object->z >> 20;
    if (x == 34 && z == 32 && leaderX == 33)
      MapGrid_SetCellBehavior(0x2100000, 0x2000000, 2, 255);
    if (x == 33 && z == 32 && leaderX == 32)
      MapGrid_SetCellBehavior(0x2000000, 0x2000000, 2, 255);
  }
}

#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"

struct DoorPlan {
  s32 value[6];
};

extern u8 gData_02005290[];

s32 Actor_SetSingleChildFlags();
void Func_0200112c(void);
void Func_020017ec(void);
void Func_02001cb0(void);
s32 Func_02004240(struct DoorPlan *);
void Func_020044c4(struct DoorPlan);
s32 Func_02004a0c();
s32 Func_02004a24();
s32 Func_02004a84();
void Func_02004a94();
void Func_02004a9c();
s32 Func_02004bb4();
s32 Func_02004c44();
s32 Func_02004c4c();

void Func_02000200(s32 mode, s32 id) {
  struct Actor *object;
  struct Actor *sparks[4];
  struct Actor *spark;
  s32 x;
  s32 z;
  s32 hit;
  s32 i;

  object = Actor_Get(id);
  x = object->x >> 20;
  hit = 0;
  z = object->z >> 20;
  if (id == 10) {
    MapGrid_SetCellBehavior(0x2100000, 0x2000000, 2, 0);
    MapGrid_SetCellBehavior(0x2000000, 0x2000000, 2, 0);
  }
  if (x == 31 && z == 32) {
    Event_SetFlag(0x8a0);
    hit = 1;
  }
  if (x == 25 && z == 33) {
    Event_SetFlag(0x8a1);
    hit = 1;
  }
  if (x == 21 && z == 33) {
    Event_SetFlag(0x8a4);
    hit = 1;
  }
  if (x == 27 && z == 33) {
    Event_SetFlag(0x8a5);
    hit = 1;
  }
  if (hit) {
    if (id != 10)
      Event_PlaySound(310);
    object->gravity = 0x170a;
    object->motionFlags = 3;
    Actor_SetSingleChildFlags(Actor_Get(id), 1);
    System_Wait(5);
    for (i = 0; i < 60 && object->y > 0; i++)
      System_Wait(1);
    Event_PlaySound(298);
    System_Wait(1);
    Event_PlaySound(240);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    Func_02004a0c(object, 4);
    i = 0;
  create:
    spark = (FieldActor_Create(276, object->x, object->y, object->z));
    sparks[i] = spark;
    spark->scaleX = 0xa666;
    Func_02004a0c(spark, 1);
    Actor_SetSingleChildFlags(spark, 0);
    Event_PlaySound(154);
    System_Wait(5);
    i++;
    if (i <= 2)
      goto create;
    Func_02000fa4();
    FieldActor_Stop(gMainRoutineWork.currentOwner);
    System_Wait(30);
    for (i = 0; i <= 2; i++)
      Func_02004a24(sparks[i]);
  }
}

#include "actor.h"

void Func_02000394(s32 unused, s32 id) {
  Func_02000190(unused, id);
  MapGrid_SetCellBehavior(0x1400000, 0x1d00000, 2, 255);
  MapGrid_SetCellBehavior(0x1400000, 0x1e00000, 2, 255);
  MapGrid_SetCellBehavior(0x1400000, 0x1f00000, 2, 255);
  MapGrid_SetCellBehavior(0x1400000, 0x2000000, 2, 255);
  MapGrid_SetCellBehavior(0x1600000, 0x1d00000, 2, 255);
  MapGrid_SetCellBehavior(0x1600000, 0x1e00000, 2, 255);
  MapGrid_SetCellBehavior(0x1600000, 0x1f00000, 2, 255);
  MapGrid_SetCellBehavior(0x1600000, 0x2000000, 2, 255);
  MapGrid_SetCellBehavior(0x1500000, 0x2100000, 2, 0);
}

void Func_0200042c(s32 mode, s32 id) {
  struct Actor *object;
  s32 x;
  s32 z;
  s32 open;

  object = Actor_Get(id);
  x = object->x >> 20;
  z = object->z >> 20;
  open = 0;
  if (x == 21 && z == 33)
    open = 1;
  MapGrid_SetCellBehavior(0x1400000, 0x1d00000, 2, 0);
  MapGrid_SetCellBehavior(0x1400000, 0x1e00000, 2, 0);
  MapGrid_SetCellBehavior(0x1400000, 0x1f00000, 2, 0);
  MapGrid_SetCellBehavior(0x1400000, 0x2000000, 2, 0);
  MapGrid_SetCellBehavior(0x1600000, 0x1d00000, 2, 0);
  MapGrid_SetCellBehavior(0x1600000, 0x1e00000, 2, 0);
  MapGrid_SetCellBehavior(0x1600000, 0x1f00000, 2, 0);
  MapGrid_SetCellBehavior(0x1600000, 0x2000000, 2, 0);
  MapGrid_SetCellBehavior(0x1500000, 0x2100000, 2, 255);
  if (open)
    FieldCamera_MoveTo(PIXELS(336), -1, PIXELS(560), 1);
  Func_02000200(mode, id);
  if (open)
    FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
}
