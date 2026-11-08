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
struct Actor *Actor_GetById();
s32 Actor_SetSingleChildFlags();
void Actor_Stop();
void Actor_OffsetPositionAndResetMotion();
void Actor_SetAngleToward();
void Actor_SetSpeedParameters();
void Actor_ArmCallback();
void Actor_SetPositionAndReset();
void Actor_WaitForMoveById();
void Motion_SetVarCbAndRefresh();
void FieldEvent_Wait();
s32 FieldMessage_ShowAtActor();
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
struct Actor *Func_02004a1c();
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
  struct Actor *object;
  struct Actor *sparks[4];
  struct Actor *spark;
  s32 x;
  s32 z;
  s32 hit;
  s32 i;

  object = Actor_GetById(id);
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
    GameFlag_SetBit(SPLIT_CONSTANT(0x8a1));
    hit = 1;
  }
  if (x == 21 && z == 33) {
    GameFlag_SetBit(SPLIT_CONSTANT(0x8a4));
    hit = 1;
  }
  if (x == 27 && z == 33) {
    GameFlag_SetBit(SPLIT_CONSTANT(0x8a5));
    hit = 1;
  }
  if (hit) {
    if (id != 10)
      Func_02004c9c(310);
    object->gravity = SPLIT_CONSTANT(0x170a);
    object->motionFlags = 3;
    Actor_SetSingleChildFlags(Actor_GetById(id), 1);
    wait(5);
    for (i = 0; i < 60 && object->y > 0; i++)
      wait(1);
    Func_02004c9c(298);
    wait(1);
    Func_02004c9c(240);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    Func_02004a0c(object, 4);
    i = 0;
  create:
    spark = Func_02004a1c(276, object->x, object->y, object->z);
    sparks[i] = spark;
    spark->scaleX = SPLIT_CONSTANT(0xa666);
    Func_02004a0c(spark, 1);
    Actor_SetSingleChildFlags(spark, 0);
    Func_02004c9c(154);
    wait(5);
    i++;
    if (i <= 2)
      goto create;
    Func_02000fa4();
    Actor_Stop(gMainRoutineWork.currentOwner);
    wait(30);
    for (i = 0; i <= 2; i++)
      Func_02004a24(sparks[i]);
  }
}

