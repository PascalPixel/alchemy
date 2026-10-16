// Near miss, differ 6 of 409 (2 real), written from the listing in the
// style of idejima's other scenes. Left: the native adds 100 to actor 5's
// pointer (for the action wait at the end) right after the leader's
// scaleX store; here sched2 sinks the add to just before the action
// store, so three pool offsets move with it. The add is ready from the
// start with priority 3 and never wins a cycle: in the native the load of
// the owner after the store must have stalled a cycle. Tried 2026-10-15:
// the action pointer taken at the native's place, in its own variable
// (differ 25) or in the actor's (25, and the store's zero is pooled).
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "call.h"
#include "text/msg_ids.h"

extern const u8 gData_020033a8[];
extern const u8 gData_02003400[];
extern const u8 gData_0200346c[];
extern const u8 gData_020034e4[];
extern const u8 gData_0200350c[];

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Battle_WaitMode0();
void Motion_CamBounds();
void Engine_CameraSetSpeed();
s32 Engine_ActorShowEmote();
void Engine_ActorSetAttachedEffect();
void Event_StartScreenTransition();
void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_EnableActionAndSetCallback();
void Object_SetActionCallbackAndRefreshById();
void ObjectMotion_Launch();
void ObjectMotion_ArmCallback();
void Func_0200161c(void);
void DriftScene_Prepare(void);
void Func_02003180();
void Func_02003228();
void Func_02003298();
void Func_020032b8();
void Func_020032e8();
void Func_02003310();

void Func_02001ee0(void) {
  struct ObjectRuntime *actor = Object_GetById(5);

  Engine_EventBegin();
  Call1((void (*)())Func_02003310, 0);
  Func_0200161c();
  DriftScene_Prepare();
  Call4((void (*)())Motion_CamBounds, -1, -1, -1, 0);
  Call3((void (*)())Func_02003228, 6, 0x1bc0000, 0xd40000);
  Call2((void (*)())Engine_ActorSetAnimation, 6, 19);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(6), 0);
  Call3((void (*)())Func_02003228, gPartyState.currentOwner, 0x1b00000,
        0xea0000);
  Object_GetById(gPartyState.currentOwner)->scaleX = 0xffff0000;
  Call2((void (*)())Engine_ActorSetAnimation, gPartyState.currentOwner, 41);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  Call3((void (*)())Func_02003228, 10, 0x1880000, 0xac0000);
  Task_Wait(1);
  Call4((void (*)())Motion_CamBounds, 0xc00000, -1, 0xd60000, 0);
  Func_02003180();
  Task_Wait(1);
  Call3((void (*)())Func_02003228, 5, 0xd00000, 0xda0000);
  Call2((void (*)())Engine_ActorSetAnimation, 5, 19);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(5), 0);
  Call3((void (*)())Func_02003228, 8, 0xb00000, 0xcc0000);
  Call2((void (*)())Engine_ActorSetAnimation, 8, 5);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
  Task_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call1((void (*)())Battle_WaitMode0, 120);
  Call1((void (*)())Engine_EventSetMessage, (s32)MsgIdejimaHowLongWillWeDrift);
  Call3((void (*)())Func_02003298, 5, 0, 40);
  Call2((void (*)())Engine_EventShowMessage, 8, 0);
  Value3((s32 (*)())Engine_ActorShowEmote, 5, 263, 0);
  Call2((void (*)())Engine_EventShowMessage, 5, 0);
  Value3((s32 (*)())Engine_ActorShowEmote, 8, 261, 80);
  Call2((void (*)())Engine_EventShowMessage, 8, 0);
  Call2((void (*)())Engine_EventShowMessage, 5, 0);
  Value3((s32 (*)())Engine_ActorShowEmote, 8, 261, 40);
  Value3((s32 (*)())Engine_ActorShowEmote, 5, 265, 80);
  Call2((void (*)())Engine_EventShowMessage, 5, 0);
  Call2((void (*)())Engine_EventShowMessage, 8, 0);
  Value3((s32 (*)())Engine_ActorShowEmote, 5, 258, 0);
  Call2((void (*)())Engine_EventShowMessage, 5, 0);
  Call2((void (*)())Engine_ActorSetAttachedEffect, 8, 258);
  Call1((void (*)())Battle_WaitMode0, 40);
  Call2((void (*)())Engine_EventShowMessage, 8, 0);
  Value3((s32 (*)())Engine_ActorShowEmote, 5, 261, 40);
  Call3((void (*)())Func_02003298, 8, 0, 40);
  Value3((s32 (*)())Engine_ActorShowEmote, 5, 265, 20);
  Call3((void (*)())Func_02003298, 5, 0, 20);
  Call3((void (*)())Func_02003298, 8, 0, 40);
  Value3((s32 (*)())Engine_ActorShowEmote, 5, 261, 20);
  Call2((void (*)())Engine_EventShowMessage, 5, 0);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 10, 0xcccc, 0x6666);
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 10, 304, 172);
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 10, 224, 172);
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 10, 208, 192);
  Call2((void (*)())Engine_EventShowMessage, 10, 0);
  Value3((s32 (*)())Engine_ActorShowEmote, 8, 257, 0);
  Value3((s32 (*)())Engine_ActorShowEmote, 5, 257, 80);
  Call2((void (*)())Engine_EventShowMessage, 10, 0);
  Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 10,
        gData_020033a8);
  Call1((void (*)())Battle_WaitMode0, 40);
  Call2((void (*)())Engine_ActorSetAnimation, 8, 1);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 1);
  Call3((void (*)())ObjectMotion_Launch, 8, 4, 20);
  Call2((void (*)())Engine_ActorSetAnimation, 5, 1);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(5), 1);
  Call3((void (*)())ObjectMotion_Launch, 5, 4, 20);
  Call3((void (*)())ObjectMotion_ArmCallback, 8, 0x3000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 5, 0xa000, 40);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 8, 0xcccc, 0x6666);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 5, 0xcccc, 0x6666);
  Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 8, gData_02003400);
  Call1((void (*)())Battle_WaitMode0, 40);
  Call2((void (*)())Engine_CameraSetSpeed, 0xcccc, 0x1999);
  Call4((void (*)())Motion_CamBounds, 0x1700000, -1, 0xd20000, 1);
  actor->action = 0;
  Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 5, gData_0200346c);
  do {
    Task_Wait(1);
  } while (actor->action == 0);
  Call1((void (*)())Battle_WaitMode0, 10);
  Call2((void (*)())Engine_ActorSetAttachedEffect, 8, 258);
  Call2((void (*)())Engine_ActorSetAttachedEffect, 5, 258);
  Call11((void (*)())Func_020032b8, 5, 7, 13, 2, 12, 8, 9, 2, 4, 1, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 10, 0x3000, 0);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 8, 0x19999, 0xcccc);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 5, 0x19999, 0xcccc);
  Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 8, gData_020034e4);
  Call2((void (*)())Object_SetActionCallbackAndRefreshById, 5, gData_0200350c);
  Call1((void (*)())Battle_WaitMode0, 40);
  Call2((void (*)())Engine_EventShowMessage, 0x5008, 0);
  Call3((void (*)())ObjectMotion_Launch, 5, 4, 10);
  Call2((void (*)())Engine_EventShowMessage, 0x1005, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 8, 0xb000, 0);
  Call3((void (*)())Func_02003298, 0x8008, 0, 40);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1b4) = 120;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Call1((void (*)())Func_020032e8, 12);
}
