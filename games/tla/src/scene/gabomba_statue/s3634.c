#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Func_0200253c();
void Func_020027cc();
void Func_02002a64();
void Func_02002f70();
void Func_02003338();
s32 Func_02003604();
void Func_020048a4();
void Func_020052a4();
void Func_020053ac();
void Func_0200564c();
void Func_020056b4();
void Func_020056bc();
void Actor_SetSingleChildFlags();
void Func_02001ac0();
void Func_02002074();
extern u8 gData_02005d8c[];

void Func_02003634(void) {
  s32 t1, t4, t6, t8, t10, t12, t14, t16, t18, t20, t22, t24, t26, t27, t29,
      t30, t42, t43, t46, t50, t53, t59, t60, p_r3, v_r5;
  const u8 *table;

  t1 = ((s32)Actor_Get(10));
  Actor_SetSpriteColor(t1, 3);
  Task_AddFrame((s32)Func_02002074, 0x480);
  t4 = ((s32)Actor_Get(11));
  Actor_SetSpriteColor(t4, 11);
  t6 = ((s32)Actor_Get(12));
  Actor_SetSpriteColor(t6, 11);
  t8 = ((s32)Actor_Get(13));
  Actor_SetSpriteColor(t8, 10);
  t10 = ((s32)Actor_Get(14));
  Actor_SetSpriteColor(t10, 10);
  t12 = ((s32)Actor_Get(16));
  Actor_SetSingleChildFlags(t12, 0);
  t14 = ((s32)Actor_Get(17));
  Actor_SetSingleChildFlags(t14, 0);
  t16 = ((s32)Actor_Get(18));
  Actor_SetSingleChildFlags(t16, 0);
  t18 = ((s32)Actor_Get(19));
  Actor_SetSingleChildFlags(t18, 0);
  t20 = ((s32)Actor_Get(20));
  Actor_SetSingleChildFlags(t20, 0);
  t22 = ((s32)Actor_Get(21));
  Actor_SetSingleChildFlags(t22, 0);
  t24 = ((s32)Actor_Get(22));
  Actor_SetSingleChildFlags(t24, 0);
  t26 = Event_GetFlag(0x8fb);
  if (t26 == 0)
    goto L_0200b6f2;
  t27 = Event_GetFlag(0x8fd);
  if (t27 != 0)
    goto L_0200b6f2;
  Func_02002f70();
  goto L_0200b856;
L_0200b6f2:
  t29 = Event_GetFlag(0x8fc);
  if (t29 == 0)
    goto L_0200b762;
  t30 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  v_r5 = t30;
  Actor_SetPosition(11, 0, 0);
  Actor_SetPosition(12, 0, 0);
  Actor_SetPosition(13, 0, 0);
  Actor_SetPosition(14, 0, 0);
  Func_020053ac();
  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(FIELD_AT(v_r5, s32, 8), -1, FIELD_AT(v_r5, s32, 16), 0);
  Func_0200564c();
  System_Wait(1);
  Func_020056bc();
L_0200b762:
  t42 = Event_GetFlag(0x8f6);
  if (t42 == 0)
    goto L_0200b78e;
  t43 = Event_GetFlag(0x8fd);
  if (t43 != 0)
    goto L_0200b78e;
  Func_020052a4(0);
  Task_AddFrame((s32)Func_02001ac0, 0x480);
L_0200b78e:
  t46 = Func_02003604();
  /* FAKEMATCH(temporary): the table's address in a local set after the call,
   * which makes it the load's base register; as gData_02005d8c's address
   * added in place, 2 of 211 instructions differ in tla-en. */
  table = gData_02005d8c;
  Func_020048a4(19, 11, *(s32 *)(table + (t46 << 2)), 16);
  Actor_SetAnimation(15, 5);
  if (FIELD_AT(&gMainRoutineWork, s16, 482) != 2)
    goto L_0200b7c0;
  Func_020027cc();
  goto L_0200b856;
L_0200b7c0:
  t50 = Event_GetFlag(0x8f6);
  if (t50 != 0)
    goto L_0200b7e4;
  if (FIELD_AT(&gMainRoutineWork, s16, 482) != 1) {
    p_r3 = FIELD_AT(&gMainRoutineWork, s16, 482);
    goto L_0200b7dc;
  }
  Func_0200253c();
  goto L_0200b7e4;
L_0200b7dc:
  if (p_r3 != 3)
    goto L_0200b7e4;
  Func_02002a64();
L_0200b7e4:
  t53 = Event_GetFlag(0x8fc);
  if (t53 == 0)
    goto L_0200b856;
  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x209;
  Event_ClearFlag(0x8fc);
  t59 = Event_GetFlag(0x9eb);
  if (t59 == 0)
    goto L_0200b84a;
  t60 = Event_GetFlag(0x9ec);
  if (t60 != 0)
    goto L_0200b84a;
  Func_02003338();
L_0200b84a:
  Event_ClearFlag(0x12f);
  Func_020056bc();
L_0200b856:
}
