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
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

struct DoorPlan {
  s32 value[6];
};

extern u8 gData_02004d0c[];
extern u8 gData_02004dbc[];
extern u8 gData_02004fe4[];
extern u8 gData_02005290[];

s32 Actor_SetSingleChildFlags();
void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void Func_02000fa4(void);
void Func_0200112c(void);
void Func_020017ec(void);
void Func_02001cb0(void);
void Func_02003f48(s32, s32, s32, s32);
void Func_02004004(void);
void Func_020040c0(void);
s32 Func_02004240(struct DoorPlan *);
void Func_020044c4(struct DoorPlan);
void Func_02004868(void *, s32, s32);
s32 Func_02004a0c();
s32 Func_02004a24();
s32 Func_02004a7c();
s32 Func_02004a84();
void Func_02004a94();
void Func_02004a9c();
s32 Func_02004bb4();
s32 Func_02004c24();
s32 Func_02004c44();
s32 Func_02004c4c();
s32 Func_02004c8c();

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

