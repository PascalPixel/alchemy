#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02004e5c();
s32 Func_02000f00();
s32 Func_02000fac();
s32 Func_02004e64();
extern u8 MsgGaiaRockInside07[];
extern u8 gData_02005610[];
extern u8 MsgGaiaRockInside04[];

void Func_020012ac(void) {
  s32 t5;
  s32 t2;

  Func_02004e5c();
  Event_ResolvePendingActions(0);
  t2 = PartyInventory_FindOwner(0x1c3);
  if (t2 != -1) {
    Event_SetMessageId((s32)MsgGaiaRockInside07);
    Event_ShowMessageAndWait(0xa, 0);
  } else {
    t5 = ((s32)Actor_GetById(9));
    if (FIELD_AT(t5, s32, 0) == (s32)gData_02005610) {
      Actor_RefreshSelectorById(9);
    }
    Func_02000f00();
    Event_SetMessageId((s32)MsgGaiaRockInside04);
    Event_ShowMessage(0xa, 0);
    Func_02000fac();
  }
  Func_02004e64();
}
