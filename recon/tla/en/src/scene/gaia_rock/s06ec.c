#include "version.h"
#include "iwram_call.h"
#include "call.h"
#include "flag_call.h"
#include "actor.h"
#include "field/event_work.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#if !TLA_PATCH_1
#define SCREEN_EFFECT 0x201
#else
#define SCREEN_EFFECT 0x204
#endif

extern char SceneId_Ov680Room1;
extern char SceneId_Ov680Room2;
extern char SceneId_Ov680Room3;
extern u8 Func_02000670[];
s32 Func_02000f0c();
struct Actor *Actor_GetById();
s32 Task_Add();
s32 Func_02000f14();
void Actor_SetSingleChildFlags();
void Func_02000e44();
void Func_02000e8c();
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Actor_StartRepeatedMotion();
void Func_02000ed4();
void FieldEvent_Wait();
void Func_02000ea4();
void Func_02000eac();
void Func_02000e7c();
void Func_02000dd0();
void Func_02000b2c();

s32 Func_020006ec(void) {
  struct Actor *actor;
  struct EventWork *work;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = SCREEN_EFFECT;
  gMainRoutineWork.retreatScene = (s32)&SceneId_Ov680Room1;
  gMainRoutineWork.retreatEntrance = 5;
  Actor_GetById(Func_02000f0c())->priorityFlags |= 32;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room1) {
    Task_Add((s32)Func_02000670, 0x480);
    Call2((void (*)())Func_02000f14, 8, 1);
    Call2((void (*)())Func_02000f14, 9, 1);
    Call2((void (*)())Func_02000f14, 10, 1);
    Call2((void (*)())Func_02000f14, 11, 1);
    Call2((void (*)())Func_02000f14, 12, 1);
    Call2((void (*)())Func_02000f14, 13, 1);
    Call2((void (*)())Func_02000f14, 14, 1);
    Func_02000f14(15, 1);
    if (GameFlag_Test(0x920)) {
      actor = Actor_GetById(16);
      actor->motionFlags = 0;
      actor->x = 0x580000;
      actor->y = 0x800000;
      Actor_SetSingleChildFlags(actor, 0);
      Call6((void (*)())Func_02000e44, 6, 27, 1, 1, 5, 27);
    }
    actor = Actor_GetById(19);
    actor->priorityFlags = 0;
    if (GameFlag_Test(SPLIT_CONSTANT(0x921))) {
      actor->motionFlags = 0;
      actor->x = 0x2880000;
      actor->y = 0x400000;
      Actor_SetSingleChildFlags(actor, 0);
      Call6((void (*)())Func_02000e44, 33, 102, 1, 2, 37, 102);
      Call6((void (*)())Func_02000e44, 27, 44, 1, 1, 40, 46);
    } else if (GameFlag_Test(0x109)) {
      if ((actor->x >> 19) != 75) {
        Call6((void (*)())Func_02000e44, 33, 102, 1, 2, 37, 102);
      }
      if ((actor->x >> 19) == 79) {
        actor->motionFlags = 0;
        Call6((void (*)())Func_02000e44, 38, 46, 1, 1, 39, 46);
      }
    }
    if (GameFlag_Test(SPLIT_CONSTANT(0x927)) && !GameFlag_Test(SPLIT_CONSTANT(0x9ab))) {
      Call3((void (*)())Func_02000e8c, 20, 0x31e0000, 0x3200000);
      Func_02000e44(53, 52, 1, 1, 49, 49);
      Func_02000e44(53, 52, 1, 1, 50, 49);
      Actor_SetModeById(20, 5);
      if (!Flag_IsSet(SPLIT_CONSTANT(0xa61))) {
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        Actor_StartRepeatedMotion(20, 3);
        Func_02000ed4(20, 1);
        FieldEvent_Wait(30);
        Func_02000ea4((s32)MsgIMNot);
        Func_02000eac(20, 0);
        Actor_StartRepeatedMotion(20, 2);
        FieldEvent_Wait(30);
        GameFlag_SetBit(SPLIT_CONSTANT(0xa61));
        Func_02000e7c();
      }
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room2) {
    Call2((void (*)())Func_02000f14, 8, 1);
    Call2((void (*)())Func_02000f14, 9, 1);
    Call2((void (*)())Func_02000f14, 10, 1);
    Call2((void (*)())Func_02000f14, 11, 1);
    Call2((void (*)())Func_02000f14, 12, 1);
    Call2((void (*)())Func_02000f14, 13, 1);
    Func_02000f14(14, 1);
    if ((u16)(gMainRoutineWork.entrance - 3) <= 1) {
      Func_02000dd0();
    }
    if (GameFlag_Test(0x300)) {
      Func_02000b2c(17, 0x2fc0000, -0x1870000, 0x1a80000);
    }
    if (GameFlag_Test(0x301)) {
      Func_02000b2c(18, 0x3150000, -0xf80000, 0x1a80000);
    }
    if (GameFlag_Test(0x302)) {
      Func_02000b2c(19, 0x30a0000, -0x770000, 0x1a80000);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room3) {
    actor = Actor_GetById(64);
    if (actor != 0) {
      Func_02000e8c(64, 0, 0);
    }
    work = Ram_HeapSlots->eventWork;
    actor = work->objects[16];
    work->objects[64] = actor;
    actor = Actor_GetById(8);
    actor->motionFlags = 0;
    Call6((void (*)())Func_02000e44, 11, 14, 1, 1, 13, actor->z >> 20);
  }
  return 0;
}
