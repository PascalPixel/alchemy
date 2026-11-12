/// @file
/// Scene code of overlay 0x64d, in ROM order, from prepare's offset up to the next object the overlay links.

#include "types.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "call.h"

s32 Scene_PrepareMap(void) { return 0; }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Func_02002864();
s32 Func_0200298c();
extern u8 gData_0200b000[];

void Func_0200030c(void) {
  s32 t1;
  s32 t3;
  s32 t9;
  s32 t16;

  Event_SetFlag(0x891);
  t1 = ((s32)Actor_Get(10));
  FIELD_AT((t1 + 0x55), u8, 0) = 0;
  FIELD_AT(t1, s32, 72) = 0x28f;
  FIELD_AT(t1, s32, 48) = 0x18000;
  FIELD_AT(t1, s32, 52) = 0x8000;
  FieldActor_SetPositionAndResetMotion(t1, PIXELS(216), PIXELS(96),
                                       PIXELS(176));
  t3 = ((s32)Actor_Get(11));
  FIELD_AT((t3 + 0x55), u8, 0) = 0;
  FIELD_AT(t3, s32, 72) = 0x7ae;
  FIELD_AT(t3, s32, 48) = 0x18000;
  FIELD_AT(t3, s32, 52) = 0x8000;
  FieldActor_SetPositionAndResetMotion(t3, PIXELS(616), PIXELS(96),
                                       PIXELS(152));
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(376), -1, PIXELS(248), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  t9 = ((s32)Actor_Get(10));
  Actor_SetAnimation(10, 4);
  Event_PlaySound(165);
  FieldActor_SetMoveTarget(t9, PIXELS(352), PIXELS(8), PIXELS(176));
  Func_02002864(t9, (s32)gData_0200b000);
  FieldEvent_Wait(25);
  FieldCamera_MoveTo(PIXELS(376), -1, PIXELS(104), 1);
  t16 = ((s32)Actor_Get(11));
  Actor_SetAnimation(11, 4);
  FieldActor_SetMoveTarget(t16, PIXELS(416), PIXELS(8), PIXELS(152));
  Func_02002864(t16, (s32)gData_0200b000);
  FieldEvent_Wait(60);
  Event_PlaySound(165);
  FieldActor_WaitForMoveById(10);
  FieldActor_WaitForMoveById(11);
  Func_0200298c();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

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
