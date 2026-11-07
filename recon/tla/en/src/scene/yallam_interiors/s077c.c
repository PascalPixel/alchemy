#include "types.h"
#include "main_routine_work.h"
#include "flag_call.h"

extern const u8 MsgAhSoYou[];
extern const u8 MsgICanTWait[];
extern const u8 MsgMyWifeHas[];
extern const u8 MsgYouWantMe[];
extern const u8 MsgThisThisIs[];
extern const u8 MsgItTakesOur[];
extern const u8 MsgHereSThe[];
extern const u8 MsgManSunshineWould[];

void FieldEvent_Wait(s32 frames);
s32 FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
s32 PartyInventory_GiveItem(s32 item, s32 count);
void Party_AdjustCoins(s32 amount);
void Func_02000980(s32 value, s32 slot);
u16 *Func_02000988(s32 item);
s32 Func_020009a0(s32 item);
void Func_02000a18(s32 message);
void Func_02000a28(s32 actor, s32 flags);
void Func_02000a40(s32 item, s32 mode);
s32 Func_02000a58(void);
void Func_02000a78(s32 actor);
void Func_02000a88(s32 value);
s32 Func_02000a90(void);

void Func_0200077c(s32 actor) {
  if (!Flag_IsSet(0x10f)) {
    Func_02000a18((s32)MsgThisThisIs);
    Func_02000a28(actor, 0);
  } else if (Flag_IsSet(0x301)) {
    Func_02000a18((s32)MsgItTakesOur);
    Func_02000a28(actor, 0);
  } else if (Func_02000a90()) {
    s32 item = Func_02000a90();
    u32 price = *Func_02000988(item);
    s32 message;

    Func_02000980(item, 2);
    Func_02000980(price, 5);
    message = (s32)MsgHereSThe;
    Func_02000a18(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02000a58(), 0) != 0) {
      Func_02000a18(message + 1);
      Func_02000a28(actor, 0);
    } else if (Func_020009a0(item) < 0) {
      Func_02000a18(message + 2);
      Func_02000a28(actor, 0);
    } else if ((u32)gMainRoutineWork.coins < price) {
      Func_02000a18(message + 3);
      Func_02000a28(actor, 0);
    } else {
      Func_02000a18(message + 4);
      Func_02000a28(actor, 0);
      Func_02000a40(item, 3);
      PartyInventory_GiveItem(item, 0);
      Func_02000a88(0);
      Party_AdjustCoins(-price);
    }
  } else {
    Func_02000a18((s32)MsgManSunshineWould);
    Func_02000a28(actor, 0);
  }
}
