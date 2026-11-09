#include "types.h"
#include "main_routine_work.h"
#include "flag_call.h"
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
void Func_02000980(s32 value, s32 slot);
u16 *Func_02000988(s32 item);
s32 Func_020009a0(s32 item);
void Func_02000a40(s32 item, s32 mode);
void Func_02000a78(s32 actor);
void Func_02000a88(s32 value);
s32 Func_02000a90(void);

void Func_0200077c(s32 actor) {
  if (!Flag_IsSet(0x10f)) {
    Event_SetMessageId((s32)MsgThisThisIs);
    Event_ShowMessageAndWait(actor, 0);
  } else if (Flag_IsSet(0x301)) {
    Event_SetMessageId((s32)MsgItTakesOur);
    Event_ShowMessageAndWait(actor, 0);
  } else if (Func_02000a90()) {
    s32 item = Func_02000a90();
    u32 price = *Func_02000988(item);
    s32 message;

    Func_02000980(item, 2);
    Func_02000980(price, 5);
    message = (s32)MsgHereSThe;
    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) != 0) {
      Event_SetMessageId(message + 1);
      Event_ShowMessageAndWait(actor, 0);
    } else if (Func_020009a0(item) < 0) {
      Event_SetMessageId(message + 2);
      Event_ShowMessageAndWait(actor, 0);
    } else if ((u32)gMainRoutineWork.coins < price) {
      Event_SetMessageId(message + 3);
      Event_ShowMessageAndWait(actor, 0);
    } else {
      Event_SetMessageId(message + 4);
      Event_ShowMessageAndWait(actor, 0);
      Func_02000a40(item, 3);
      PartyInventory_GiveItem(item, 0);
      Func_02000a88(0);
      Party_AdjustCoins(-price);
    }
  } else {
    Event_SetMessageId((s32)MsgManSunshineWould);
    Event_ShowMessageAndWait(actor, 0);
  }
}
