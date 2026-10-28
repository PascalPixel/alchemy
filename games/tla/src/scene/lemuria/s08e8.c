#include "types.h"
#include "ram_buffer.h"
#include "sequence_parent_work.h"
#include "actor.h"
#include "call.h"
#include "text/msg_ids.h"

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
  s32 y = Actor_GetById(gSequenceParentWork.currentOwner)->y;

  /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the owner's y
   * before it builds the limit (2 differ without it). */
  do {
  } while (0);
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
    Call3(Actor_SetAngleToward, 30, gSequenceParentWork.currentOwner, 0);
    Call1((void (*)())Actor_RefreshSelectorById, 30);
    Call1(Func_02001c6c, (s32)MsgYouLookLike);
    Call2(Func_02001c84, 30, 0);
    Call2(Func_02001c94, 30, 0xd000);
    return;
  }
  Func_02001bcc();
  Call1(Func_02001d04, 0);
  Call1(Func_02001c6c, (s32)MsgDoYouWish);
  Value3(Func_02001ca4, 30, 0x100, 30);
  Call3(Actor_SetAngleToward, 30, gSequenceParentWork.currentOwner, 0);
  Call1((void (*)())Actor_RefreshSelectorById, 30);
  Call1(FieldEvent_Wait, 5);
  Value2(FieldMessage_ShowAtActor, 30, 0);
  if (Value2(Event_AskYesNoWithActorAnimation, 4, 0)) {
    Call1(FieldEvent_Wait, 30);
    Call3(Func_02001c7c, 30, 0, 0);
  } else {
    Call1(FieldEvent_Wait, 20);
    Call2(Motion_SetModeAndWaitAnimation, 30, 3);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Call3(Func_02001c7c, 30, 0, 5);
    Call3(Actor_SetPositionAndReset, gSequenceParentWork.currentOwner, 0x1d0,
          0x410);
    Call2(Func_02001c94, gSequenceParentWork.currentOwner, 0xc000);
    Call1(FieldEvent_Wait, 5);
    Call3(Actor_SetSpeedParameters, 30, 0x13333, 0x9999);
    Call3(Actor_SetPositionAndReset, 30, 0x1b8, 0x418);
    Call1(GameFlag_SetBit, 0x908);
  }
  Call2(Func_02001c94, 30, 0xd000);
  Func_02001bd4();
}
