#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "call.h"
#include "text/msg_ids.h"

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
  Call3((void (*)())Func_02003228, 6, 0x1bc0000, 0xd40000);
  Call2((void (*)())Import_ActorSetAnimation, 6, 19);
  Actor_SetSingleChildFlags(Actor_GetById(6), 0);
  Call3((void (*)())Func_02003228, gSequenceParentWork.currentOwner, 0x1b00000,
        0xea0000);
  Actor_GetById(gSequenceParentWork.currentOwner)->scaleX = 0xffff0000;
  Call2((void (*)())Import_ActorSetAnimation, gSequenceParentWork.currentOwner, 41);
  Actor_SetSingleChildFlags(Actor_GetById(gSequenceParentWork.currentOwner),
                                     0);
  Call3((void (*)())Func_02003228, 10, 0x1880000, 0xac0000);
  Task_Wait(1);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0xc00000, -1, 0xd60000, 0);
  Func_02003180();
  Task_Wait(1);
  Call3((void (*)())Func_02003228, 5, 0xd00000, 0xda0000);
  Call2((void (*)())Import_ActorSetAnimation, 5, 19);
  Actor_SetSingleChildFlags(Actor_GetById(5), 0);
  Call3((void (*)())Func_02003228, 8, 0xb00000, 0xcc0000);
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
  Call3((void (*)())Actor_ArmCallback, 8, 0x3000, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0xa000, 40);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0xcccc, 0x6666);
  Call2((void (*)())Actor_SetMovementTypeById, 8, gData_02003400);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call2((void (*)())Import_CameraSetSpeed, 0xcccc, 0x1999);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1700000, -1, 0xd20000, 1);
  actor->action = 0;
  Call2((void (*)())Actor_SetMovementTypeById, 5, gData_0200346c);
  do {
    Task_Wait(1);
  } while (actor->action == 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Import_ActorSetAttachedEffect, 8, 258);
  Call2((void (*)())Import_ActorSetAttachedEffect, 5, 258);
  Call11((void (*)())Func_020032b8, 5, 7, 13, 2, 12, 8, 9, 2, 4, 1, 0);
  Call3((void (*)())Actor_ArmCallback, 10, 0x3000, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x19999, 0xcccc);
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x19999, 0xcccc);
  Call2((void (*)())Actor_SetMovementTypeById, 8, gData_020034e4);
  Call2((void (*)())Actor_SetMovementTypeAndWaitById, 5, gData_0200350c);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call2((void (*)())Import_EventShowMessageAndWait, 0x5008, 0);
  Call3((void (*)())Actor_Jump, 5, 4, 10);
  Call2((void (*)())Import_EventShowMessageAndWait, 0x1005, 0);
  Call3((void (*)())Actor_ArmCallback, 8, 0xb000, 0);
  Call3((void (*)())Func_02003298, 0x8008, 0, 40);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1b4) = 120;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Call1((void (*)())Func_020032e8, 12);
}
