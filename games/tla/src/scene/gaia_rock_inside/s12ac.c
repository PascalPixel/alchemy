/// @file
/// Scene code of overlay 0x681, in ROM order, from s12ac's offset up to the next object the overlay links.

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
  FieldEvent_ResolvePendingActions(0);
  t2 = FieldPartyInventory_FindOwner(0x1c3);
  if (t2 != -1) {
    FieldEvent_SetMessageId((s32)MsgGaiaRockInside07);
    Event_ShowMessage(0xa, 0);
  } else {
    t5 = ((s32)Actor_Get(9));
    if (FIELD_AT(t5, s32, 0) == (s32)gData_02005610) {
      FieldActor_RefreshSelectorById(9);
    }
    Func_02000f00();
    FieldEvent_SetMessageId((s32)MsgGaiaRockInside04);
    Event_ShowMessage(0xa, 0);
    Func_02000fac();
  }
  Func_02004e64();
}

#include "event_inline.h"
s32 Scene_SetActorSpritePriority();
s32 Func_02004d9c();
s32 Actor_SetSingleChildFlags();
s32 Animation_SetStateFlags();
s32 Func_02004dac();
extern u8 gData_02005208[];

void Func_02001314(s32 a0) {
  s32 t0;

  t0 = (s32)FieldActor_Create(0x2a0, FIELD_AT(a0, s32, 8),
                              FIELD_AT(a0, s32, 12), FIELD_AT(a0, s32, 16));
  FIELD_AT(t0, s32, 28) = 0x6666;
  FIELD_AT(t0, s32, 24) = 0x6666;
  FIELD_AT((t0 + 0x55), u8, 0) = 0;
  Actor_SetSpriteColor(t0, 2);
  Scene_SetActorSpritePriority(t0, 1);
  Func_02004d9c(t0, 1);
  Actor_SetSingleChildFlags(t0, 0);
  Animation_SetStateFlags(t0, 1);
  Func_02004dac(t0, (s32)gData_02005208);
}
