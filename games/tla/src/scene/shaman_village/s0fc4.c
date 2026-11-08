#include "shaman_village.h"

u32 Func_02004a8c(s32 shop);
void Func_02004a94(u32 price);
void Func_0200487c(u32 value, s32 digits);

void Func_02000fc4(void) {
  if (GameFlag_Test(0x9fe)) {
    if (Func_02000f94()) {
      Func_02004a84(11, 13);
    } else {
      Func_020048c4();
      Func_02004a24(0);
      Func_0200498c((s32)MsgShamanVisitorsHaveTraveledFar);
      Func_020049ac(13, 0);
      Func_020048cc();
    }
  } else {
    Func_020048c4();
    Func_02004a24(0);
    if (Func_02000f94()) {
      u32 price = Func_02004a8c(11);

      Actor_SetModeById(13, 5);
      FieldEvent_Wait(30);
      Func_0200498c((s32)MsgShamanInnPaymentPrompt);
      Func_0200487c(price, 5);
      Func_0200487c(price, 5);
      FieldMessage_ShowAtActor(12, 0);
      if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
        if ((u32)gMainRoutineWork.coins < price) {
          Actor_SetModeById(13, 6);
          FieldEvent_Wait(20);
          Actor_SetModeById(13, 5);
          FieldEvent_Wait(30);
          Func_0200498c((s32)MsgShamanInnPaymentPrompt + 3);
          Func_020049a4(12, 0);
        } else {
          Func_02004a94(price);
        }
      }
      Actor_SetModeById(13, 6);
    } else {
      Func_0200498c((s32)MsgShamanSilentVillager);
      Func_020049a4(13, 0);
    }
    Func_020048cc();
  }
}
