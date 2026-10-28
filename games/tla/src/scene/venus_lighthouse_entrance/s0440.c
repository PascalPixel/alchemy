#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 GameFlag_SetBit();
s32 Func_020028b4();
s32 Func_020029cc();
s32 Import_SoundPlay();
s32 Actor_SetModeById();
s32 FieldEvent_Wait();
s32 Actor_GetById();
s32 Func_02002884();
s32 Func_02002864();
s32 Func_020028bc();
extern u8 gData_02003040[];

void Func_02000440(void) {
  s32 t7;
  s32 t11;

  Call1(GameFlag_SetBit, 0x892);
  Func_020028b4();
  Call1(Func_020029cc, 0);
  Call1(Import_SoundPlay, 0xa5);
  Call2(Actor_SetModeById, 0xa, 3);
  Call2(Actor_SetModeById, 0xb, 3);
  Call1(FieldEvent_Wait, 0x15);
  t7 = Value1(Actor_GetById, 0xa);
  FIELD_AT((t7 + 0x55), u8, 0) = 0;
  FIELD_AT(t7, s32, 48) = 0x20000;
  FIELD_AT(t7, s32, 52) = 0x8000;
  Call2(Actor_SetModeById, 0xa, 4);
  Call4(Func_02002884, t7, 0x2680000, 0x600000, 0xb00000);
  Call2(Func_02002864, t7, (s32)gData_02003040);
  t11 = Value1(Actor_GetById, 0xb);
  FIELD_AT((t11 + 0x55), u8, 0) = 0;
  FIELD_AT(t11, s32, 48) = 0x20000;
  FIELD_AT(t11, s32, 52) = 0x8000;
  Call2(Actor_SetModeById, 0xb, 4);
  Call4(Func_02002884, t11, 0x780000, 0x600000, 0xb00000);
  Call2(Func_02002864, t11, (s32)gData_02003040);
  Func_020028bc();
}
