#include "jupiter_lighthouse_room_m.h"

extern u32 gData_02005548[];

void Func_0200484c(s32 id, s32 x, s32 z, s32 angle);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);

extern const u8 MsgAhhhh2[];
extern u32 gData_020056ec[];
extern u32 gData_02005748[];
extern u32 gData_0200578c[];
extern u32 gData_020057d0[];
extern u32 gData_02005800[];
extern u32 gData_0200d830[];
extern u32 gData_02005860[];
extern u32 gData_02005890[];
extern u32 gData_020058d8[];
extern u32 gData_02005920[];
extern u32 gData_02005950[];
extern u32 gData_020049cc[];
extern u32 gData_020049dc[];
extern u32 gData_020049ec[];
extern u32 gData_020049fc[];
extern u32 gData_02004a18[];
extern u32 gData_02004a34[];
extern u32 gData_02004a50[];
extern u32 gData_0200599c[];

void Func_020047d4(void);
void Func_020047dc(void);
void Func_0200488c(const u8 *message);
void Func_0200489c(s32 actor, s32 a, s32 b);
void Func_020048b4(s32 actor, s32 a);
void Func_020048bc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h,
                   s32 i, s32 j, s32 k);
struct ObjectRuntime *Func_020048cc(s32 actor, s32 emote, s32 frames);
void Func_020048d4(s32 actor, s32 a);
void Func_020048e4(s32 a, s32 b);
void Func_020048f4(void);
void Func_0200494c(s32 a);
void Func_02004964(s32 actor, s32 x, s32 z, s32 angle);
void Func_020049c4(s32 cue);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void ObjectMotion_EnableActionAndResetMotion(s32 id);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_OffsetPositionAndReset(s32 id, s32 dx, s32 dz);
void ObjectMotion_SetVariantCallback(s32 id, s32 variant);
void ObjectMotion_SetAngleToward(s32 id, s32 target, s32 flags);
void ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Camera_FollowActor(s32 id, s32 mode);
void Object_LinkPair(s32 a, s32 b, s32 c);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 d);
s32 UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);

static __inline__ void I_Func_020048bc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                       s32 g, s32 h, s32 i, s32 j, s32 k) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 7 sites, 152 of 1568 instructions differ in tla-en. */
  Func_020048bc(a, b, c, d, e, f, g, h, i, j, k);
}

static __inline__ void I_ObjectMotion_ArmCallback(s32 id, s32 angle,
                                                  s32 flags) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 15 sites, 73 of 1568 instructions differ in tla-en. */
  ObjectMotion_ArmCallback(id, angle, flags);
}

static __inline__ void I_Func_02004964(s32 actor, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 4 sites, 16 of 1568 instructions differ in tla-en. */
  Func_02004964(actor, x, z, angle);
}

static __inline__ struct ObjectRuntime *I_Func_020048cc(s32 actor, s32 emote,
                                                        s32 frames) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 42 sites, 171 of 1568 instructions differ in tla-en. */
  return Func_020048cc(actor, emote, frames);
}

static __inline__ void I_ObjectMotion_SetSpeedParameters(s32 actor, s32 speed,
                                                         s32 acceleration) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 14 sites, 78 of 1568 instructions differ in tla-en. */
  ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void I_Motion_CamBounds(s32 x, s32 y, s32 z, s32 d) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 6 sites, 25 of 1568 instructions differ in tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, d);
}

static __inline__ void I_Func_0200484c(s32 id, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 3 sites, 14 of 1568 instructions differ in tla-en. */
  Func_0200484c(id, x, z, angle);
}

static __inline__ void I_Func_020048b4(s32 actor, s32 a) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 20 sites, 85 of 1568 instructions differ in tla-en. */
  Func_020048b4(actor, a);
}

static __inline__ void I_Func_020048d4(s32 actor, s32 a) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its one site, 16 of 1568 instructions differ in tla-en. */
  Func_020048d4(actor, a);
}

static __inline__ void
I_ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 6 sites, 26 of 1568 instructions differ in tla-en. */
  ObjectMotion_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void I_ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x,
                                                                 s32 z) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 6 sites, 25 of 1568 instructions differ in tla-en. */
  ObjectMotion_ResetAndSetPositionInMode2(id, x, z);
}

static __inline__ void I_ObjectMotion_CommitPositionAndActivate(s32 id, s32 x,
                                                                s32 z) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its one site, 7 of 1568 instructions differ in tla-en. */
  ObjectMotion_CommitPositionAndActivate(id, x, z);
}

// The scene in which actor 4 meets 5, 6, 7 and 28 and the group is placed for
// the arrival; arg selects one of its two layouts.
void Func_02000cac(s32 arg) {
  struct ObjectRuntime *object;
  s32 angle;

  Func_020047d4();
  Func_0200494c(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_0200488c(MsgAhhhh2);
  Func_020049c4(124);
  FieldEvent_WaitFrames(26);
  Func_020049c4(217);
  ObjectMotion_EnableActionAndSetCallback(4, gData_020056ec);
  if (arg != 0) {
    I_Func_020048bc(3, 20, 9, 15, 8, 1, 20, 2, 15, 1, 25);
    Func_020049c4(310);
    FieldEvent_WaitFrames(30);
    Func_020049c4(144);
    I_Func_020048bc(3, 21, 9, 16, 8, 1, 21, 2, 16, 1, 0);
    I_Func_020048bc(0, 20, 9, 15, 8, 2, 20, 2, 15, 1, 0);
  } else {
    I_Func_020048bc(3, 5, 9, 0, 8, 1, 5, 2, 0, 1, 25);
    Func_020049c4(310);
    FieldEvent_WaitFrames(30);
    Func_020049c4(144);
    I_Func_020048bc(3, 5, 9, 0, 8, 1, 5, 2, 0, 1, 0);
    I_Func_020048bc(0, 5, 9, 0, 8, 2, 5, 2, 0, 1, 0);
  }
  ObjectMotion_EnableActionAndResetMotion(4);
  I_ObjectMotion_ArmCallback(4, 0x4000, 0);
  I_Func_02004964(7, 16, 0, 0x8000);
  I_Func_02004964(28, -16, 0, 0);
  I_Func_02004964(6, 16, 24, 0x8000);
  I_Func_02004964(5, -16, 24, 0);
  Camera_FollowActor(5, 1);
  ObjectMotion_CommitCurrentPositionAndActivate(5);
  I_Func_020048cc(28, 0x102, 30);
  Func_0200489c(28, 0, 5);
  I_ObjectMotion_ArmCallback(5, 0xc000, 0);
  I_ObjectMotion_ArmCallback(6, 0xb000, 0);
  I_Func_020048cc(7, 0x101, 30);
  I_ObjectMotion_ArmCallback(5, 0xd000, 0);
  UiText_OpenMessageAtObject(7, 0);
  I_ObjectMotion_ArmCallback(4, 0, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Object_SetModeById(28, 3);
    FieldEvent_WaitFrames(20);
    FieldEvent_WaitFrames(10);
    Func_0200489c(28, 0, 5);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    Object_SetModeById(28, 4);
    FieldEvent_WaitFrames(33);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Func_0200489c(28, 0, 5);
  }
  Motion_SetVarCbAndRefresh(5, 2);
  I_ObjectMotion_ArmCallback(4, 0x4000, 0);
  FieldEvent_WaitFrames(5);
  Func_0200489c(5, 0, 5);
  I_Func_020048cc(6, 0x101, 30);
  Func_0200489c(6, 0, 5);
  I_Func_020048cc(4, 0x102, 30);
  I_ObjectMotion_SetSpeedParameters(4, 0x00013333, 0x9999);
  ObjectMotion_EnableActionAndSetCallback(4, arg != 0 ? gData_02005748
                                                      : gData_0200578c);
  FieldEvent_WaitFrames(30);
  I_ObjectMotion_ArmCallback(6, 0x8000, 0);
  I_ObjectMotion_ArmCallback(5, 0, 0);
  I_ObjectMotion_ArmCallback(28, 0, 0);
  I_ObjectMotion_ArmCallback(7, 0x8000, 0);
  FieldEvent_WaitFrames(10);
  Object_SetModeById(6, 3);
  Object_SetModeById(7, 3);
  Object_SetModeById(5, 3);
  Object_SetModeById(28, 3);
  FieldEvent_WaitFrames(30);
  I_ObjectMotion_SetSpeedParameters(5, 0x10000, 0x8000);
  I_ObjectMotion_SetSpeedParameters(7, 0x10000, 0x8000);
  I_ObjectMotion_SetSpeedParameters(6, 0x10000, 0x8000);
  I_ObjectMotion_SetSpeedParameters(28, 0xcccc, 0x6666);
  ObjectMotion_EnableActionAndSetCallback(28, arg != 0 ? gData_020057d0
                                                       : gData_02005800);
  ObjectMotion_EnableActionAndSetCallback(7, arg != 0 ? gData_0200d830
                                                      : gData_02005860);
  ObjectMotion_EnableActionAndSetCallback(5, arg != 0 ? gData_02005890
                                                      : gData_020058d8);
  ObjectMotion_EnableActionAndSetCallback(6, arg != 0 ? gData_02005920
                                                      : gData_02005950);
  Func_020048e4(0x8000, 0x1000);
  if (arg != 0) {
    I_Motion_CamBounds(0x02a50000, 0xffc00000, 0x011f0000, 1);
    Func_020048f4();
  } else {
    I_Motion_CamBounds(0x03b60000, 0xffc00000, 0x011f0000, 1);
    Func_020048f4();
  }
  Object_RefreshSelectorById(4);
  Object_RefreshSelectorById(7);
  Object_RefreshSelectorById(5);
  Object_RefreshSelectorById(6);
  Object_RefreshSelectorById(28);
  FieldEvent_WaitFrames(20);
  angle = 0x2000;
  I_Func_0200484c(2, 0x02e50000, 0xe80000, angle);
  Func_020048e4(0x40000, 0x8000);
  I_Motion_CamBounds(0x3200000, 0, 0x10e0000, 1);
  Func_020048f4();
  Func_020049c4(39);
  FieldEvent_WaitFrames(50);
  Motion_SetVarCbAndRefresh(0, 2);
  Func_0200489c(0, 0, 5);
  Object_SetModeById(3, 0);
  I_Func_020048b4(3, 0xa000);
  Func_0200489c(3, 0, 5);
  I_Func_020048b4(3, angle);
  Object_SetModeById(3, 9);
  Func_0200489c(3, 0, 5);
  Motion_SetVarCbAndRefresh(1, 2);
  FieldEvent_WaitFrames(5);
  Func_0200489c(1, 0, 5);
  ObjectMotion_SetVariantCallback(3, 3);
  I_Func_020048d4(3, 258);
  FieldEvent_WaitFrames(40);
  Func_0200489c(3, 0, 5);
  Motion_SetVarCbAndRefresh(1, 2);
  FieldEvent_WaitFrames(5);
  Func_0200489c(1, 0, 5);
  Object_GetById(3)->actionFlags &= 0xfe;
  I_ObjectMotion_SetSpeedParameters(3, 0x1999, 0xccc);
  I_ObjectMotion_OffsetPositionAndResetMotion(3, 4, 2);
  ObjectMotion_CommitCurrentPositionAndActivate(3);
  Object_SetModeById(3, 9);
  Motion_SetVarCbAndRefresh(3, 2);
  I_ObjectMotion_OffsetPositionAndResetMotion(3, -2, 2);
  ObjectMotion_CommitCurrentPositionAndActivate(3);
  Object_SetModeById(3, 9);
  Motion_SetVarCbAndRefresh(3, 2);
  I_ObjectMotion_OffsetPositionAndResetMotion(3, 2, 1);
  ObjectMotion_CommitCurrentPositionAndActivate(3);
  Object_SetModeById(3, 9);
  Motion_SetVarCbAndRefresh(3, 2);
  I_ObjectMotion_OffsetPositionAndResetMotion(3, -2, -1);
  ObjectMotion_CommitCurrentPositionAndActivate(3);
  Object_SetModeById(3, 9);
  Motion_SetVarCbAndRefresh(3, 2);
  FieldEvent_WaitFrames(40);
  I_ObjectMotion_OffsetPositionAndResetMotion(3, 0, -2);
  ObjectMotion_CommitCurrentPositionAndActivate(3);
  I_ObjectMotion_SetSpeedParameters(3, 0x20000, 0x10000);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    Object_SetModeById(3, 4);
  } while (0);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_WaitFrames(33);
  } while (0);
  Func_0200489c(3, 0, 5);
  Object_GetById(3)->actionFlags |= 1;
  I_ObjectMotion_OffsetPositionAndResetMotion(3, -2, -8);
  ObjectMotion_CommitCurrentPositionAndActivate(3);
  Object_SetModeById(3, 1);
  I_Func_020048b4(3, 0xa000);
  FieldEvent_WaitFrames(10);
  Func_0200489c(3, 0, 5);
  Object_SetModeById(0, 1);
  Object_SetModeById(2, 1);
  Object_LinkPair(0, 2, 0);
  FieldEvent_WaitFrames(10);
  I_Func_020048cc(0, 0x102, 30);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    Func_0200489c(0, 0, 5);
  } while (0);
  Object_SetModeById(2, 3);
  FieldEvent_WaitFrames(20);
  FieldEvent_WaitFrames(10);
  ObjectMotion_EnableActionAndSetCallback(3, gData_02005548);
  I_Func_0200484c(26, 0x02990000, 0x800000, angle);
  I_Func_0200484c(27, 0x02810000, 0x800000, angle);
  Func_020049c4(78);
  Func_0200489c(26, 0, 5);
  I_Func_020048cc(0, 0x100, 0);
  I_Func_020048cc(2, 0x100, 30);
  I_ObjectMotion_ArmCallback(0, 0xa000, 0);
  I_Func_020048b4(2, 0xa000);
  FieldEvent_WaitFrames(10);
  I_Motion_CamBounds(0x2d00000, 0, 0xd20000, 1);
  I_ObjectMotion_SetSpeedParameters(26, 0x4000, angle);
  I_ObjectMotion_SetSpeedParameters(27, 0x4000, angle);
  I_ObjectMotion_ResetAndSetPositionInMode2(26, 714, 193);
  I_ObjectMotion_ResetAndSetPositionInMode2(27, 698, 209);
  ObjectMotion_EnableActionAndSetCallback(26, gData_020049cc);
  ObjectMotion_EnableActionAndSetCallback(27, gData_020049dc);
  FieldEvent_WaitFrames(20);
  I_Func_020048cc(2, 0x102, 30);
  Func_0200489c(2, 0, 5);
  Func_020049c4(34);
  I_Func_020048cc(27, 0x108, 30);
  Func_0200489c(27, 0, 5);
  I_Func_020048cc(26, 0x107, 30);
  Func_0200489c(26, 0, 5);
  I_Func_020048cc(0, 0x107, 30);
  Func_0200489c(0, 0, 5);
  Object_RefreshSelectorById(26);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    Object_RefreshSelectorById(27);
  } while (0);
  Object_SetModeById(27, 4);
  FieldEvent_WaitFrames(33);
  Func_0200489c(27, 0, 5);
  I_Func_020048cc(2, 0x100, 30);
  Func_0200489c(2, 0, 5);
  I_Func_020048cc(26, 0x107, 30);
  Func_0200489c(26, 0, 5);
  I_Func_020048cc(2, 0x101, 0);
  I_Func_020048cc(0, 0x101, 30);
  Object_LinkPair(0, 2, 0);
  FieldEvent_WaitFrames(10);
  I_Func_020048cc(0, 0x106, 30);
  I_ObjectMotion_ArmCallback(2, 0xa000, 0);
  I_Func_020048b4(0, 0xa000);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    Func_0200489c(0, 0, 5);
  } while (0);
  Object_SetModeById(27, 3);
  FieldEvent_WaitFrames(20);
  FieldEvent_WaitFrames(10);
  Func_0200489c(27, 0, 5);
  I_Func_020048cc(2, 0x102, 30);
  Func_0200489c(2, 0, 5);
  I_Func_020048b4(26, 0xa000);
  FieldEvent_WaitFrames(10);
  Func_0200489c(26, 0, 5);
  Motion_SetVarCbAndRefresh(26, 2);
  FieldEvent_WaitFrames(5);
  Func_0200489c(26, 0, 5);
  I_Func_020048cc(0, 0x109, 30);
  Func_0200489c(0, 0, 5);
  Motion_SetVarCbAndRefresh(27, 2);
  FieldEvent_WaitFrames(5);
  Func_0200489c(27, 0, 5);
  I_Func_020048b4(26, angle);
  FieldEvent_WaitFrames(10);
  Func_0200489c(26, 0, 5);
  I_Func_020048cc(2, 0x107, 30);
  Func_0200489c(2, 0, 5);
  I_Func_020048cc(27, 0x108, 30);
  Func_0200489c(27, 0, 5);
  I_Func_020048cc(0, 0x103, 30);
  Func_0200489c(0, 0, 5);
  I_Func_020048cc(26, 0x108, 30);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    Func_0200489c(26, 0, 5);
  } while (0);
  Object_SetModeById(27, 3);
  FieldEvent_WaitFrames(20);
  FieldEvent_WaitFrames(10);
  Func_0200489c(27, 0, 5);
  Motion_SetVarCbAndRefresh(2, 2);
  FieldEvent_WaitFrames(5);
  Func_0200489c(2, 0, 5);
  Object_SetModeById(27, 4);
  Object_SetModeById(26, 4);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_WaitFrames(33);
  } while (0);
  I_Func_020048cc(2, 0x107, 30);
  Func_0200489c(2, 0, 5);
  I_Func_020048cc(26, 0x100, 30);
  Func_0200489c(26, 0, 5);
  I_Func_020048cc(27, 0x109, 30);
  Func_0200489c(27, 0, 5);
  I_Func_020048cc(0, 0x100, 30);
  I_Func_020048b4(0, angle);
  Object_SetModeById(0, 38);
  I_Motion_CamBounds(0x3100000, 0, 0x1120000, 1);
  Func_020048f4();
  FieldEvent_WaitFrames(60);
  I_Motion_CamBounds(0x2d00000, 0, 0xd20000, 1);
  Func_020048f4();
  Object_SetModeById(0, 1);
  I_Func_020048b4(0, 0xa000);
  FieldEvent_WaitFrames(10);
  Func_0200489c(0, 0, 5);
  Object_LinkPair(26, 27, 0);
  FieldEvent_WaitFrames(10);
  I_Func_020048cc(26, 0x108, 0);
  I_Func_020048cc(26, 0x108, 30);
  I_Func_020048cc(2, 0x101, 30);
  Func_0200489c(2, 0, 5);
  {
    /* FAKEMATCH(pin): the angle copied to r1 first; unpinned, 2 of 1568
     * instructions differ in tla-en. */
    register s32 a1 asm("r1") = angle;
    s32 flags = 0;

    /* FAKEMATCH(barrier): r1 and r2 are set before r0, as the native sets
     * them; without it, 18 of 1568 instructions differ in tla-en. */
    __asm__("" : : "r"(a1), "r"(flags));
    I_ObjectMotion_ArmCallback(26, a1, flags);
  }
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    I_Func_020048b4(27, angle);
  } while (0);
  Object_SetModeById(26, 3);
  FieldEvent_WaitFrames(20);
  FieldEvent_WaitFrames(10);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    Func_0200489c(26, 0, 5);
  } while (0);
  Object_SetModeById(27, 4);
  FieldEvent_WaitFrames(33);
  Func_0200489c(27, 0, 5);
  I_Func_020048cc(2, 0x103, 30);
  Func_0200489c(2, 0, 5);
  I_Func_020048cc(26, 0x108, 30);
  Func_0200489c(26, 0, 5);
  Motion_SetVarCbAndRefresh(0, 2);
  FieldEvent_WaitFrames(5);
  Func_0200489c(0, 0, 5);
  I_Func_020048cc(27, 0x108, 30);
  Func_0200489c(27, 0, 5);
  I_Func_020048b4(2, 0xe000);
  FieldEvent_WaitFrames(10);
  Func_0200489c(2, 0, 5);
  I_Func_020048cc(26, 0x109, 30);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    Func_0200489c(26, 0, 5);
  } while (0);
  Object_SetModeById(27, 4);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_WaitFrames(33);
  } while (0);
  Func_0200489c(27, 0, 5);
  I_Func_020048b4(27, 0xa000);
  FieldEvent_WaitFrames(20);
  I_Func_020048cc(27, 0x101, 30);
  I_ObjectMotion_SetSpeedParameters(27, 0x20000, 0x10000);
  I_ObjectMotion_CommitPositionAndActivate(27, -24, -24);
  FieldEvent_WaitFrames(5);
  I_Func_020048b4(27, 0x8000);
  FieldEvent_WaitFrames(10);
  I_Func_020048b4(27, 0xc000);
  FieldEvent_WaitFrames(10);
  I_Func_020048b4(27, 0);
  FieldEvent_WaitFrames(10);
  I_Func_020048b4(27, 0xc000);
  FieldEvent_WaitFrames(10);
  I_Func_020048cc(27, 0x102, 30);
  Func_0200489c(27, 0, 5);
  Func_0200489c(26, 0, 5);
  I_Func_020048b4(27, angle);
  FieldEvent_WaitFrames(10);
  Func_0200489c(27, 0, 5);
  I_Func_020048b4(26, 0xa000);
  FieldEvent_WaitFrames(10);
  Func_0200489c(26, 0, 5);
  Object_LinkPair(0, 2, 0);
  FieldEvent_WaitFrames(10);
  I_Func_020048bc(0, 20, 7, 15, 6, 2, 17, 13, 12, 12, 0);
  I_ObjectMotion_ArmCallback(0, 0xa000, 0);
  I_ObjectMotion_ArmCallback(2, 0xa000, 0);
  I_Func_020048cc(27, 0x103, 30);
  Func_0200489c(27, 0, 5);
  I_Func_020048cc(26, 0x107, 30);
  Func_0200489c(26, 0, 5);
  I_Func_020048b4(26, angle);
  FieldEvent_WaitFrames(10);
  I_Func_020048cc(26, 0x108, 30);
  Func_0200489c(26, 0, 5);
  I_Func_020048cc(27, 0x108, 30);
  ObjectMotion_OffsetPositionAndReset(27, 24, 24);
  ObjectMotion_EnableActionAndSetCallback(27, gData_020049ec);
  Func_0200489c(27, 0, 5);
  Object_RefreshSelectorById(27);
  Object_LinkPair(26, 27, 30);
  Object_SetModeById(26, 3);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    Object_SetModeById(27, 3);
  } while (0);
  FieldEvent_WaitFrames(20);
  FieldEvent_WaitFrames(30);
  I_ObjectMotion_SetSpeedParameters(26, 0x30000, 0x18000);
  I_ObjectMotion_SetSpeedParameters(27, 0x30000, 0x18000);
  I_ObjectMotion_SetSpeedParameters(0, 0x30000, 0x18000);
  I_ObjectMotion_SetSpeedParameters(2, 0x30000, 0x18000);
  I_ObjectMotion_ResetAndSetPositionInMode2(26, 737, 202);
  I_ObjectMotion_ResetAndSetPositionInMode2(27, 731, 213);
  ObjectMotion_EnableActionAndSetCallback(27, gData_020049fc);
  ObjectMotion_EnableActionAndSetCallback(26, gData_02004a18);
  FieldEvent_WaitFrames(5);
  I_ObjectMotion_ResetAndSetPositionInMode2(0, 776, 202);
  I_ObjectMotion_ResetAndSetPositionInMode2(2, 731, 232);
  ObjectMotion_EnableActionAndSetCallback(0, gData_02004a34);
  ObjectMotion_EnableActionAndSetCallback(2, gData_02004a50);
  Object_RefreshSelectorById(0);
  Object_RefreshSelectorById(2);
  Object_RefreshSelectorById(26);
  Object_RefreshSelectorById(27);
  FieldEvent_WaitFrames(40);
  Func_02000b48();
  FieldEvent_WaitFrames(200);
  Camera_FollowActor(4, 1);
  FieldEvent_WaitFrames(10);
  Motion_SetVarCbAndRefresh(28, 2);
  FieldEvent_WaitFrames(5);
  Func_0200489c(28, 0, 5);
  I_Func_020048cc(6, 0x109, 30);
  Func_0200489c(6, 0, 5);
  I_Func_020048cc(7, 0x107, 30);
  Func_0200489c(7, 0, 5);
  I_Func_020048cc(5, 0x109, 30);
  Func_0200489c(5, 0, 5);
  I_Func_020048b4(28, 0x4000);
  FieldEvent_WaitFrames(10);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    Func_0200489c(28, 0, 5);
  } while (0);
  Object_SetModeById(4, 3);
  FieldEvent_WaitFrames(20);
  FieldEvent_WaitFrames(10);
  ObjectMotion_SetAngleToward(5, 4, 0);
  ObjectMotion_SetAngleToward(7, 4, 0);
  ObjectMotion_SetAngleToward(6, 4, 0);
  FieldEvent_WaitFrames(20);
  Object_SetModeById(7, 3);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    Object_SetModeById(6, 3);
  } while (0);
  Object_SetModeById(5, 3);
  FieldEvent_WaitFrames(20);
  FieldEvent_WaitFrames(10);
  Object_SetModeById(7, 2);
  object = Object_GetById(4);
  if (object != NULL)
    ObjectMotion_ResetAndSetPosition(7, ((s16 *)&object->x)[1],
                                     ((s16 *)&object->z)[1]);
  Object_SetModeById(6, 2);
  object = Object_GetById(4);
  if (object != NULL)
    ObjectMotion_ResetAndSetPosition(6, ((s16 *)&object->x)[1],
                                     ((s16 *)&object->z)[1]);
  Object_SetModeById(5, 2);
  object = Object_GetById(4);
  if (object != NULL)
    ObjectMotion_ResetAndSetPosition(5, ((s16 *)&object->x)[1],
                                     ((s16 *)&object->z)[1]);
  Object_SetModeById(28, 2);
  object = Object_GetById(4);
  if (object != NULL)
    ObjectMotion_ResetAndSetPosition(28, ((s16 *)&object->x)[1],
                                     ((s16 *)&object->z)[1]);
  ObjectMotion_EnableActionAndSetCallback(7, gData_0200599c);
  ObjectMotion_EnableActionAndSetCallback(6, gData_0200599c);
  ObjectMotion_EnableActionAndSetCallback(5, gData_0200599c);
  ObjectMotion_EnableActionAndSetCallback(28, gData_0200599c);
  Object_RefreshSelectorById(7);
  Object_RefreshSelectorById(6);
  Object_RefreshSelectorById(5);
  Object_RefreshSelectorById(28);
  Func_020047dc();
}
