#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "call.h"
#include "text/msg_ids.h"
#include "scene_tables.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

struct Actor *Actor_GetById(s32 id);
void FieldMessage_Show(s32 message, s32 position);
void Func_02001d7c(s32 value);
void Func_02001bb4(s32 value);

s32 GameFlag_Test();
void GameFlag_SetBit();
void FieldEvent_Wait();
void Actor_SetAngleToward();
void Actor_SetPositionAndReset();
void Actor_SetSpeedParameters();
s32 FieldMessage_ShowAtActor();
s32 Event_AskYesNoWithActorAnimation();
void Motion_SetModeAndWaitAnimation();
void Func_02001bcc(void);
void Func_02001bd4(void);
void Func_02001c6c();
void Func_02001c7c();
void Func_02001c84();
void Func_02001c94();
s32 Func_02001ca4();
void Func_02001d04();

void Func_020008e8(void) {
  s32 y = Actor_GetById(gMainRoutineWork.currentOwner)->y;


  if (y < 0x5a0000) {
    FieldMessage_Show((s32)MsgAndTheOthers, 1);
    Func_02001d7c(126);
    Func_02001bb4(1);
  } else {
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 346) = 1;
  }
}

/// Actor 30 offers a test of luck at the Spring of Lemuria; flag 0x908
/// records that the party took it.
void Func_02000934(void) {
  if (Value1(GameFlag_Test, 0x908)) {
    Actor_SetAngleToward(30, gMainRoutineWork.currentOwner, 0);
    Actor_RefreshSelectorById(30);
    Func_02001c6c((s32)MsgYouLookLike);
    Func_02001c84(30, 0);
    Call2(Func_02001c94, 30, FACING_NORTH + FACING_STEP);
    return;
  }
  Func_02001bcc();
  Func_02001d04(0);
  Func_02001c6c((s32)MsgDoYouWish);
  Func_02001ca4(30, 0x100, 30);
  Actor_SetAngleToward(30, gMainRoutineWork.currentOwner, 0);
  Actor_RefreshSelectorById(30);
  FieldEvent_Wait(5);
  FieldMessage_ShowAtActor(30, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0)) {
    FieldEvent_Wait(30);
    Func_02001c7c(30, 0, 0);
  } else {
    FieldEvent_Wait(20);
    Motion_SetModeAndWaitAnimation(30, 3);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Func_02001c7c(30, 0, 5);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 464, 1040);
    Func_02001c94(gMainRoutineWork.currentOwner, FACING_NORTH);
    FieldEvent_Wait(5);
    Call3(Actor_SetSpeedParameters, 30, 0x13333, 0x9999);
    Actor_SetPositionAndReset(30, 440, 1048);
    GameFlag_SetBit(0x908);
  }
  Call2(Func_02001c94, 30, FACING_NORTH + FACING_STEP);
  Func_02001bd4();
}
