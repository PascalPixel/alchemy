#include "types.h"
#include "scene_tables.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
#include "event_inline.h"
s32 Func_020028b4();
s32 Func_02002864();
s32 Func_020028bc();
extern u8 gData_02003040[];

void Func_02000440(void) {
  s32 t7;
  s32 t11;

  Event_SetFlag(0x892);
  Func_020028b4();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(165);
  Actor_SetAnimation(10, 3);
  FieldActor_SetAnimation(11, 3);
  FieldEvent_Wait(21);
  t7 = Actor_Get(10);
  FIELD_AT((t7 + 0x55), u8, 0) = 0;
  FIELD_AT(t7, s32, 48) = 0x20000;
  FIELD_AT(t7, s32, 52) = 0x8000;
  Actor_SetAnimation(10, 4);
  FieldActor_SetMoveTarget(t7, PIXELS(616), PIXELS(96), PIXELS(176));
  Func_02002864(t7, (s32)gData_02003040);
  t11 = Actor_Get(11);
  FIELD_AT((t11 + 0x55), u8, 0) = 0;
  FIELD_AT(t11, s32, 48) = 0x20000;
  FIELD_AT(t11, s32, 52) = 0x8000;
  Actor_SetAnimation(11, 4);
  FieldActor_SetMoveTarget(t11, PIXELS(120), PIXELS(96), PIXELS(176));
  Call2(Func_02002864, t11, (s32)gData_02003040);
  Func_020028bc();
}
