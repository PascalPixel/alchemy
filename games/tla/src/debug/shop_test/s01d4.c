#include "types.h"
#include "adventure.h"
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
void PartyInventory_GiveItem(s32 item, s32 count);
void Party_AdjustCoins(s32 amount);
void Func_02000d08(s32 value, s32 slot);
u16 *Func_02000d48(s32 item);
s32 Func_02000dc8(s32 item);
void Func_02000e08(s32 message);
void Func_02000e18(s32 actor, s32 flags);
void Func_02000e28(s32 item, s32 mode);
s32 Func_02000e30(void);
void Func_02000e58(s32 actor);
void Func_02000e68(s32 value);
s32 Func_02000e70(void);

void Func_020001d4(s32 actor) {
  s32 message;

  if (!Flag_IsSet(0x10f)) {
    message = (s32)MsgAhSoYou;
    Func_02000e08(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02000e30(), 0) == 0) {
      FieldEvent_Wait(10);
      Func_02000e08(message + 1);
    } else {
      FieldEvent_Wait(20);
      Func_02000e08(message + 2);
    }
    Func_02000e18(actor, 0);
  } else if (Flag_IsSet(0x200)) {
    Func_02000e08((s32)MsgICanTWait);
    Func_02000e18(actor, 0);
  } else if (Func_02000e70()) {
    Func_02000e08((s32)MsgMyWifeHas);
    Func_02000e18(actor, 0);
  } else {
    message = (s32)MsgYouWantMe;
    Func_02000e08(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02000e30(), 0) == 0) {
      Func_02000e08(message + 1);
      Func_02000e18(actor, 0);
      Func_02000e58(actor);
      if (Func_02000e70())
        Flag_Set(0x200);
    } else {
      Func_02000e08(message + 2);
      Func_02000e18(actor, 0);
    }
  }
}
