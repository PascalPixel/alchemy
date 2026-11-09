#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern char SceneId_Ov6A6Room2;
extern char MsgLoho23;
extern char MsgLoho27;
extern const u8 gData_02001900[];
extern const u8 gData_02001a80[];

struct EventWork15a {
  u8 unk000[0x15a];
  u16 flag;
};

const u8 *Func_02000080(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room2)
    return gData_02001a80;
  return gData_02001900;
}

void Func_020000ac(void) {
  Map_CopyCellAttributeRect(69, 70, 10, 12, 24, 7);
  Func_0802b450(69, 70, 10, 12, 24, 7);
  Map_CopyCellAttributeRect(79, 70, 10, 12, 88, 7);
  Func_0802b450(79, 70, 10, 12, 88, 7);
  Map_CopyCellAttributeRect(69, 82, 10, 12, 24, 71);
  Func_0802b450(69, 82, 10, 12, 24, 71);
}

void Func_0200012c(s32 actor) {
  s32 msg = (s32)&MsgLoho23;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void Func_02000178(void) {
  FieldMessage_Show((s32)&MsgLoho27, 1);
  ((struct EventWork15a *)Ram_HeapSlots->eventWork)->flag = 1;
}
