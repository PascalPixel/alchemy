#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Object_GetById();
s32 Object_SetModeById();
s32 Event_StartScreenTransition();
s32 Func_020010f4();
s32 GameFlag_Test();
extern u8 gPartyState[];

s32 Func_0200012c(void) {
  s32 t0;
  s32 t3;
  s32 t4;
  s32 t7;

  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x100;
  t0 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
  FIELD_AT((t0 + 0x23), u8, 0) = (0x20 | FIELD_AT((t0 + 0x23), u8, 0));
  FIELD_AT(0x4000050, u16, 0) = 0x3f42;
  FIELD_AT(0x4000052, u16, 0) = 0x80c;
  Call2(Object_SetModeById, 0x18, 2);
  Call2(Object_SetModeById, 0x19, 2);
  t3 = Value1(Object_GetById, 0x18);
  FIELD_AT((t3 + 0x23), u8, 0) = 2;
  t4 = Value1(Object_GetById, 0x19);
  FIELD_AT((t4 + 0x23), u8, 0) = 2;
  Event_StartScreenTransition();
  if (FIELD_AT(gPartyState, s16, 482) == 1) {
    Func_020010f4();
    t7 = Value1(GameFlag_Test, 0x200);
    if (t7 != 0) {
      FIELD_AT(0x4000050, u16, 0) = 0x3f42;
      FIELD_AT(0x4000052, u16, 0) = 0x1000;
    }
  }
  return 0;
}
