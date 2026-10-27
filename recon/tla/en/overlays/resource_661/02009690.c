// Near miss, differ 43 of 478 (1,248 bytes), lifted mechanically from the
// listing and not yet worked by hand: call sites through call.h's layer or
// direct, the rest is argument order and the function's own loads and stores.
#include "types.h"
#include "call.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 FieldEvent_WaitTicks();
s32 Func_02004040();
s32 Func_02004048();
s32 Func_02004090();
s32 Func_02004098();
s32 Func_020040c8();
s32 Func_020040d0();
s32 Func_02004100();
s32 Func_02004118();
s32 Func_02004128();
s32 Func_02004130();
s32 Func_020041b8();
s32 GameFlag_SetBit();
s32 Camera_MoveWithinMapBounds();
s32 Motion_SetModeAndWaitAnimation();
s32 ObjectMotion_ArmCallback();
s32 ObjectMotion_EnableActionAndResetMotion();
s32 ObjectMotion_EnableActionAndSetCallback();
s32 ObjectMotion_Launch();
s32 ObjectMotion_SetSpeedParameters();
s32 Object_LinkObjectAndSetCallback();
s32 Object_SetActionCallbackAndRefreshById();
s32 Object_SetModeById();
s32 WaitTicks();
extern u8 gData_0200468c[];
extern u8 gData_020046dc[];
extern u8 gData_02004718[];
extern u8 MsgHeyLookUp[];
extern u8 gPartyState[];

void Func_02001690(void) {
  s32 v5;
  s32 v6;
  s32 t1;

  Func_02004040();
  Call1(Func_020041b8, 0);
  Call1(Func_020040c8, ((s32)MsgHeyLookUp));
  Call1(ObjectMotion_EnableActionAndResetMotion, 9);
  Call1(WaitTicks, 1);
  {
    s32 a1 = 0x100;
    s32 a2 = 0;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_02004100(9, a1, a2);
  }
  Call3(ObjectMotion_ArmCallback, 9, 0, 20);
  Call2(Func_020040d0, 9, 0);
  Call1(ObjectMotion_EnableActionAndResetMotion, 8);
  Call1(WaitTicks, 1);
  Call3(ObjectMotion_ArmCallback, 8, 0, 0);
  Call3(ObjectMotion_ArmCallback, 10, 0, 0);
  Call3(ObjectMotion_ArmCallback, 11, 0xd000, 0);
  Call3(ObjectMotion_ArmCallback, 12, 0xd000, 0);
  Call3(ObjectMotion_ArmCallback, 13, 0xd000, 0);
  Call3(ObjectMotion_ArmCallback, 14, 0xd000, 0);
  v6 = ((s32)gPartyState);
  v6 = (v6 + 0x214);
  Call3(Func_02004100, *(s32 *)(v6), 0x100, 0);
  Call3(ObjectMotion_ArmCallback, *(s32 *)(v6), 0x8000, 0);
  t1 = Func_02004130();
  *(u8 *)((t1 + 85)) = 0;
  Call2(Func_02004118, 0x19999, 0x3333);
  Call4(Camera_MoveWithinMapBounds, 0xd40000, 0, 0xd40000, 1);
  Func_02004128();
  Call2(Func_020040d0, 12, 0);
  Call3(ObjectMotion_ArmCallback, 11, 0, 10);
  Call2(Func_020040d0, 11, 0);
  Call3(ObjectMotion_ArmCallback, 13, 0xb000, 0);
  Call3(ObjectMotion_ArmCallback, 14, 0x5000, 20);
  Call3(ObjectMotion_Launch, 14, 4, 40);
  Call2(Motion_SetModeAndWaitAnimation, 13, 3);
  Call2(Motion_SetModeAndWaitAnimation, 14, 3);
  Call2(Object_LinkObjectAndSetCallback, 8, 14);
  Call2(Object_LinkObjectAndSetCallback, 9, 14);
  Call2(Object_LinkObjectAndSetCallback, 10, 14);
  Call2(Object_LinkObjectAndSetCallback, 11, 14);
  Call2(Object_LinkObjectAndSetCallback, 12, 14);
  Call2(Object_LinkObjectAndSetCallback, 13, 14);
  Call3(ObjectMotion_SetSpeedParameters, 13, 0x10000, 0x8000);
  Call2(ObjectMotion_EnableActionAndSetCallback, 14, ((s32)gData_0200468c));
  Call2(Func_02004118, 0xcccc, 0x1999);
  Call4(Camera_MoveWithinMapBounds, 0xf00000, -1, 0x1040000, 1);
  Func_02004128();
  Call1(ObjectMotion_EnableActionAndResetMotion, 8);
  Call1(ObjectMotion_EnableActionAndResetMotion, 9);
  Call1(ObjectMotion_EnableActionAndResetMotion, 10);
  Call1(ObjectMotion_EnableActionAndResetMotion, 11);
  Call1(ObjectMotion_EnableActionAndResetMotion, 12);
  Call1(ObjectMotion_EnableActionAndResetMotion, 13);
  Call2(Func_02004118, 0x26666, 0x4ccc);
  Call4(Camera_MoveWithinMapBounds, 0xd40000, -1, 0xd40000, 1);
  Func_02004128();
  Call1(FieldEvent_WaitTicks, 20);
  Call3(ObjectMotion_ArmCallback, 8, 0xd000, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0x5000, 20);
  Call2(Func_020040d0, 9, 0);
  Call2(Motion_SetModeAndWaitAnimation, 8, 3);
  Call2(Func_020040d0, 8, 0);
  Call3(ObjectMotion_Launch, 10, 4, 0);
  Call3(ObjectMotion_ArmCallback, 10, 0, 0);
  Call2(Object_SetModeById, 10, 4);
  Call2(Func_020040d0, 10, 0);
  Call3(ObjectMotion_ArmCallback, 8, 0x8000, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0x8000, 0);
  {
    s32 a1 = 0x102;
    s32 a2 = 0;
    /* FAKEMATCH: the native loads the later arguments before the first; the
     * plain call loads the first earlier. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Func_02004100(8, a1, a2);
  }
  Func_02004100(9, 0x102, 80);
  Call2(Func_020040d0, 14, 0);
  Call3(ObjectMotion_ArmCallback, 8, 0xd000, 0);
  Call3(ObjectMotion_ArmCallback, 10, 0x3000, 0);
  Call3(ObjectMotion_ArmCallback, 11, 0xd000, 0);
  Call3(ObjectMotion_ArmCallback, 12, 0, 0);
  Call3(ObjectMotion_ArmCallback, 13, 0x8000, 20);
  Call2(Object_SetModeById, 8, 3);
  Call2(Object_SetModeById, 9, 3);
  Call2(Object_SetModeById, 10, 3);
  Call2(Object_SetModeById, 11, 3);
  Call2(Object_SetModeById, 12, 3);
  Call2(Motion_SetModeAndWaitAnimation, 13, 3);
  Call3(ObjectMotion_SetSpeedParameters, 8, 0x19999, 0xcccc);
  Call3(ObjectMotion_SetSpeedParameters, 9, 0x19999, 0xcccc);
  Call3(ObjectMotion_SetSpeedParameters, 10, 0x19999, 0xcccc);
  Call3(ObjectMotion_SetSpeedParameters, 11, 0x19999, 0xcccc);
  Call3(ObjectMotion_SetSpeedParameters, 12, 0x19999, 0xcccc);
  Call3(ObjectMotion_SetSpeedParameters, 13, 0x19999, 0xcccc);
  Call2(Object_LinkObjectAndSetCallback, *(s32 *)(v6), 13);
  v5 = ((s32)gData_020046dc);
  Call2(ObjectMotion_EnableActionAndSetCallback, 13, v5);
  Call1(FieldEvent_WaitTicks, 10);
  Call2(ObjectMotion_EnableActionAndSetCallback, 12, v5);
  Call2(Func_02004118, 0x6666, 0xccc);
  Call4(Camera_MoveWithinMapBounds, 0xf00000, -1, 0x1180000, 1);
  Call2(ObjectMotion_EnableActionAndSetCallback, 8, v5);
  Call1(FieldEvent_WaitTicks, 10);
  Call2(ObjectMotion_EnableActionAndSetCallback, 9, v5);
  v5 = ((s32)gData_02004718);
  Call2(ObjectMotion_EnableActionAndSetCallback, 10, v5);
  Call1(FieldEvent_WaitTicks, 20);
  Call2(Object_SetActionCallbackAndRefreshById, 11, v5);
  Call1(ObjectMotion_EnableActionAndResetMotion, *(s32 *)(v6));
  Call1(WaitTicks, 1);
  Call3(Func_02004090, 8, 0, 0);
  Call3(Func_02004090, 9, 0, 0);
  Call3(Func_02004090, 10, 0, 0);
  Call3(Func_02004090, 11, 0, 0);
  Call3(Func_02004090, 12, 0, 0);
  Call3(Func_02004090, 13, 0, 0);
  v6 = 176;
  Call3(Func_02004090, 14, 0, 0);
  v6 = (v6 << 8);
  Call4(Func_02004098, 8, 0x2080000, 0xf20000, v6);
  goto L_02009abc;
L_02009abc:;
  Func_02004098();
  {
    s32 a3 = 0x3000;
    /* FAKEMATCH: the native loads the last argument first; the plain call loads
     * another first. */
    asm volatile("" : "+l"(a3));
    Call4(Func_02004098, 10, 0x1b30000, 0xe80000, a3);
  }
  v5 = 208;
  {
    s32 a3 = 0x5000;
    /* FAKEMATCH: the native loads the last argument first; the plain call loads
     * another first. */
    asm volatile("" : "+l"(a3));
    Call4(Func_02004098, 11, 0x1d30000, 0xf80000, a3);
  }
  v5 = (v5 << 8);
  Call4(Func_02004098, 12, 0x1ce0000, 0x1540000, v5);
  Call4(Func_02004098, 13, 0x1e20000, 0x1540000, v5);
  Call4(Func_02004098, 14, 0x1e90000, 0x1400000, v6);
  Call2(Object_SetModeById, 8, 1);
  Call2(Object_SetModeById, 9, 1);
  Call2(Object_SetModeById, 10, 1);
  Call2(Object_SetModeById, 11, 1);
  Call2(Object_SetModeById, 12, 1);
  Call2(Object_SetModeById, 13, 1);
  Call2(Object_SetModeById, 14, 1);
  Call1(GameFlag_SetBit, 0x8f0);
  Call1(GameFlag_SetBit, 0x88f);
  Func_02004048();
}
