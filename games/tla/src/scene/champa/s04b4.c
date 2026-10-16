#include "types.h"

extern u8 MsgChampa30[];
extern u8 MsgChampa05[];

s32 GameFlag_Test(s32 flag);
void Func_02002adc(s32 message);
void UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Func_02002bb4(void);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void FieldEvent_WaitFrames(s32 frames);
void Func_02002af4(s32 speaker, s32 flags);

void Func_020004b4(s32 actor) {
  if (GameFlag_Test(0x97f)) {
    Func_02002adc((s32)MsgChampa30);
    UiText_OpenMessageAtObject(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02002bb4(), 0) == 0) {
      FieldEvent_WaitFrames(10);
      Func_02002adc((s32)MsgChampa30 + 1);
    } else {
      FieldEvent_WaitFrames(20);
      Func_02002adc((s32)MsgChampa30 + 2);
    }
    Func_02002af4(actor, 0);
  } else {
    Func_02002adc((s32)MsgChampa05);
    UiText_OpenMessageAtObject(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02002bb4(), 0) == 0) {
      FieldEvent_WaitFrames(10);
      Func_02002adc((s32)MsgChampa05 + 1);
    } else {
      FieldEvent_WaitFrames(20);
      Func_02002adc((s32)MsgChampa05 + 2);
    }
    Func_02002af4(actor, 0);
  }
}
