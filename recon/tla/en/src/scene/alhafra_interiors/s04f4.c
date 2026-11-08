#include "alhafra_interiors.h"

void Func_020004f4(s32 actor) {
  s32 msg = (s32)MsgAlhafraInteriors74;

  Func_02000b24(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02000ba4(), 0) == 0) {
    FieldEvent_Wait(10);
    Func_02000b24(msg + 1);
  } else {
    FieldEvent_Wait(20);
    Func_02000b24(msg + 2);
  }
  Func_02000b3c(actor, 0);
}

u8 *Func_02000540(void) {
  if (GameFlag_Test(SPLIT_CONSTANT(SPLIT_CONSTANT(0x7a8, 255))))
    return gData_020019dc;
  if (GameFlag_Test(SPLIT_CONSTANT(SPLIT_CONSTANT(0x800, 171))))
    return gData_02001730;
  return gData_020013dc;
}
