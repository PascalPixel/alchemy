#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern const u8 MsgAhSoYou[];
extern const u8 MsgICanTWait[];
extern const u8 MsgMyWifeHas[];
extern const u8 MsgYouWantMe[];
extern const u8 MsgThisThisIs[];
extern const u8 MsgItTakesOur[];
extern const u8 MsgHereSThe[];
extern const u8 MsgManSunshineWould[];

void Party_AdjustCoins(s32 amount);
void Func_02000d08(s32 value, s32 slot);
u16 *Func_02000d48(s32 item);
s32 Func_02000dc8(s32 item);
void Func_02000e28(s32 item, s32 mode);
void Func_02000e58(s32 actor);
void Func_02000e68(s32 value);
s32 Func_02000e70(void);

void Func_020001d4(s32 actor) {
  s32 message;

  if (!Event_GetFlag(0x10f)) {
    message = (s32)MsgAhSoYou;
    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(message + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(message + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
  } else if (Event_GetFlag(0x200)) {
    Event_SetMessageId((s32)MsgICanTWait);
    Event_ShowMessageAndWait(actor, 0);
  } else if (Func_02000e70()) {
    Event_SetMessageId((s32)MsgMyWifeHas);
    Event_ShowMessageAndWait(actor, 0);
  } else {
    message = (s32)MsgYouWantMe;
    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(message + 1);
      Event_ShowMessageAndWait(actor, 0);
      Func_02000e58(actor);
      if (Func_02000e70())
        Event_SetFlag(0x200);
    } else {
      Event_SetMessageId(message + 2);
      Event_ShowMessageAndWait(actor, 0);
    }
  }
}
