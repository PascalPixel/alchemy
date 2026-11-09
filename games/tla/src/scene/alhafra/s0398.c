#include "types.h"
#include "ram_buffer.h"
#include "event_inline.h"

struct AlhafraEventWork {
  u8 unk000[0x15a];
  s16 value15a;
  u8 unk15c[0x68];
  u16 value1c4;
};

extern const u8 MsgAlhafra65[];
extern const u8 MsgAlhafra70[];
extern const u8 MsgAlhafra71[];

void Func_02000398(s32 id) {
  if (!GameFlag_Test(0x8b5)) {
    ((struct AlhafraEventWork *)Ram_HeapSlots->eventWork)->value15a = 1;
    Event_SetMessageId(MsgAlhafra65);
    Event_ShowMessageAndWait(id, 0);
  } else if (Event_GetFlag(0x8b6)) {
    Event_SetMessageId(MsgAlhafra71);
    Event_ShowMessageAndWait(id, 0);
  } else {
    Event_SetMessageId(MsgAlhafra70);
    Event_Wait(10);
    Motion_SetModeAndWaitAnimation(id, 3);
    Event_ShowMessageThenWait(id, 0, 10);
    Motion_SetVarCbAndRefresh(id, 2);
    FieldMessage_ShowAtActor(id, 0);
    Event_Wait(10);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      GameFlag_SetBit(0x8b6);
      Event_Wait(20);
      Event_ShowMessageThenWait(id, 0, 10);
      ((struct AlhafraEventWork *)Ram_HeapSlots->eventWork)->value1c4++;
    } else {
      Event_Wait(30);
      ((struct AlhafraEventWork *)Ram_HeapSlots->eventWork)->value1c4++;
      Event_ShowMessageThenWait(id, 0, 10);
    }
  }
}
