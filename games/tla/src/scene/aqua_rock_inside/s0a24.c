#include "local.h"

void Func_02000a24(void) {
  s32 seen;
  const u8 *message;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  seen = Event_GetFlag(0x95d);
  message = MsgCheckedTheColumn;
  if (!seen) {
    Message_ShowCentered(message, 1);
    Message_ShowCentered(message + 1, 1);
    if (FieldPartyInventory_FindOwner(0x1c8) != -1)
      ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->value15a = 1;
  } else {
    Message_ShowCentered(message, 1);
    Func_02003d64(0x1c8, 2);
    Message_ShowCentered(message + 2, 1);
  }
  Func_02003d84();
}
