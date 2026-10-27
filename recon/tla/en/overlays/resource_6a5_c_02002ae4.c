// Differ 156 of 1539 (was 219): the first Func_0200588c call passes its
// speed too (0xa000, kept in r8); as in Func_02001388's draft, the first
// scene's speeds are locals (fast 0xa000, slow 0x2000, mid 0xc000) and its
// Func_0200588c calls go through Call4, and Func_020058fc's calls go through
// an inline of its own that calls it directly, as do four more sites found by
// trying each call's form. Locals for the speeds through the whole function
// are worse (185).
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02005814();
s32 Func_02005984();
s32 Object_GetById();
s32 Func_0200588c();
s32 Object_SetModeById();
s32 Func_02005914();
s32 Camera_MoveWithinMapBounds();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Func_020058c4();
s32 FieldEvent_WaitFrames();
s32 Func_020058d4();
s32 Func_020058ec();
s32 Motion_SetVarCbAndRefresh();
s32 ObjectMotion_ArmCallback();
s32 UiText_OpenMessageAtObject();
s32 Event_AskYesNoWithActorAnimation();
s32 Func_020059f4();
s32 Func_02005924();
s32 Func_020058fc();
s32 ObjectMotion_SetPositionAndReset();
s32 ObjectMotion_CommitPositionAndActivate();
s32 Func_02001ac4();
s32 ObjectMotion_SetVariantCallback();
s32 Camera_FollowActor();
s32 ObjectMotion_EnableActionAndSetCallback();
s32 Object_RefreshSelectorById();
s32 Func_02003a6c();
s32 Func_0200598c();
s32 Func_02003c58();
s32 ObjectMotion_ResetAndSetPositionInMode2();
s32 ObjectMotion_ResetAndSetPosition();
s32 Object_LinkObjectAndSetCallback();
s32 ObjectMotion_SetSpeedParameters();
s32 Event_FinishScreenTransition();
s32 Func_02005934();
s32 Func_0200581c();
extern u8 MsgWeDidItWe[];
extern u8 gData_02005ff8[];
extern u8 gData_02006088[];
extern u8 gData_02005ff0[];
extern u8 gData_020061e0[];

static __inline__ void W_Func_020058ec(s32 a0, s32 a1) {
  Func_020058ec(a0, a1);
}
static __inline__ void W_Func_020058fc(s32 a0, s32 a1, s32 a2) {
  Func_020058fc(a0, a1, a2);
}
static __inline__ void W_Func_02005914(s32 a0, s32 a1) {
  Func_02005914(a0, a1);
}
static __inline__ void W_ObjectMotion_ArmCallback(s32 a0, s32 a1, s32 a2) {
  ObjectMotion_ArmCallback(a0, a1, a2);
}
static __inline__ void W_ObjectMotion_EnableActionAndSetCallback(s32 a0,
                                                                 s32 a1) {
  ObjectMotion_EnableActionAndSetCallback(a0, a1);
}

void Func_02002ae4(void) {
  s32 fast;
  s32 slow;
  s32 mid;
  s32 t2;
  s32 t3;
  s32 t4;
  s32 t5;
  s32 t6;
  s32 t7;
  s32 t8;
  s32 t9;
  s32 t10;
  s32 t11;
  s32 t12;
  s32 t13;
  s32 t67;
  s32 t166;
  s32 t167;
  s32 t168;
  s32 t169;
  s32 t184;
  s32 t185;
  s32 t186;
  s32 t269;
  s32 t292;
  s32 t295;
  s32 t298;
  s32 t325;
  s32 t330;
  s32 t358;
  s32 t361;
  s32 t364;
  s32 t367;
  s32 t381;
  s32 t382;

  Func_02005814();
  Call1(Func_02005984, 0);
  t2 = Value1(Object_GetById, 0);
  FIELD_AT((t2 + 0x23), u8, 0) = (FIELD_AT((t2 + 0x23), u8, 0) | 1);
  t3 = Value1(Object_GetById, 1);
  FIELD_AT((t3 + 0x23), u8, 0) = (FIELD_AT((t3 + 0x23), u8, 0) | 1);
  t4 = Value1(Object_GetById, 2);
  FIELD_AT((t4 + 0x23), u8, 0) = (FIELD_AT((t4 + 0x23), u8, 0) | 1);
  t5 = Value1(Object_GetById, 3);
  FIELD_AT((t5 + 0x23), u8, 0) = (FIELD_AT((t5 + 0x23), u8, 0) | 1);
  t6 = Value1(Object_GetById, 4);
  FIELD_AT((t6 + 0x23), u8, 0) = (FIELD_AT((t6 + 0x23), u8, 0) | 1);
  t7 = Value1(Object_GetById, 5);
  FIELD_AT((t7 + 0x23), u8, 0) = (FIELD_AT((t7 + 0x23), u8, 0) | 1);
  t8 = Value1(Object_GetById, 7);
  FIELD_AT((t8 + 0x23), u8, 0) = (FIELD_AT((t8 + 0x23), u8, 0) | 1);
  t9 = Value1(Object_GetById, 6);
  FIELD_AT((t9 + 0x23), u8, 0) = (FIELD_AT((t9 + 0x23), u8, 0) | 1);
  t10 = Value1(Object_GetById, 0x1e);
  FIELD_AT((t10 + 0x23), u8, 0) = (FIELD_AT((t10 + 0x23), u8, 0) | 1);
  t11 = Value1(Object_GetById, 0x1b);
  FIELD_AT((t11 + 0x23), u8, 0) = (FIELD_AT((t11 + 0x23), u8, 0) | 1);
  t12 = Value1(Object_GetById, 0x1c);
  FIELD_AT((t12 + 0x23), u8, 0) = (FIELD_AT((t12 + 0x23), u8, 0) | 1);
  t13 = Value1(Object_GetById, 0x1d);
  FIELD_AT((t13 + 0x23), u8, 0) = (1 | FIELD_AT((t13 + 0x23), u8, 0));
  fast = 0xa000;
  Call4(Func_0200588c, 4, 0x2080000, 0x1300000, fast);
  Call4(Func_0200588c, 7, 0x2180000, 0x1200000, fast);
  slow = 0x2000;
  Call4(Func_0200588c, 0x1b, 0x1e00000, 0x1280000, slow);
  Call4(Func_0200588c, 0x1c, 0x1f00000, 0x1180000, slow);
  Call4(Func_0200588c, 5, 0x2200000, 0x1300000, fast);
  mid = 0xc000;
  Call4(Func_0200588c, 6, 0x2300000, 0x1200000, mid);
  Call4(Func_0200588c, 0x1d, 0x1e00000, 0x1680000, mid);
  Call4(Func_0200588c, 0x1e, 0x2500000, 0x1880000, 0);
  Call2(Object_SetModeById, 0x1b, 0xa);
  Call2(Object_SetModeById, 0x1c, 9);
  W_Func_02005914(0x10000, slow);
  Call4(Camera_MoveWithinMapBounds, 0x2080000, 0x680000, 0x1380000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call1(Func_020058c4, (s32)MsgWeDidItWe);
  Call2(Object_SetModeById, 7, 4);
  Call1(FieldEvent_WaitFrames, 0x21);
  Call3(Func_020058d4, 7, 0, 0);
  Call2(Object_SetModeById, 5, 3);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call3(Func_020058d4, 5, 0, 0);
  Call2(Func_020058ec, 6, 0x6000);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call3(Func_020058d4, 6, 0, 0);
  Call2(Motion_SetVarCbAndRefresh, 0x1b, 2);
  Call1(FieldEvent_WaitFrames, 5);
  Call3(Func_020058d4, 0x1b, 0, 0);
  Call2(Motion_SetVarCbAndRefresh, 0x1c, 2);
  Call1(FieldEvent_WaitFrames, 5);
  Call3(Func_020058d4, 0x1c, 0, 0);
  Call3(ObjectMotion_ArmCallback, 4, 0, 0);
  Call2(Func_020058ec, 5, 0x8000);
  Call1(FieldEvent_WaitFrames, 0x28);
  W_ObjectMotion_ArmCallback(4, fast, 0);
  Call2(Func_020058ec, 5, fast);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call2(Object_SetModeById, 5, 4);
  Call1(FieldEvent_WaitFrames, 0x21);
  Call3(Func_020058d4, 5, 0, 0);
  Call2(Motion_SetVarCbAndRefresh, 0x1b, 2);
  Call1(FieldEvent_WaitFrames, 5);
  Call3(Func_020058d4, 0x1b, 0, 0);
  Call3(ObjectMotion_ArmCallback, 4, 0xe000, 0);
  Call3(ObjectMotion_ArmCallback, 7, 0x6000, 0);
  Call3(ObjectMotion_ArmCallback, 5, 0xe000, 0);
  Call2(Func_020058ec, 6, 0x6000);
  Call1(FieldEvent_WaitFrames, 0x1e);
  Call2(Motion_SetVarCbAndRefresh, 0x1c, 2);
  UiText_OpenMessageAtObject(0x1c, 0);
  Call3(ObjectMotion_ArmCallback, 6, 0x6000, 0);
  Call2(Func_020058ec, 5, 0x8000);
  Call3(ObjectMotion_ArmCallback, 4, 0x8000, 0);
  t67 = Value2(Event_AskYesNoWithActorAnimation, 4, 0);
  if (t67 == 0) {
    Call3(Func_020058d4, 0x1d, 0, 0);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
  } else {
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
    Call3(Func_020058d4, 0x1d, 0, 0);
  }
  Call1(Func_020059f4, 0x4e);
  Call4(Camera_MoveWithinMapBounds, 0x2080000, 0x680000, 0x1600000, 1);
  Func_02005924();
  Call1(FieldEvent_WaitFrames, 0xa);
  Call1(Func_020059f4, 0x27);
  W_Func_020058fc(5, 0x100, 0x1e);
  Call3(ObjectMotion_ArmCallback, 5, 0x6000, 0);
  Call3(Func_020058d4, 5, 0, 0);
  Call4(Camera_MoveWithinMapBounds, 0x2080000, 0x680000, 0x1380000, 1);
  Call3(ObjectMotion_SetPositionAndReset, 0x1d, 0x1e0, 0x140);
  Call2(Object_SetModeById, 0x1d, 4);
  Call1(FieldEvent_WaitFrames, 0x21);
  Call3(Func_020058d4, 0x1d, 0, 0);
  Call2(Func_020058ec, 0x1d, 0);
  Call3(Func_020058d4, 0x1d, 0, 0);
  W_Func_020058fc(7, 0x109, 0);
  W_Func_020058fc(5, 0x109, 0);
  W_Func_020058fc(6, 0x109, 0);
  W_Func_020058fc(4, 0x109, 0x1e);
  Call2(Func_020058ec, 0x1d, mid);
  Call3(ObjectMotion_CommitPositionAndActivate, 0x1d, 0x14, -20);
  Call2(Func_020058ec, 0x1d, 0x8000);
  Call2(Func_02001ac4, 0x1d, 0x1b);
  W_Func_020058fc(6, 0x100, 0x1e);
  Call3(Func_020058d4, 6, 0, 0);
  Call2(Func_020058ec, 0x1d, mid);
  Call2(Func_02001ac4, 0x1d, 0x1c);
  Call3(ObjectMotion_CommitPositionAndActivate, 0x1d, -20, 0x14);
  Call2(Func_020058ec, 0x1d, 0);
  W_Func_020058fc(0x1d, 0x108, 0x1e);
  Call3(Func_020058d4, 0x1d, 0, 0);
  Call2(ObjectMotion_SetVariantCallback, 0x1b, 2);
  Call2(Motion_SetVarCbAndRefresh, 0x1c, 2);
  Call1(FieldEvent_WaitFrames, 5);
  W_Func_020058fc(5, 0x102, 0x1e);
  Call3(Func_020058d4, 5, 0, 0);
  Call2(Object_SetModeById, 0x1d, 4);
  Call1(FieldEvent_WaitFrames, 0x21);
  Call3(Func_020058d4, 0x1d, 0, 0);
  Call2(Object_SetModeById, 7, 4);
  Call1(FieldEvent_WaitFrames, 0x21);
  Call3(Func_020058d4, 7, 0, 0);
  W_Func_020058fc(0x1d, 0x109, 0x1e);
  Call3(Func_020058d4, 0x1d, 0, 0);
  Call2(Func_020058ec, 0x1b, 0x4000);
  W_Func_020058fc(0x1b, 0x107, 0x1e);
  Call3(Func_020058d4, 0x1b, 0, 0);
  Call2(Func_020058ec, 0x1c, 0x4000);
  W_Func_020058fc(0x1c, 0x107, 0x1e);
  Call3(Func_020058d4, 0x1c, 0, 0);
  W_Func_020058fc(0x1d, 0x108, 0x1e);
  Call3(Func_020058d4, 0x1d, 0, 0);
  Call3(Func_020058d4, 0x1e, 0, 0);
  Call3(ObjectMotion_ArmCallback, 0x1b, 0xe000, 0);
  Call2(Func_020058ec, 0x1c, 0x6000);
  Call1(FieldEvent_WaitFrames, 0xa);
  W_Func_020058fc(0x1c, 0x102, 0);
  W_Func_020058fc(0x1b, 0x102, 0x1e);
  Call2(Func_020058ec, 0x1d, 0);
  Call3(ObjectMotion_ArmCallback, 0x1c, 0x4000, 0);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call2(Object_SetModeById, 0x1d, 3);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call3(Func_020058d4, 0x1d, 0, 0);
  Call2(Motion_SetVarCbAndRefresh, 6, 2);
  Call1(FieldEvent_WaitFrames, 5);
  Call3(Func_020058d4, 6, 0, 0);
  W_Func_020058fc(0x1d, 0x108, 0x1e);
  Call3(Func_020058d4, 0x1d, 0, 0);
  Call3(ObjectMotion_ArmCallback, 5, 0xe000, 0);
  Call2(Func_020058ec, 6, 0x6000);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call2(Object_SetModeById, 0x1d, 3);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call3(Func_020058d4, 0x1d, 0, 0);
  Call3(ObjectMotion_ArmCallback, 5, 0x8000, 0);
  Call2(Func_020058ec, 0x1d, mid);
  Call2(Object_SetModeById, 0x1d, 3);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call3(ObjectMotion_ArmCallback, 0x1c, 0x6000, 0);
  Call2(Func_020058ec, 0x1b, 0xe000);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call2(Object_SetModeById, 0x1c, 3);
  Call2(Object_SetModeById, 0x1b, 3);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call2(Camera_FollowActor, 0x1b, 1);
  Call2(ObjectMotion_EnableActionAndSetCallback, 0x1d, (s32)gData_02005ff8);
  Call1(FieldEvent_WaitFrames, 0x1e);
  Call2(ObjectMotion_EnableActionAndSetCallback, 0x1b, (s32)gData_02005ff8);
  Call1(FieldEvent_WaitFrames, 0x1e);
  Call2(ObjectMotion_EnableActionAndSetCallback, 0x1c, (s32)gData_02005ff8);
  Call1(Object_RefreshSelectorById, 0x1d);
  t166 = Value1(Object_GetById, 0x1d);
  FIELD_AT((t166 + 0x23), u8, 0) = (FIELD_AT((t166 + 0x23), u8, 0) | 0x40);
  t167 = Value1(Object_GetById, 0x1b);
  FIELD_AT((t167 + 0x23), u8, 0) = (FIELD_AT((t167 + 0x23), u8, 0) | 0x40);
  t168 = Value1(Object_GetById, 0x1c);
  FIELD_AT((t168 + 0x23), u8, 0) = (0x40 | FIELD_AT((t168 + 0x23), u8, 0));
  t169 = Value1(Object_GetById, 0x1d);
  FIELD_AT((t169 + 0x55), u8, 0) = 2;
  Call2(ObjectMotion_EnableActionAndSetCallback, 0x1d, (s32)gData_02006088);
  Call1(Object_RefreshSelectorById, 0x1b);
  Call2(ObjectMotion_EnableActionAndSetCallback, 0x1b, (s32)gData_02006088);
  Call1(Object_RefreshSelectorById, 0x1c);
  Call2(ObjectMotion_EnableActionAndSetCallback, 0x1c, (s32)gData_02006088);
  Call1(Object_RefreshSelectorById, 0x1d);
  Call3(ObjectMotion_SetPositionAndReset, 0x1d, 0x137, 0x100);
  Call3(ObjectMotion_ArmCallback, 0x1d, 0x4000, 0);
  Call1(Object_RefreshSelectorById, 0x1b);
  Call3(ObjectMotion_SetPositionAndReset, 0x1b, 0x14c, 0x10a);
  Call3(ObjectMotion_ArmCallback, 0x1b, 0x4000, 0);
  Call1(Object_RefreshSelectorById, 0x1c);
  Call3(ObjectMotion_SetPositionAndReset, 0x1c, 0x122, 0x10a);
  Call3(ObjectMotion_ArmCallback, 0x1c, 0x4000, 0);
  t184 = Value1(Object_GetById, 0x1d);
  FIELD_AT((t184 + 0x23), u8, 0) = (FIELD_AT((t184 + 0x23), u8, 0) | 0x20);
  t185 = Value1(Object_GetById, 0x1b);
  FIELD_AT((t185 + 0x23), u8, 0) = (FIELD_AT((t185 + 0x23), u8, 0) | 0x20);
  t186 = Value1(Object_GetById, 0x1c);
  FIELD_AT((t186 + 0x23), u8, 0) = (0x20 | FIELD_AT((t186 + 0x23), u8, 0));
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call1(Func_020059f4, 0x4e);
  Call3(Func_020058d4, 0x1e, 0, 0);
  Func_0200588c(0x1e, 0x2000000, 0x1480000, 0xc000);
  Call4(Func_0200588c, 0, 0x2180000, 0x1500000, 0xc000);
  Call4(Func_0200588c, 1, 0x2180000, 0x1600000, 0xc000);
  Call4(Func_0200588c, 2, 0x2280000, 0x1600000, 0xc000);
  Call4(Func_0200588c, 3, 0x2280000, 0x1700000, 0xc000);
  Call3(ObjectMotion_ArmCallback, 4, 0x4000, 0);
  Call3(ObjectMotion_ArmCallback, 5, 0x4000, 0);
  Call3(ObjectMotion_ArmCallback, 6, 0x4000, 0);
  Call3(ObjectMotion_ArmCallback, 7, 0x4000, 0);
  Call1(Func_02003a6c, 3);
  Func_0200598c();
  Call3(Func_020058d4, 0, 0, 0);
  Func_020058fc(4, 0x102, 0x1e);
  W_Func_020058fc(5, 0x109, 0x1e);
  Call3(Func_020058d4, 5, 0, 0);
  Call2(Func_020058ec, 1, 0x8000);
  Call2(Func_020058ec, 1, 0xc000);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call1(Func_02003c58, 0);
  W_Func_020058fc(1, 0x103, 0x1e);
  Call3(Func_020058d4, 1, 0, 0);
  Call2(Func_020058ec, 0, 0xa000);
  Call2(Motion_SetVarCbAndRefresh, 0, 2);
  Call1(FieldEvent_WaitFrames, 5);
  Call3(Func_020058d4, 0, 0, 0);
  W_Func_020058fc(0x1e, 0x109, 0x1e);
  Call2(Func_020058ec, 0x1e, 0x2000);
  Call3(Func_020058d4, 0x1e, 0, 0);
  W_Func_020058fc(1, 0x107, 0x1e);
  Call3(Func_020058d4, 1, 0, 0);
  W_Func_020058fc(2, 0x109, 0x1e);
  Call3(ObjectMotion_ResetAndSetPositionInMode2, 2, 0x228, 0x140);
  Func_020058ec(2, 0xc000);
  Call2(Func_020058ec, 2, 0x6000);
  Call2(Func_020058ec, 2, 0x8000);
  Call2(Object_SetModeById, 2, 4);
  Call1(FieldEvent_WaitFrames, 0x21);
  Call2(Motion_SetVarCbAndRefresh, 2, 2);
  Call3(Func_020058d4, 2, 0, 0);
  Call2(Motion_SetVarCbAndRefresh, 0, 2);
  Call2(Func_020058ec, 0, 0xe000);
  Call1(FieldEvent_WaitFrames, 5);
  Call3(Func_020058d4, 0, 0, 0);
  Call2(Func_020058ec, 2, 0x6000);
  Call2(Motion_SetVarCbAndRefresh, 2, 2);
  Call1(FieldEvent_WaitFrames, 5);
  Call3(Func_020058d4, 2, 0, 0);
  Call3(ObjectMotion_ArmCallback, 5, 0xe000, 0);
  Call3(ObjectMotion_ArmCallback, 6, 0x6000, 0);
  Call3(ObjectMotion_ArmCallback, 7, 0x6000, 0);
  Call3(ObjectMotion_ArmCallback, 4, 0xe000, 0);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call2(Func_020058ec, 0x1e, 0);
  Call2(Object_SetModeById, 0x1e, 3);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call2(Func_020058ec, 0x1e, 0);
  Call3(Func_020058d4, 0x1e, 0, 0);
  Call3(ObjectMotion_ArmCallback, 4, 0x4000, 0);
  Call3(ObjectMotion_ArmCallback, 5, 0x4000, 0);
  Call3(ObjectMotion_ArmCallback, 7, 0x4000, 0);
  Call3(ObjectMotion_ArmCallback, 6, 0x4000, 0);
  Call2(Func_020058ec, 0, 0x4000);
  Call1(FieldEvent_WaitFrames, 0x3c);
  W_Func_020058ec(1, 0xc000);
  Call2(Object_SetModeById, 3, 3);
  Call2(Object_SetModeById, 1, 3);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call2(Func_020058ec, 0, 0xe000);
  Call1(FieldEvent_WaitFrames, 0x3c);
  Call2(Object_SetModeById, 6, 3);
  Call2(Object_SetModeById, 7, 3);
  Call2(Object_SetModeById, 5, 3);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call2(Func_020058ec, 0, 0xc000);
  Call1(FieldEvent_WaitFrames, 0x1e);
  Call2(Func_020058ec, 4, 0x4000);
  t269 = Value2(Event_AskYesNoWithActorAnimation, 4, 0);
  if (t269 == 0) {
    Call2(Object_SetModeById, 0, 3);
    Call1(FieldEvent_WaitFrames, 0x14);
    Call1(FieldEvent_WaitFrames, 0xa);
    Call3(Func_020058d4, 0, 0, 0);
    Call2(Object_SetModeById, 0, 3);
    Call1(FieldEvent_WaitFrames, 0x14);
    Call1(FieldEvent_WaitFrames, 0xa);
  } else {
    W_Func_020058fc(0, 0x107, 0x1e);
    Call2(Object_SetModeById, 5, 4);
    Call1(FieldEvent_WaitFrames, 0x21);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 2);
    Call2(Func_020058ec, 5, 0x8000);
    Call3(Func_020058d4, 5, 0, 0);
    Call2(Func_020058ec, 5, 0x4000);
    Call1(FieldEvent_WaitFrames, 0xa);
    Call2(Motion_SetVarCbAndRefresh, 5, 2);
    Call1(FieldEvent_WaitFrames, 5);
    Call3(Func_020058d4, 5, 0, 0);
    Call2(Object_SetModeById, 0, 3);
    Call1(FieldEvent_WaitFrames, 0x14);
    Call1(FieldEvent_WaitFrames, 0xa);
    Call3(Func_020058d4, 0, 0, 0);
  }
  Call2(Object_SetModeById, 2, 2);
  t292 = Value1(Object_GetById, 0);
  if (t292 != 0) {
    Call3(ObjectMotion_ResetAndSetPosition, 2, FIELD_AT(t292, s16, 10),
          FIELD_AT(t292, s16, 18));
  }
  Call2(Object_SetModeById, 3, 2);
  t295 = Value1(Object_GetById, 0);
  if (t295 != 0) {
    Call3(ObjectMotion_ResetAndSetPosition, 3, FIELD_AT(t295, s16, 10),
          FIELD_AT(t295, s16, 18));
  }
  Call2(Object_SetModeById, 1, 2);
  t298 = Value1(Object_GetById, 0);
  if (t298 != 0) {
    Call3(ObjectMotion_ResetAndSetPosition, 1, FIELD_AT(t298, s16, 10),
          FIELD_AT(t298, s16, 18));
  }
  Call2(ObjectMotion_EnableActionAndSetCallback, 2, (s32)gData_02005ff0);
  W_ObjectMotion_EnableActionAndSetCallback(3, (s32)gData_02005ff0);
  Call2(ObjectMotion_EnableActionAndSetCallback, 1, (s32)gData_02005ff0);
  Call1(Object_RefreshSelectorById, 2);
  Call1(Object_RefreshSelectorById, 3);
  Call1(Object_RefreshSelectorById, 1);
  Call2(Camera_FollowActor, 0, 1);
  Call2(Object_LinkObjectAndSetCallback, 4, 0);
  Call2(Object_LinkObjectAndSetCallback, 7, 0);
  Call2(Object_LinkObjectAndSetCallback, 5, 0);
  Call2(Object_LinkObjectAndSetCallback, 6, 0);
  Call2(Object_LinkObjectAndSetCallback, 0x1e, 0);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call3(ObjectMotion_SetSpeedParameters, 0, 0x18000, 0xc000);
  Call3(ObjectMotion_SetPositionAndReset, 0, 0x21f, 0x16b);
  Call2(Func_020058ec, 0, 0xc000);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call2(Motion_SetVarCbAndRefresh, 0, 2);
  Call1(FieldEvent_WaitFrames, 5);
  Call3(Func_020058d4, 0, 0, 0);
  Call3(ObjectMotion_SetPositionAndReset, 0, 0x23d, 0x166);
  Call3(ObjectMotion_SetPositionAndReset, 0, 0x248, 0x12c);
  Call3(ObjectMotion_SetPositionAndReset, 0, 0x248, 0xfc);
  Call3(ObjectMotion_SetPositionAndReset, 0, 0x268, 0xf7);
  Call3(ObjectMotion_SetPositionAndReset, 0, 0x269, 0x138);
  t325 = Value1(Object_GetById, 0);
  FIELD_AT((t325 + 0x55), u8, 0) = 2;
  Call2(ObjectMotion_EnableActionAndSetCallback, 0, (s32)gData_020061e0);
  Call1(Object_RefreshSelectorById, 0);
  Call2(Func_020058ec, 0, 0x4000);
  Call1(FieldEvent_WaitFrames, 0x1e);
  t330 = Value1(Object_GetById, 0);
  FIELD_AT((t330 + 0x23), u8, 0) = (0x20 | FIELD_AT((t330 + 0x23), u8, 0));
  Call3(ObjectMotion_ArmCallback, 4, 0, 0);
  Call3(ObjectMotion_ArmCallback, 7, 0, 0);
  Call3(ObjectMotion_ArmCallback, 5, 0, 0);
  Call3(ObjectMotion_ArmCallback, 6, 0, 0);
  Call3(ObjectMotion_ArmCallback, 0x1e, 0, 0);
  Call1(Func_02003a6c, 1);
  Call1(FieldEvent_WaitFrames, 0x64);
  Call3(ObjectMotion_ArmCallback, 4, 0x8000, 0);
  Call3(ObjectMotion_ArmCallback, 7, 0x8000, 0);
  Call3(ObjectMotion_ArmCallback, 5, 0x8000, 0);
  Call3(ObjectMotion_ArmCallback, 6, 0x8000, 0);
  Call3(ObjectMotion_ArmCallback, 0x1e, 0x8000, 0);
  Call1(Func_02003c58, 1);
  Call1(FieldEvent_WaitFrames, 0x3c);
  Call2(Camera_FollowActor, 4, 1);
  Func_02005924();
  Call1(FieldEvent_WaitFrames, 0x3c);
  Call2(Func_020058ec, 4, 0);
  Call1(FieldEvent_WaitFrames, 0x3c);
  Call2(Object_SetModeById, 5, 3);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call2(Func_020058ec, 4, 0xe000);
  Call2(Object_SetModeById, 7, 3);
  Call2(Object_SetModeById, 6, 3);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call2(Object_SetModeById, 7, 2);
  t358 = Value1(Object_GetById, 4);
  if (t358 != 0) {
    Call3(ObjectMotion_ResetAndSetPosition, 7, FIELD_AT(t358, s16, 10),
          FIELD_AT(t358, s16, 18));
  }
  Call2(Object_SetModeById, 6, 2);
  t361 = Value1(Object_GetById, 4);
  if (t361 != 0) {
    Call3(ObjectMotion_ResetAndSetPosition, 6, FIELD_AT(t361, s16, 10),
          FIELD_AT(t361, s16, 18));
  }
  Call2(Object_SetModeById, 5, 2);
  t364 = Value1(Object_GetById, 4);
  if (t364 != 0) {
    Call3(ObjectMotion_ResetAndSetPosition, 5, FIELD_AT(t364, s16, 10),
          FIELD_AT(t364, s16, 18));
  }
  Call2(Object_SetModeById, 0x1e, 2);
  t367 = Value1(Object_GetById, 4);
  if (t367 != 0) {
    Call3(ObjectMotion_ResetAndSetPosition, 0x1e, FIELD_AT(t367, s16, 10),
          FIELD_AT(t367, s16, 18));
  }
  Call2(ObjectMotion_EnableActionAndSetCallback, 7, (s32)gData_02005ff0);
  Call2(ObjectMotion_EnableActionAndSetCallback, 6, (s32)gData_02005ff0);
  Call2(ObjectMotion_EnableActionAndSetCallback, 5, (s32)gData_02005ff0);
  Call2(ObjectMotion_EnableActionAndSetCallback, 0x1e, (s32)gData_02005ff0);
  Call1(Object_RefreshSelectorById, 7);
  Call1(Object_RefreshSelectorById, 6);
  Call1(Object_RefreshSelectorById, 5);
  Call1(Object_RefreshSelectorById, 0x1e);
  Call2(Camera_FollowActor, 4, 1);
  Call3(ObjectMotion_CommitPositionAndActivate, 4, -16, 0);
  Call2(ObjectMotion_EnableActionAndSetCallback, 4, (s32)gData_02005ff8);
  Call1(Object_RefreshSelectorById, 4);
  t381 = Value1(Object_GetById, 4);
  FIELD_AT((t381 + 0x23), u8, 0) = (FIELD_AT((t381 + 0x23), u8, 0) | 0x40);
  t382 = Value1(Object_GetById, 0x1d);
  FIELD_AT((t382 + 0x55), u8, 0) = 2;
  Call2(ObjectMotion_EnableActionAndSetCallback, 4, (s32)gData_02006088);
  Call1(Object_RefreshSelectorById, 4);
  Call3(ObjectMotion_SetPositionAndReset, 4, 0x137, 0x100);
  Call2(Func_020058ec, 4, 0x4000);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call1(Func_02003a6c, 2);
  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x200;
  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1b4), s32, 0) = 0x18;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Call1(Func_02005934, 1);
  Func_0200581c();
}
