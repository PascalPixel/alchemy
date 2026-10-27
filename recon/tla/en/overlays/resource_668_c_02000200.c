
// Draft of resource_668 Func_02000200 and Func_02000514 (English 0x02008200 and
// 0x02008514), listings between scene_tables.c, events2.c and events.c.

// Func_02000200 differs by 4 instructions: native sets r8 = sp (the address of
// sparks) inside the creation loop, after the create call; GCSE's PRE hoists it
// to the loop's entry here (for and do-while loops are strength-reduced
// instead). Func_02000514 differs by about 70: native computes &plan.value[4]
// (sp + 24) into r8 once, before the facing test, and saves sl; here each call
// site adds sp + 24 and only r8 is saved. Its wait loop needs the while form
// below.

#define OWN_ObjectDispatch_SetSingleChildFlags
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_scene.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

struct DoorPlan {
  s32 value[6];
};

extern u8 gData_02004d0c[];
extern u8 gData_02004dbc[];
extern u8 gData_02004fe4[];
extern u8 gData_02005290[];

s32 GameFlag_Test();
void GameFlag_SetBit();
struct ObjectRuntime *Object_GetById();
s32 ObjectDispatch_SetSingleChildFlags();
void ObjectMotion_EnableActionAndResetMotion();
void ObjectMotion_OffsetPositionAndResetMotion();
void ObjectMotion_SetAngleToward();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_ArmCallback();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_CommitCurrentPositionAndActivate();
void Motion_SetVarCbAndRefresh();
void FieldEvent_WaitTicks();
s32 UiText_OpenMessageAtObject();
s32 Event_AskYesNoWithActorAnimation();
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
struct ObjectRuntime *Func_02004a1c();
s32 Func_02004a24();
s32 Func_02004a34();
s32 Func_02004a44();
s32 Func_02004a7c();
s32 Func_02004a84();
void Func_02004a94();
void Func_02004a9c();
s32 Func_02004b04();
s32 Func_02004b3c();
s32 Func_02004b4c();
s32 Func_02004b5c();
s32 Func_02004b6c();
s32 Func_02004bac();
s32 Func_02004bb4();
s32 Func_02004be4();
s32 Func_02004bec();
s32 Func_02004c24();
s32 Func_02004c44();
s32 Func_02004c4c();
void Func_02004c9c(s32);
s32 Func_02004c7c();
s32 Func_02004c8c();

void Func_02000200(s32 mode, s32 id) {
  struct ObjectRuntime *object;
  struct ObjectRuntime *sparks[4];
  struct ObjectRuntime *spark;
  s32 x;
  s32 z;
  s32 hit;
  s32 i;

  object = Object_GetById(id);
  x = object->x >> 20;
  hit = 0;
  z = object->z >> 20;
  if (id == 10) {
    Value4(Func_02004c7c, 0x2100000, 0x2000000, 2, 0);
    Value4(Func_02004c7c, 0x2000000, 0x2000000, 2, 0);
  }
  if (x == 31 && z == 32) {
    GameFlag_SetBit(0x8a0);
    hit = 1;
  }
  if (x == 25 && z == 33) {
    GameFlag_SetBit(0x8a1);
    hit = 1;
  }
  if (x == 21 && z == 33) {
    GameFlag_SetBit(0x8a4);
    hit = 1;
  }
  if (x == 27 && z == 33) {
    GameFlag_SetBit(0x8a5);
    hit = 1;
  }
  if (hit) {
    if (id != 10)
      Func_02004c9c(310);
    object->gravity = 0x170a;
    object->motionFlags = 3;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(id), 1);
    WaitTicks(5);
    for (i = 0; i < 60 && object->y > 0; i++)
      WaitTicks(1);
    Func_02004c9c(298);
    WaitTicks(1);
    Func_02004c9c(240);
    object->collisionFlags = 0;
    /* FAKEMATCH: ⚓️ stores the cleared flags before it addresses the priority
     * byte. */
    __asm__ volatile("");
    object->priorityFlags |= 2;
    Func_02004a0c(object, 4);
    i = 0;
  create:
    spark = Func_02004a1c(276, object->x, object->y, object->z);
    sparks[i] = spark;
    spark->scaleX = 0xa666;
    Func_02004a0c(spark, 1);
    ObjectDispatch_SetSingleChildFlags(spark, 0);
    Func_02004c9c(154);
    i++;
    WaitTicks(5);
    if (i <= 2)
      goto create;
    Func_02000fa4();
    ObjectMotion_EnableActionAndResetMotion(gPartyState.currentOwner);
    WaitTicks(30);
    for (i = 0; i <= 2; i++)
      Func_02004a24(sparks[i]);
  }
}

void Func_02000514(void) {
  struct ObjectRuntime *door;
  struct ObjectRuntime *block;
  struct DoorPlan plan;
  s32 facing;
  s32 i;

  door = Object_GetById(15);
  block = Object_GetById(16);
  facing = (Object_GetById(gPartyState.currentOwner)->angle + 0x2000) & 0xc000;
  if (block != NULL && Value1(GameFlag_Test, 0x8a4) == 0)
    Func_02004c7c(block->x, block->z, 2, 255);
  if (Func_02004240(&plan) != 0) {
    if (Value1(GameFlag_Test, 0x8a4) == 0 || facing == 0xc000) {
      Func_020044c4(plan);
    } else {
      GameFlag_SetBit(0x8a6);
      Func_02004a94();
      Func_02004be4(0);
      Camera_MoveWithinMapBounds(0x1500000, -1, 0x2300000, 1);
      Func_020044c4(plan);
      Func_02004c7c(door->x - 0x180000, door->z, 0, 0);
      Func_02004c7c(door->x + 0x80000, door->z, 0, 0);
      Func_02004c7c(door->x - 0x180000, door->z, 2, 0);
      Func_02004c7c(door->x - 0x80000, door->z, 2, 0);
      Func_02004c7c(door->x + 0x80000, door->z, 2, 0);
      Func_02004c7c(door->x + 0x180000, door->z, 2, 0);
      Object_SetModeById(15, 3);
      ObjectMotion_OffsetPositionAndResetMotion(15, 0, 16);
      FieldEvent_WaitTicks(20);
      door->gravity = 0xccc;
      door->motionFlags = 3;
      i = 0;
      while (door->y > 0) {
        i++;
        WaitTicks(1);
        if (i > 59)
          break;
      }
      Func_02000fa4();
      Func_02004c9c(240);
      Object_SetModeById(15, 8);
      FieldEvent_WaitTicks(60);
      Camera_FollowActor(gPartyState.currentOwner, 1);
      Func_02004a9c();
    }
  }
  if (block != NULL && Value1(GameFlag_Test, 0x8a4) == 0)
    Func_02004c7c(block->x, block->z, 2, 0);
}
