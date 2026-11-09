#include "jupiter_lighthouse_room_m.h"
#include "scene_tables.h"
#include "event_inline.h"

extern u32 gData_02005548[];


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
void Func_020048f4(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

static __inline__ void I_Func_020048bc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                       s32 g, s32 h, s32 i, s32 j, s32 k) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 7 sites, 152 of 1568 instructions differ in tla-en. */
  Battle_ShowPairedUnitWorkAndWait(a, b, c, d, e, f, g, h, i, j, k);
}

static __inline__ void I_Actor_ArmCallback(s32 id, s32 angle, s32 flags) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 15 sites, 73 of 1568 instructions differ in tla-en. */
  Actor_ArmCallback(id, angle, flags);
}

static __inline__ void I_Func_02004964(s32 actor, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 4 sites, 16 of 1568 instructions differ in tla-en. */
  Func_080d3070(actor, x, z, angle);
}

static __inline__ struct Actor *I_Func_020048cc(s32 actor, s32 emote,
                                                s32 frames) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 42 sites, 171 of 1568 instructions differ in tla-en. */
  return (struct Actor *)Func_080d47b4(actor, emote, frames);
}

static __inline__ void I_Actor_SetSpeedParameters(s32 actor, s32 speed,
                                                  s32 acceleration) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 14 sites, 78 of 1568 instructions differ in tla-en. */
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void I_Motion_CamBounds(s32 x, s32 y, s32 z, s32 d) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 6 sites, 25 of 1568 instructions differ in tla-en. */
  Camera_MoveWithinMapBounds(x, y, z, d);
}

static __inline__ void I_Func_0200484c(s32 id, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 3 sites, 14 of 1568 instructions differ in tla-en. */
  Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, angle);
}

static __inline__ void I_Func_020048b4(s32 actor, s32 a) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 20 sites, 85 of 1568 instructions differ in tla-en. */
  Actor_ArmCallbackAndRefresh(actor, a);
}

static __inline__ void I_Func_020048d4(s32 actor, s32 a) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its one site, 16 of 1568 instructions differ in tla-en. */
  Func_080d489c(actor, a);
}

static __inline__ void I_Actor_OffsetPositionAndResetMotion(s32 id, s32 dx,
                                                            s32 dz) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 6 sites, 26 of 1568 instructions differ in tla-en. */
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void I_Actor_ResetAndSetPositionInMode2(s32 id, s32 x,
                                                          s32 z) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its 6 sites, 25 of 1568 instructions differ in tla-en. */
  Actor_ResetAndSetPositionInMode2(id, x, z);
}

static __inline__ void I_Actor_CommitPositionAndActivate(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly at its one site, 7 of 1568 instructions differ in tla-en. */
  Actor_CommitPositionAndActivate(id, x, z);
}

/// The scene in which actor 4 meets 5, 6, 7 and 28 and the group is placed for
/// the arrival; arg selects one of its two layouts.
void Func_02000cac(s32 arg) {
  struct Actor *object;
  s32 angle;

  Func_020047d4();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId(MsgAhhhh2);
  Event_PlaySound(124);
  FieldEvent_Wait(26);
  Event_PlaySound(217);
  Actor_SetScript(4, gData_020056ec);
  if (arg != 0) {
    I_Func_020048bc(3, 20, 9, 15, 8, 1, 20, 2, 15, 1, 25);
    Event_PlaySound(310);
    FieldEvent_Wait(30);
    Event_PlaySound(144);
    I_Func_020048bc(3, 21, 9, 16, 8, 1, 21, 2, 16, 1, 0);
    FieldEvent_ShowTwoMessagesAndWait(0, 20, 9, 15, 8, 2, 20, 2, 15, 1, 0);
  } else {
    I_Func_020048bc(3, 5, 9, 0, 8, 1, 5, 2, 0, 1, 25);
    Event_PlaySound(310);
    FieldEvent_Wait(30);
    Event_PlaySound(144);
    FieldEvent_ShowTwoMessagesAndWait(3, 5, 9, 0, 8, 1, 5, 2, 0, 1, 0);
    FieldEvent_ShowTwoMessagesAndWait(0, 5, 9, 0, 8, 2, 5, 2, 0, 1, 0);
  }
  FieldActor_Stop(4);
  I_Actor_ArmCallback(4, FACING_SOUTH, 0);
  I_Func_02004964(7, 16, 0, FACING_WEST);
  I_Func_02004964(28, -16, 0, FACING_EAST);
  I_Func_02004964(6, 16, 24, FACING_WEST);
  I_Func_02004964(5, -16, 24, FACING_EAST);
  FieldCamera_FollowActor(5, 1);
  FieldActor_WaitForMoveById(5);
  (struct Actor *)Actor_ShowEmote(28, 0x102, 30);
  FieldEvent_ShowMessageThenWait(28, 0, 5);
  I_Actor_ArmCallback(5, FACING_NORTH, 0);
  I_Actor_ArmCallback(6, FACING_NORTHWEST + FACING_STEP, 0);
  (struct Actor *)Actor_ShowEmote(7, 0x101, 30);
  I_Actor_ArmCallback(5, FACING_NORTH + FACING_STEP, 0);
  Event_OpenMessage(7, 0);
  Actor_FaceDirection(4, FACING_EAST, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    Actor_SetAnimation(28, 3);
    FieldEvent_Wait(20);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(28, 0, 5);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    Actor_SetAnimation(28, 4);
    FieldEvent_Wait(33);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    FieldEvent_ShowMessageThenWait(28, 0, 5);
  }
  Actor_RunRepeatedMotion(5, 2);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  (struct Actor *)Actor_ShowEmote(6, 0x101, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  (struct Actor *)Actor_ShowEmote(4, 0x102, 30);
  I_Actor_SetSpeedParameters(4, 0x00013333, 0x9999);
  Actor_SetScript(4, arg != 0 ? gData_02005748 : gData_0200578c);
  FieldEvent_Wait(30);
  I_Actor_ArmCallback(6, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirection(28, FACING_EAST, 0);
  I_Actor_ArmCallback(7, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(28, 3);
  FieldEvent_Wait(30);
  I_Actor_SetSpeedParameters(5, 0x10000, 0x8000);
  I_Actor_SetSpeedParameters(7, 0x10000, 0x8000);
  I_Actor_SetSpeedParameters(6, 0x10000, 0x8000);
  I_Actor_SetSpeedParameters(28, 0xcccc, 0x6666);
  Actor_SetScript(28, arg != 0 ? gData_020057d0 : gData_02005800);
  Actor_SetScript(7, arg != 0 ? gData_0200d830 : gData_02005860);
  Actor_SetScript(5, arg != 0 ? gData_02005890 : gData_020058d8);
  Actor_SetScript(6, arg != 0 ? gData_02005920 : gData_02005950);
  FieldCamera_SetSpeed(0x8000, 0x1000);
  if (arg != 0) {
    FieldCamera_MoveTo(PIXELS(677), PIXELS(-64), PIXELS(287), 1);
    Func_020048f4();
  } else {
    FieldCamera_MoveTo(PIXELS(950), PIXELS(-64), PIXELS(287), 1);
    Func_020048f4();
  }
  FieldActor_RefreshSelectorById(4);
  FieldActor_RefreshSelectorById(7);
  FieldActor_RefreshSelectorById(5);
  FieldActor_RefreshSelectorById(6);
  FieldActor_RefreshSelectorById(28);
  FieldEvent_Wait(20);
  angle = 0x2000;
  I_Func_0200484c(2, PIXELS(741), PIXELS(232), angle);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  FieldCamera_MoveTo(PIXELS(800), 0, PIXELS(270), 1);
  Func_020048f4();
  Event_PlaySound(39);
  FieldEvent_Wait(50);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_SetAnimation(3, 0);
  I_Func_020048b4(3, FACING_NORTHWEST);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_FaceDirectionAndWait(3, angle);
  Actor_SetAnimation(3, 9);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_RunRepeatedMotion(1, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(1, 0, 5);
  FieldActor_StartRepeatedMotion(3, 3);
  I_Func_020048d4(3, 258);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_RunRepeatedMotion(1, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(1, 0, 5);
  Actor_Get(3)->actionFlags &= 0xfe;
  I_Actor_SetSpeedParameters(3, 0x1999, 0xccc);
  Actor_SetDestinationOffset(3, 4, 2);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 9);
  Actor_RunRepeatedMotion(3, 2);
  I_Actor_OffsetPositionAndResetMotion(3, -2, 2);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 9);
  Actor_RunRepeatedMotion(3, 2);
  Actor_SetDestinationOffset(3, 2, 1);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 9);
  Actor_RunRepeatedMotion(3, 2);
  I_Actor_OffsetPositionAndResetMotion(3, -2, -1);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 9);
  Actor_RunRepeatedMotion(3, 2);
  FieldEvent_Wait(40);
  I_Actor_OffsetPositionAndResetMotion(3, 0, -2);
  FieldActor_WaitForMoveById(3);
  I_Actor_SetSpeedParameters(3, 0x20000, 0x10000);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    Actor_SetAnimation(3, 4);
  } while (0);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_Wait(33);
  } while (0);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_Get(3)->actionFlags |= 1;
  Actor_SetDestinationOffset(3, -2, -8);
  FieldActor_WaitForMoveById(3);
  Actor_SetAnimation(3, 1);
  I_Func_020048b4(3, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(3, 0, 5);
  Actor_SetAnimation(0, 1);
  Actor_SetAnimation(2, 1);
  Actor_FaceEachOther(0, 2, 0);
  FieldEvent_Wait(10);
  I_Func_020048cc(0, 0x102, 30);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_ShowMessageThenWait(0, 0, 5);
  } while (0);
  Actor_SetAnimation(2, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_SetScript(3, gData_02005548);
  I_Func_0200484c(26, PIXELS(665), PIXELS(128), angle);
  Actor_SetPositionAndFacing(27, PIXELS(641), PIXELS(128),
                                                 angle);
  Event_PlaySound(78);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(0, 0x100, 0);
  I_Func_020048cc(2, 0x100, 30);
  I_Actor_ArmCallback(0, FACING_NORTHWEST, 0);
  I_Func_020048b4(2, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  I_Motion_CamBounds(PIXELS(720), 0, PIXELS(210), 1);
  I_Actor_SetSpeedParameters(26, 0x4000, angle);
  I_Actor_SetSpeedParameters(27, 0x4000, angle);
  I_Actor_ResetAndSetPositionInMode2(26, 714, 193);
  I_Actor_ResetAndSetPositionInMode2(27, 698, 209);
  Actor_SetScript(26, gData_020049cc);
  Actor_SetScript(27, gData_020049dc);
  FieldEvent_Wait(20);
  I_Func_020048cc(2, 0x102, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  Event_PlaySound(34);
  I_Func_020048cc(27, 0x108, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  I_Func_020048cc(26, 0x107, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(0, 0x107, 30);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  FieldActor_RefreshSelectorById(26);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    FieldActor_RefreshSelectorById(27);
  } while (0);
  Actor_SetAnimation(27, 4);
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  I_Func_020048cc(2, 0x100, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(26, 0x107, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(2, 0x101, 0);
  I_Func_020048cc(0, 0x101, 30);
  Actor_FaceEachOther(0, 2, 0);
  FieldEvent_Wait(10);
  (struct Actor *)Actor_ShowEmote(0, 0x106, 30);
  I_Actor_ArmCallback(2, FACING_NORTHWEST, 0);
  I_Func_020048b4(0, FACING_NORTHWEST);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_ShowMessageThenWait(0, 0, 5);
  } while (0);
  Actor_SetAnimation(27, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  (struct Actor *)Actor_ShowEmote(2, 0x102, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  I_Func_020048b4(26, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_RunRepeatedMotion(26, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(0, 0x109, 30);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_RunRepeatedMotion(27, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_FaceDirectionAndWait(26, angle);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(2, 0x107, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(27, 0x108, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  (struct Actor *)Actor_ShowEmote(0, 0x103, 30);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  I_Func_020048cc(26, 0x108, 30);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_ShowMessageThenWait(26, 0, 5);
  } while (0);
  Actor_SetAnimation(27, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_RunRepeatedMotion(2, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  Actor_SetAnimation(27, 4);
  Actor_SetAnimation(26, 4);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_Wait(33);
  } while (0);
  I_Func_020048cc(2, 0x107, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(26, 0x100, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  (struct Actor *)Actor_ShowEmote(27, 0x109, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  (struct Actor *)Actor_ShowEmote(0, 0x100, 30);
  Actor_FaceDirectionAndWait(0, angle);
  Actor_SetAnimation(0, 38);
  I_Motion_CamBounds(PIXELS(784), 0, PIXELS(274), 1);
  Func_020048f4();
  FieldEvent_Wait(60);
  FieldCamera_MoveTo(PIXELS(720), 0, PIXELS(210), 1);
  Func_020048f4();
  Actor_SetAnimation(0, 1);
  Actor_FaceDirectionAndWait(0, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_FaceEachOther(26, 27, 0);
  FieldEvent_Wait(10);
  I_Func_020048cc(26, 0x108, 0);
  (struct Actor *)Actor_ShowEmote(26, 0x108, 30);
  (struct Actor *)Actor_ShowEmote(2, 0x101, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  {
    /* FAKEMATCH(pin): the angle copied to r1 first; unpinned, 2 of 1568
     * instructions differ in tla-en. */
    register s32 a1 asm("r1") = angle;
    s32 flags = 0;

    /* FAKEMATCH(barrier): r1 and r2 are set before r0, as the native sets
     * them; without it, 18 of 1568 instructions differ in tla-en. */
    __asm__("" : : "r"(a1), "r"(flags));
    Actor_FaceDirection(26, a1, flags);
  }
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    Actor_FaceDirectionAndWait(27, angle);
  } while (0);
  Actor_SetAnimation(26, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_ShowMessageThenWait(26, 0, 5);
  } while (0);
  Actor_SetAnimation(27, 4);
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  I_Func_020048cc(2, 0x103, 30);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(26, 0x108, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  I_Func_020048cc(27, 0x108, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  Actor_FaceDirectionAndWait(2, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(26, 0x109, 30);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_ShowMessageThenWait(26, 0, 5);
  } while (0);
  Actor_SetAnimation(27, 4);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_Wait(33);
  } while (0);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  I_Func_020048b4(27, FACING_NORTHWEST);
  FieldEvent_Wait(20);
  (struct Actor *)Actor_ShowEmote(27, 0x101, 30);
  I_Actor_SetSpeedParameters(27, 0x20000, 0x10000);
  I_Actor_CommitPositionAndActivate(27, -24, -24);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(27, FACING_WEST);
  FieldEvent_Wait(10);
  I_Func_020048b4(27, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(27, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(27, FACING_NORTH);
  FieldEvent_Wait(10);
  (struct Actor *)Actor_ShowEmote(27, 0x102, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_FaceDirectionAndWait(27, angle);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  I_Func_020048b4(26, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_FaceEachOther(0, 2, 0);
  FieldEvent_Wait(10);
  I_Func_020048bc(0, 20, 7, 15, 6, 2, 17, 13, 12, 12, 0);
  I_Actor_ArmCallback(0, FACING_NORTHWEST, 0);
  I_Actor_ArmCallback(2, FACING_NORTHWEST, 0);
  (struct Actor *)Actor_ShowEmote(27, 0x103, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  I_Func_020048cc(26, 0x107, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_FaceDirectionAndWait(26, angle);
  FieldEvent_Wait(10);
  I_Func_020048cc(26, 0x108, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  (struct Actor *)Actor_ShowEmote(27, 0x108, 30);
  Actor_WalkBy(27, 24, 24);
  Actor_SetScript(27, gData_020049ec);
  FieldEvent_ShowMessageThenWait(27, 0, 5);
  FieldActor_RefreshSelectorById(27);
  Actor_FaceEachOther(26, 27, 30);
  Actor_SetAnimation(26, 3);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    Actor_SetAnimation(27, 3);
  } while (0);
  FieldEvent_Wait(20);
  FieldEvent_Wait(30);
  I_Actor_SetSpeedParameters(26, 0x30000, 0x18000);
  I_Actor_SetSpeedParameters(27, 0x30000, 0x18000);
  I_Actor_SetSpeedParameters(0, 0x30000, 0x18000);
  I_Actor_SetSpeedParameters(2, 0x30000, 0x18000);
  I_Actor_ResetAndSetPositionInMode2(26, 737, 202);
  I_Actor_ResetAndSetPositionInMode2(27, 731, 213);
  Actor_SetScript(27, gData_020049fc);
  Actor_SetScript(26, gData_02004a18);
  FieldEvent_Wait(5);
  I_Actor_ResetAndSetPositionInMode2(0, 776, 202);
  I_Actor_ResetAndSetPositionInMode2(2, 731, 232);
  Actor_SetScript(0, gData_02004a34);
  Actor_SetScript(2, gData_02004a50);
  FieldActor_RefreshSelectorById(0);
  FieldActor_RefreshSelectorById(2);
  FieldActor_RefreshSelectorById(26);
  FieldActor_RefreshSelectorById(27);
  FieldEvent_Wait(40);
  Func_02000b48();
  FieldEvent_Wait(200);
  FieldCamera_FollowActor(4, 1);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(28, 0, 5);
  I_Func_020048cc(6, 0x109, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  (struct Actor *)Actor_ShowEmote(7, 0x107, 30);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  (struct Actor *)Actor_ShowEmote(5, 0x109, 30);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_FaceDirectionAndWait(28, FACING_SOUTH);
  FieldEvent_Wait(10);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    FieldEvent_ShowMessageThenWait(28, 0, 5);
  } while (0);
  Actor_SetAnimation(4, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_FaceActor(5, 4, 0);
  Actor_FaceActor(7, 4, 0);
  Actor_FaceActor(6, 4, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(7, 3);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    Actor_SetAnimation(6, 3);
  } while (0);
  Actor_SetAnimation(5, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_SetAnimation(7, 2);
  object = Actor_Get(4);
  if (object != NULL)
    Actor_SetDestination(7, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_SetAnimation(6, 2);
  object = Actor_Get(4);
  if (object != NULL)
    Actor_SetDestination(6, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_SetAnimation(5, 2);
  object = Actor_Get(4);
  if (object != NULL)
    Actor_SetDestination(5, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_SetAnimation(28, 2);
  object = Actor_Get(4);
  if (object != NULL)
    Actor_SetDestination(28, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_SetScript(7, gData_0200599c);
  Actor_SetScript(6, gData_0200599c);
  Actor_SetScript(5, gData_0200599c);
  Actor_SetScript(28, gData_0200599c);
  FieldActor_RefreshSelectorById(7);
  FieldActor_RefreshSelectorById(6);
  FieldActor_RefreshSelectorById(5);
  FieldActor_RefreshSelectorById(28);
  Func_020047dc();
}
