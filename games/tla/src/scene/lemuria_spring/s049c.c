#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
#include "scene_tables.h"
s32 Func_02001380();
s32 Func_020013f8();
s32 FieldEvent_Wait();
s32 Func_02001408();
s32 Actor_ArmCallback();
s32 Func_020012f8();
s32 Actor_GetById();
s32 Func_020013b8();
s32 Actor_SetModeById();
s32 Func_02001400();
s32 Func_02001338();
s32 Func_02001370();
s32 Func_020013b0();
s32 Func_020013f0();
s32 PartyInventory_GiveItem();
s32 Func_02001388();

void Func_0200049c(s32 a0) {
  s32 t52;

  Func_02001380();
  Call1(Func_020013f8, 0);
  Call1(FieldEvent_Wait, 30);
  Call1(Func_02001408, 0x94);
  Call1(FieldEvent_Wait, 100);
  Call3(Actor_ArmCallback, 0, FACING_NORTH, 0);
  Call1(FieldEvent_Wait, 40);
  Call6(Func_020012f8, 0x52, 0x14, 0x46, 0, 3, 8);
  Call1(FieldEvent_Wait, 3);
  Call6(Func_020012f8, 0x55, 0x14, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x58, 0x14, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x5b, 0x14, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x5e, 0x14, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x61, 0x14, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x64, 0x14, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x4f, 0x1d, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x52, 0x1d, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x55, 0x1d, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x58, 0x1d, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x5b, 0x1d, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x5e, 0x1d, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x61, 0x1d, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x64, 0x1d, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 70);
  Call1(Func_02001408, 0x7e);
  if (a0 == -1) {
    t52 = Value1(Actor_GetById, 0x1d);
    Call2(Func_020013b8, 0x1d, FIELD_AT(&gMainRoutineWork, s32, 532));
    FIELD_AT((t52 + 0x55), u8, 0) = 0;
    FIELD_AT(t52, s32, 12) = 0x200000;
    Call2(Actor_SetModeById, FIELD_AT(&gMainRoutineWork, s32, 532), 0x1c);
    Call1(Func_02001400, 0x1d);
    Call1(Func_02001338, 0x16);
    Call1(Func_02001370, 0x16);
    Call3(Func_020013b0, 0x1d, 0, 0);
    Call2(Actor_SetModeById, FIELD_AT(&gMainRoutineWork, s32, 532), 1);
  } else {
    Call2(Func_020013f0, a0, 3);
    Call2(PartyInventory_GiveItem, a0, 0);
    Call1(FieldEvent_Wait, 20);
  }
  Call6(Func_020012f8, 0x61, 0x1d, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x5e, 0x1d, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x5b, 0x1d, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x58, 0x1d, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x55, 0x1d, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x52, 0x1d, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x64, 0x14, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x61, 0x14, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x5e, 0x14, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x5b, 0x14, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x58, 0x14, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x55, 0x14, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x52, 0x14, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_020012f8, 0x4f, 0x14, 0x46, 0, 3, 8);
  Call1(Func_02001408, 0x9a);
  Call1(FieldEvent_Wait, 8);
  Func_02001388();
}
