#include "local.h"

void Func_02000a24(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 6 of 45 instructions
   * differ in tla-en. */
  s32 seen;
  const u8 *message;

  Func_02003d7c();
  Func_02003eac(0);
  seen = GameFlag_Test(SPLIT_CONSTANT(0x900, 93));
  message = MsgCheckedTheColumn;
  if (!seen) {
    FieldMessage_Show(message, 1);
    FieldMessage_Show(message + 1, 1);
    if (PartyInventory_FindOwner(0x1c8) != -1)
      ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->value15a = 1;
  } else {
    FieldMessage_Show(message, 1);
    Func_02003d64(0x1c8, 2);
    FieldMessage_Show(message + 2, 1);
  }
  Func_02003d84();
}
