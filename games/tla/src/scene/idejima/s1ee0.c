#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "call.h"
#include "text/msg_ids.h"
#include "scene_tables.h"

extern const u8 gData_020033a8[];
extern const u8 gData_02003400[];
extern const u8 gData_0200346c[];
extern const u8 gData_020034e4[];
extern const u8 gData_0200350c[];

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void FieldEvent_Wait();
void Camera_MoveWithinMapBounds();
void Import_CameraSetSpeed();
s32 Import_ActorShowEmote();
void Import_ActorSetAttachedEffect();
void Event_StartScreenTransition();
void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void Actor_SetSpeedParameters();
void Actor_SetPositionAndReset();
void Actor_SetMovementTypeById();
void Actor_SetMovementTypeAndWaitById();
void Actor_Jump();
void Actor_ArmCallback();
void Func_0200161c(void);
void DriftScene_Prepare(void);
void Func_02003180();
void Func_02003228();
void Func_02003298();
void Func_020032b8();
void Func_020032e8();
void Func_02003310();

void Func_02001ee0(void) {
  struct Actor *actor = Actor_GetById(5);

  Import_EventStart();
  Call1((void (*)())Func_02003310, 0);
  Func_0200161c();
  DriftScene_Prepare();
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call3((void (*)())Func_02003228, 6, PIXELS(444), PIXELS(212));
  Call2((void (*)())Import_ActorSetAnimation, 6, 19);
  Actor_SetSingleChildFlags(Actor_GetById(6), 0);
  Call3((void (*)())Func_02003228, gMainRoutineWork.currentOwner, PIXELS(432),
        PIXELS(234));
  Actor_GetById(gMainRoutineWork.currentOwner)->scaleX = 0xffff0000;
  Call2((void (*)())Import_ActorSetAnimation, gMainRoutineWork.currentOwner, 41);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     0);
  Call3((void (*)())Func_02003228, 10, PIXELS(392), PIXELS(172));
  Task_Wait(1);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(192), -1, PIXELS(214), 0);
  Func_02003180();
  Task_Wait(1);
  Call3((void (*)())Func_02003228, 5, PIXELS(208), PIXELS(218));
  Call2((void (*)())Import_ActorSetAnimation, 5, 19);
  Actor_SetSingleChildFlags(Actor_GetById(5), 0);
  Call3((void (*)())Func_02003228, 8, PIXELS(176), PIXELS(204));
  Call2((void (*)())Import_ActorSetAnimation, 8, 5);
  Actor_SetSingleChildFlags(Actor_GetById(8), 0);
  Task_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call1((void (*)())FieldEvent_Wait, 120);
  Call1((void (*)())Import_EventSetMessageId, (s32)MsgIdejimaHowLongWillWeDrift);
  Call3((void (*)())Func_02003298, 5, 0, 40);
  Call2((void (*)())Import_EventShowMessageAndWait, 8, 0);
  Value3((s32 (*)())Import_ActorShowEmote, 5, 263, 0);
  Call2((void (*)())Import_EventShowMessageAndWait, 5, 0);
  Value3((s32 (*)())Import_ActorShowEmote, 8, 261, 80);
  Call2((void (*)())Import_EventShowMessageAndWait, 8, 0);
  Call2((void (*)())Import_EventShowMessageAndWait, 5, 0);
  Value3((s32 (*)())Import_ActorShowEmote, 8, 261, 40);
  Value3((s32 (*)())Import_ActorShowEmote, 5, 265, 80);
  Call2((void (*)())Import_EventShowMessageAndWait, 5, 0);
  Call2((void (*)())Import_EventShowMessageAndWait, 8, 0);
  Value3((s32 (*)())Import_ActorShowEmote, 5, 258, 0);
  Call2((void (*)())Import_EventShowMessageAndWait, 5, 0);
  Call2((void (*)())Import_ActorSetAttachedEffect, 8, 258);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call2((void (*)())Import_EventShowMessageAndWait, 8, 0);
  Value3((s32 (*)())Import_ActorShowEmote, 5, 261, 40);
  Call3((void (*)())Func_02003298, 8, 0, 40);
  Value3((s32 (*)())Import_ActorShowEmote, 5, 265, 20);
  Call3((void (*)())Func_02003298, 5, 0, 20);
  Call3((void (*)())Func_02003298, 8, 0, 40);
  Value3((s32 (*)())Import_ActorShowEmote, 5, 261, 20);
  Call2((void (*)())Import_EventShowMessageAndWait, 5, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 10, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetPositionAndReset, 10, 304, 172);
  Call3((void (*)())Actor_SetPositionAndReset, 10, 224, 172);
  Call3((void (*)())Actor_SetPositionAndReset, 10, 208, 192);
  Call2((void (*)())Import_EventShowMessageAndWait, 10, 0);
  Value3((s32 (*)())Import_ActorShowEmote, 8, 257, 0);
  Value3((s32 (*)())Import_ActorShowEmote, 5, 257, 80);
  Call2((void (*)())Import_EventShowMessageAndWait, 10, 0);
  Call2((void (*)())Actor_SetMovementTypeById, 10,
        gData_020033a8);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call2((void (*)())Import_ActorSetAnimation, 8, 1);
  Actor_SetSingleChildFlags(Actor_GetById(8), 1);
  Call3((void (*)())Actor_Jump, 8, 4, 20);
  Call2((void (*)())Import_ActorSetAnimation, 5, 1);
  Actor_SetSingleChildFlags(Actor_GetById(5), 1);
  Call3((void (*)())Actor_Jump, 5, 4, 20);
  Call3((void (*)())Actor_ArmCallback, 8, FACING_SOUTHEAST + FACING_STEP, 0);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTHWEST, 40);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0xcccc, 0x6666);
  Call2((void (*)())Actor_SetMovementTypeById, 8, gData_02003400);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call2((void (*)())Import_CameraSetSpeed, 0xcccc, 0x1999);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(368), -1, PIXELS(210), 1);
  actor->action = 0;
  Call2((void (*)())Actor_SetMovementTypeById, 5, gData_0200346c);
  do {
    Task_Wait(1);
  } while (actor->action == 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Import_ActorSetAttachedEffect, 8, 258);
  Call2((void (*)())Import_ActorSetAttachedEffect, 5, 258);
  Call11((void (*)())Func_020032b8, 5, 7, 13, 2, 12, 8, 9, 2, 4, 1, 0);
  Call3((void (*)())Actor_ArmCallback, 10, FACING_SOUTHEAST + FACING_STEP, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x19999, 0xcccc);
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x19999, 0xcccc);
  Call2((void (*)())Actor_SetMovementTypeById, 8, gData_020034e4);
  Call2((void (*)())Actor_SetMovementTypeAndWaitById, 5, gData_0200350c);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call2((void (*)())Import_EventShowMessageAndWait, 0x5008, 0);
  Call3((void (*)())Actor_Jump, 5, 4, 10);
  Call2((void (*)())Import_EventShowMessageAndWait, 0x1005, 0);
  Call3((void (*)())Actor_ArmCallback, 8, FACING_NORTHWEST + FACING_STEP, 0);
  Call3((void (*)())Func_02003298, 0x8008, 0, 40);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1b4) = 120;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Call1((void (*)())Func_020032e8, 12);
}
