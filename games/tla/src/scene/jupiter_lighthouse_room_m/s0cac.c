#include "jupiter_lighthouse_room_m.h"
#include "scene_tables.h"

extern u32 gData_02005548[];

void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 z,
                                                    s32 angle);
void Actor_SetMovementTypeById(s32 id, const void *script);

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
void Event_SetMessageId(const u8 *message);
void Event_ShowMessageThenWait(s32 actor, s32 a, s32 b);
void Actor_ArmCallbackAndRefresh(s32 actor, s32 a);
void Battle_ShowPairedUnitWorkAndWait(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g, s32 h, s32 i, s32 j, s32 k);
struct Actor *Func_080d47b4(s32 actor, s32 emote, s32 frames);
void Func_080d489c(s32 actor, s32 a);
void Camera_SetSpeedParameters(s32 a, s32 b);
void Func_020048f4(void);
void Event_ResolvePendingActions(s32 a);
void Func_080d3070(s32 actor, s32 x, s32 z, s32 angle);
void Sound_Play(s32 cue);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Actor_Stop(s32 id);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_WaitForMoveById(s32 id);
void Actor_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Actor_OffsetPositionAndReset(s32 id, s32 dx, s32 dz);
void Actor_StartRepeatedMotion(s32 id, s32 variant);
void Actor_SetAngleToward(s32 id, s32 target, s32 flags);
void Actor_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void Actor_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Camera_FollowActor(s32 id, s32 mode);
void Actor_TurnToFaceEachOther(s32 a, s32 b, s32 c);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 d);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);

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
  return Func_080d47b4(actor, emote, frames);
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
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId(MsgAhhhh2);
  Sound_Play(124);
  Event_Wait(26);
  Sound_Play(217);
  Actor_SetMovementTypeById(4, gData_020056ec);
  if (arg != 0) {
    I_Func_020048bc(3, 20, 9, 15, 8, 1, 20, 2, 15, 1, 25);
    Sound_Play(310);
    Event_Wait(30);
    Sound_Play(144);
    I_Func_020048bc(3, 21, 9, 16, 8, 1, 21, 2, 16, 1, 0);
    Battle_ShowPairedUnitWorkAndWait(0, 20, 9, 15, 8, 2, 20, 2, 15, 1, 0);
  } else {
    I_Func_020048bc(3, 5, 9, 0, 8, 1, 5, 2, 0, 1, 25);
    Sound_Play(310);
    Event_Wait(30);
    Sound_Play(144);
    Battle_ShowPairedUnitWorkAndWait(3, 5, 9, 0, 8, 1, 5, 2, 0, 1, 0);
    Battle_ShowPairedUnitWorkAndWait(0, 5, 9, 0, 8, 2, 5, 2, 0, 1, 0);
  }
  Actor_Stop(4);
  I_Actor_ArmCallback(4, FACING_SOUTH, 0);
  I_Func_02004964(7, 16, 0, FACING_WEST);
  I_Func_02004964(28, -16, 0, FACING_EAST);
  I_Func_02004964(6, 16, 24, FACING_WEST);
  I_Func_02004964(5, -16, 24, FACING_EAST);
  Camera_FollowActor(5, 1);
  Actor_WaitForMoveById(5);
  Func_080d47b4(28, 0x102, 30);
  Event_ShowMessageThenWait(28, 0, 5);
  I_Actor_ArmCallback(5, FACING_NORTH, 0);
  I_Actor_ArmCallback(6, FACING_NORTHWEST + FACING_STEP, 0);
  Func_080d47b4(7, 0x101, 30);
  I_Actor_ArmCallback(5, FACING_NORTH + FACING_STEP, 0);
  FieldMessage_ShowAtActor(7, 0);
  Actor_ArmCallback(4, FACING_EAST, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Actor_SetModeById(28, 3);
    Event_Wait(20);
    Event_Wait(10);
    Event_ShowMessageThenWait(28, 0, 5);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    Actor_SetModeById(28, 4);
    Event_Wait(33);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(28, 0, 5);
  }
  Motion_SetVarCbAndRefresh(5, 2);
  Actor_ArmCallback(4, FACING_SOUTH, 0);
  Event_Wait(5);
  Event_ShowMessageThenWait(5, 0, 5);
  Func_080d47b4(6, 0x101, 30);
  Event_ShowMessageThenWait(6, 0, 5);
  Func_080d47b4(4, 0x102, 30);
  I_Actor_SetSpeedParameters(4, 0x00013333, 0x9999);
  Actor_SetMovementTypeById(4, arg != 0 ? gData_02005748 : gData_0200578c);
  Event_Wait(30);
  I_Actor_ArmCallback(6, FACING_WEST, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_ArmCallback(28, FACING_EAST, 0);
  I_Actor_ArmCallback(7, FACING_WEST, 0);
  Event_Wait(10);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(28, 3);
  Event_Wait(30);
  I_Actor_SetSpeedParameters(5, 0x10000, 0x8000);
  I_Actor_SetSpeedParameters(7, 0x10000, 0x8000);
  I_Actor_SetSpeedParameters(6, 0x10000, 0x8000);
  I_Actor_SetSpeedParameters(28, 0xcccc, 0x6666);
  Actor_SetMovementTypeById(28, arg != 0 ? gData_020057d0 : gData_02005800);
  Actor_SetMovementTypeById(7, arg != 0 ? gData_0200d830 : gData_02005860);
  Actor_SetMovementTypeById(5, arg != 0 ? gData_02005890 : gData_020058d8);
  Actor_SetMovementTypeById(6, arg != 0 ? gData_02005920 : gData_02005950);
  Camera_SetSpeedParameters(0x8000, 0x1000);
  if (arg != 0) {
    Camera_MoveWithinMapBounds(PIXELS(677), PIXELS(-64), PIXELS(287), 1);
    Func_020048f4();
  } else {
    Camera_MoveWithinMapBounds(PIXELS(950), PIXELS(-64), PIXELS(287), 1);
    Func_020048f4();
  }
  Actor_RefreshSelectorById(4);
  Actor_RefreshSelectorById(7);
  Actor_RefreshSelectorById(5);
  Actor_RefreshSelectorById(6);
  Actor_RefreshSelectorById(28);
  Event_Wait(20);
  angle = 0x2000;
  I_Func_0200484c(2, PIXELS(741), PIXELS(232), angle);
  Camera_SetSpeedParameters(0x40000, 0x8000);
  Camera_MoveWithinMapBounds(PIXELS(800), 0, PIXELS(270), 1);
  Func_020048f4();
  Sound_Play(39);
  Event_Wait(50);
  Motion_SetVarCbAndRefresh(0, 2);
  Event_ShowMessageThenWait(0, 0, 5);
  Actor_SetModeById(3, 0);
  I_Func_020048b4(3, FACING_NORTHWEST);
  Event_ShowMessageThenWait(3, 0, 5);
  Actor_ArmCallbackAndRefresh(3, angle);
  Actor_SetModeById(3, 9);
  Event_ShowMessageThenWait(3, 0, 5);
  Motion_SetVarCbAndRefresh(1, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(1, 0, 5);
  Actor_StartRepeatedMotion(3, 3);
  I_Func_020048d4(3, 258);
  Event_Wait(40);
  Event_ShowMessageThenWait(3, 0, 5);
  Motion_SetVarCbAndRefresh(1, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(1, 0, 5);
  Actor_GetById(3)->actionFlags &= 0xfe;
  I_Actor_SetSpeedParameters(3, 0x1999, 0xccc);
  Actor_OffsetPositionAndResetMotion(3, 4, 2);
  Actor_WaitForMoveById(3);
  Actor_SetModeById(3, 9);
  Motion_SetVarCbAndRefresh(3, 2);
  I_Actor_OffsetPositionAndResetMotion(3, -2, 2);
  Actor_WaitForMoveById(3);
  Actor_SetModeById(3, 9);
  Motion_SetVarCbAndRefresh(3, 2);
  Actor_OffsetPositionAndResetMotion(3, 2, 1);
  Actor_WaitForMoveById(3);
  Actor_SetModeById(3, 9);
  Motion_SetVarCbAndRefresh(3, 2);
  I_Actor_OffsetPositionAndResetMotion(3, -2, -1);
  Actor_WaitForMoveById(3);
  Actor_SetModeById(3, 9);
  Motion_SetVarCbAndRefresh(3, 2);
  Event_Wait(40);
  I_Actor_OffsetPositionAndResetMotion(3, 0, -2);
  Actor_WaitForMoveById(3);
  I_Actor_SetSpeedParameters(3, 0x20000, 0x10000);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    Actor_SetModeById(3, 4);
  } while (0);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    Event_Wait(33);
  } while (0);
  Event_ShowMessageThenWait(3, 0, 5);
  Actor_GetById(3)->actionFlags |= 1;
  Actor_OffsetPositionAndResetMotion(3, -2, -8);
  Actor_WaitForMoveById(3);
  Actor_SetModeById(3, 1);
  I_Func_020048b4(3, FACING_NORTHWEST);
  Event_Wait(10);
  Event_ShowMessageThenWait(3, 0, 5);
  Actor_SetModeById(0, 1);
  Actor_SetModeById(2, 1);
  Actor_TurnToFaceEachOther(0, 2, 0);
  Event_Wait(10);
  I_Func_020048cc(0, 0x102, 30);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    Event_ShowMessageThenWait(0, 0, 5);
  } while (0);
  Actor_SetModeById(2, 3);
  Event_Wait(20);
  Event_Wait(10);
  Actor_SetMovementTypeById(3, gData_02005548);
  I_Func_0200484c(26, PIXELS(665), PIXELS(128), angle);
  Actor_SetHorizontalPositionAndAngleWithTerrain(27, PIXELS(641), PIXELS(128),
                                                 angle);
  Sound_Play(78);
  Event_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(0, 0x100, 0);
  I_Func_020048cc(2, 0x100, 30);
  I_Actor_ArmCallback(0, FACING_NORTHWEST, 0);
  I_Func_020048b4(2, FACING_NORTHWEST);
  Event_Wait(10);
  I_Motion_CamBounds(PIXELS(720), 0, PIXELS(210), 1);
  I_Actor_SetSpeedParameters(26, 0x4000, angle);
  I_Actor_SetSpeedParameters(27, 0x4000, angle);
  I_Actor_ResetAndSetPositionInMode2(26, 714, 193);
  I_Actor_ResetAndSetPositionInMode2(27, 698, 209);
  Actor_SetMovementTypeById(26, gData_020049cc);
  Actor_SetMovementTypeById(27, gData_020049dc);
  Event_Wait(20);
  I_Func_020048cc(2, 0x102, 30);
  Event_ShowMessageThenWait(2, 0, 5);
  Sound_Play(34);
  I_Func_020048cc(27, 0x108, 30);
  Event_ShowMessageThenWait(27, 0, 5);
  I_Func_020048cc(26, 0x107, 30);
  Event_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(0, 0x107, 30);
  Event_ShowMessageThenWait(0, 0, 5);
  Actor_RefreshSelectorById(26);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    Actor_RefreshSelectorById(27);
  } while (0);
  Actor_SetModeById(27, 4);
  Event_Wait(33);
  Event_ShowMessageThenWait(27, 0, 5);
  I_Func_020048cc(2, 0x100, 30);
  Event_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(26, 0x107, 30);
  Event_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(2, 0x101, 0);
  I_Func_020048cc(0, 0x101, 30);
  Actor_TurnToFaceEachOther(0, 2, 0);
  Event_Wait(10);
  Func_080d47b4(0, 0x106, 30);
  I_Actor_ArmCallback(2, FACING_NORTHWEST, 0);
  I_Func_020048b4(0, FACING_NORTHWEST);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    Event_ShowMessageThenWait(0, 0, 5);
  } while (0);
  Actor_SetModeById(27, 3);
  Event_Wait(20);
  Event_Wait(10);
  Event_ShowMessageThenWait(27, 0, 5);
  Func_080d47b4(2, 0x102, 30);
  Event_ShowMessageThenWait(2, 0, 5);
  I_Func_020048b4(26, FACING_NORTHWEST);
  Event_Wait(10);
  Event_ShowMessageThenWait(26, 0, 5);
  Motion_SetVarCbAndRefresh(26, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(0, 0x109, 30);
  Event_ShowMessageThenWait(0, 0, 5);
  Motion_SetVarCbAndRefresh(27, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(27, 0, 5);
  Actor_ArmCallbackAndRefresh(26, angle);
  Event_Wait(10);
  Event_ShowMessageThenWait(26, 0, 5);
  I_Func_020048cc(2, 0x107, 30);
  Event_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(27, 0x108, 30);
  Event_ShowMessageThenWait(27, 0, 5);
  Func_080d47b4(0, 0x103, 30);
  Event_ShowMessageThenWait(0, 0, 5);
  I_Func_020048cc(26, 0x108, 30);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    Event_ShowMessageThenWait(26, 0, 5);
  } while (0);
  Actor_SetModeById(27, 3);
  Event_Wait(20);
  Event_Wait(10);
  Event_ShowMessageThenWait(27, 0, 5);
  Motion_SetVarCbAndRefresh(2, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(2, 0, 5);
  Actor_SetModeById(27, 4);
  Actor_SetModeById(26, 4);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    Event_Wait(33);
  } while (0);
  I_Func_020048cc(2, 0x107, 30);
  Event_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(26, 0x100, 30);
  Event_ShowMessageThenWait(26, 0, 5);
  Func_080d47b4(27, 0x109, 30);
  Event_ShowMessageThenWait(27, 0, 5);
  Func_080d47b4(0, 0x100, 30);
  Actor_ArmCallbackAndRefresh(0, angle);
  Actor_SetModeById(0, 38);
  I_Motion_CamBounds(PIXELS(784), 0, PIXELS(274), 1);
  Func_020048f4();
  Event_Wait(60);
  Camera_MoveWithinMapBounds(PIXELS(720), 0, PIXELS(210), 1);
  Func_020048f4();
  Actor_SetModeById(0, 1);
  Actor_ArmCallbackAndRefresh(0, FACING_NORTHWEST);
  Event_Wait(10);
  Event_ShowMessageThenWait(0, 0, 5);
  Actor_TurnToFaceEachOther(26, 27, 0);
  Event_Wait(10);
  I_Func_020048cc(26, 0x108, 0);
  Func_080d47b4(26, 0x108, 30);
  Func_080d47b4(2, 0x101, 30);
  Event_ShowMessageThenWait(2, 0, 5);
  {
    /* FAKEMATCH(pin): the angle copied to r1 first; unpinned, 2 of 1568
     * instructions differ in tla-en. */
    register s32 a1 asm("r1") = angle;
    s32 flags = 0;

    /* FAKEMATCH(barrier): r1 and r2 are set before r0, as the native sets
     * them; without it, 18 of 1568 instructions differ in tla-en. */
    __asm__("" : : "r"(a1), "r"(flags));
    Actor_ArmCallback(26, a1, flags);
  }
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    Actor_ArmCallbackAndRefresh(27, angle);
  } while (0);
  Actor_SetModeById(26, 3);
  Event_Wait(20);
  Event_Wait(10);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    Event_ShowMessageThenWait(26, 0, 5);
  } while (0);
  Actor_SetModeById(27, 4);
  Event_Wait(33);
  Event_ShowMessageThenWait(27, 0, 5);
  I_Func_020048cc(2, 0x103, 30);
  Event_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(26, 0x108, 30);
  Event_ShowMessageThenWait(26, 0, 5);
  Motion_SetVarCbAndRefresh(0, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(0, 0, 5);
  I_Func_020048cc(27, 0x108, 30);
  Event_ShowMessageThenWait(27, 0, 5);
  Actor_ArmCallbackAndRefresh(2, FACING_NORTHEAST);
  Event_Wait(10);
  Event_ShowMessageThenWait(2, 0, 5);
  I_Func_020048cc(26, 0x109, 30);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    Event_ShowMessageThenWait(26, 0, 5);
  } while (0);
  Actor_SetModeById(27, 4);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    Event_Wait(33);
  } while (0);
  Event_ShowMessageThenWait(27, 0, 5);
  I_Func_020048b4(27, FACING_NORTHWEST);
  Event_Wait(20);
  Func_080d47b4(27, 0x101, 30);
  I_Actor_SetSpeedParameters(27, 0x20000, 0x10000);
  I_Actor_CommitPositionAndActivate(27, -24, -24);
  Event_Wait(5);
  Actor_ArmCallbackAndRefresh(27, FACING_WEST);
  Event_Wait(10);
  I_Func_020048b4(27, FACING_NORTH);
  Event_Wait(10);
  Actor_ArmCallbackAndRefresh(27, FACING_EAST);
  Event_Wait(10);
  Actor_ArmCallbackAndRefresh(27, FACING_NORTH);
  Event_Wait(10);
  Func_080d47b4(27, 0x102, 30);
  Event_ShowMessageThenWait(27, 0, 5);
  Event_ShowMessageThenWait(26, 0, 5);
  Actor_ArmCallbackAndRefresh(27, angle);
  Event_Wait(10);
  Event_ShowMessageThenWait(27, 0, 5);
  I_Func_020048b4(26, FACING_NORTHWEST);
  Event_Wait(10);
  Event_ShowMessageThenWait(26, 0, 5);
  Actor_TurnToFaceEachOther(0, 2, 0);
  Event_Wait(10);
  I_Func_020048bc(0, 20, 7, 15, 6, 2, 17, 13, 12, 12, 0);
  I_Actor_ArmCallback(0, FACING_NORTHWEST, 0);
  I_Actor_ArmCallback(2, FACING_NORTHWEST, 0);
  Func_080d47b4(27, 0x103, 30);
  Event_ShowMessageThenWait(27, 0, 5);
  I_Func_020048cc(26, 0x107, 30);
  Event_ShowMessageThenWait(26, 0, 5);
  Actor_ArmCallbackAndRefresh(26, angle);
  Event_Wait(10);
  I_Func_020048cc(26, 0x108, 30);
  Event_ShowMessageThenWait(26, 0, 5);
  Func_080d47b4(27, 0x108, 30);
  Actor_OffsetPositionAndReset(27, 24, 24);
  Actor_SetMovementTypeById(27, gData_020049ec);
  Event_ShowMessageThenWait(27, 0, 5);
  Actor_RefreshSelectorById(27);
  Actor_TurnToFaceEachOther(26, 27, 30);
  Actor_SetModeById(26, 3);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    Actor_SetModeById(27, 3);
  } while (0);
  Event_Wait(20);
  Event_Wait(30);
  I_Actor_SetSpeedParameters(26, 0x30000, 0x18000);
  I_Actor_SetSpeedParameters(27, 0x30000, 0x18000);
  I_Actor_SetSpeedParameters(0, 0x30000, 0x18000);
  I_Actor_SetSpeedParameters(2, 0x30000, 0x18000);
  I_Actor_ResetAndSetPositionInMode2(26, 737, 202);
  I_Actor_ResetAndSetPositionInMode2(27, 731, 213);
  Actor_SetMovementTypeById(27, gData_020049fc);
  Actor_SetMovementTypeById(26, gData_02004a18);
  Event_Wait(5);
  I_Actor_ResetAndSetPositionInMode2(0, 776, 202);
  I_Actor_ResetAndSetPositionInMode2(2, 731, 232);
  Actor_SetMovementTypeById(0, gData_02004a34);
  Actor_SetMovementTypeById(2, gData_02004a50);
  Actor_RefreshSelectorById(0);
  Actor_RefreshSelectorById(2);
  Actor_RefreshSelectorById(26);
  Actor_RefreshSelectorById(27);
  Event_Wait(40);
  Func_02000b48();
  Event_Wait(200);
  Camera_FollowActor(4, 1);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(28, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(28, 0, 5);
  I_Func_020048cc(6, 0x109, 30);
  Event_ShowMessageThenWait(6, 0, 5);
  Func_080d47b4(7, 0x107, 30);
  Event_ShowMessageThenWait(7, 0, 5);
  Func_080d47b4(5, 0x109, 30);
  Event_ShowMessageThenWait(5, 0, 5);
  Actor_ArmCallbackAndRefresh(28, FACING_SOUTH);
  Event_Wait(10);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 4 of 1568 instructions differ in tla-en. */
  do {
    Event_ShowMessageThenWait(28, 0, 5);
  } while (0);
  Actor_SetModeById(4, 3);
  Event_Wait(20);
  Event_Wait(10);
  Actor_SetAngleToward(5, 4, 0);
  Actor_SetAngleToward(7, 4, 0);
  Actor_SetAngleToward(6, 4, 0);
  Event_Wait(20);
  Actor_SetModeById(7, 3);
  /* FAKEMATCH(one-pass): the native starts a scheduling region here; as
   * plain statements, 2 of 1568 instructions differ in tla-en. */
  do {
    Actor_SetModeById(6, 3);
  } while (0);
  Actor_SetModeById(5, 3);
  Event_Wait(20);
  Event_Wait(10);
  Actor_SetModeById(7, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_ResetAndSetPosition(7, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_SetModeById(6, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_ResetAndSetPosition(6, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_SetModeById(5, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_ResetAndSetPosition(5, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_SetModeById(28, 2);
  object = Actor_GetById(4);
  if (object != NULL)
    Actor_ResetAndSetPosition(28, ((s16 *)&object->x)[1],
                              ((s16 *)&object->z)[1]);
  Actor_SetMovementTypeById(7, gData_0200599c);
  Actor_SetMovementTypeById(6, gData_0200599c);
  Actor_SetMovementTypeById(5, gData_0200599c);
  Actor_SetMovementTypeById(28, gData_0200599c);
  Actor_RefreshSelectorById(7);
  Actor_RefreshSelectorById(6);
  Actor_RefreshSelectorById(5);
  Actor_RefreshSelectorById(28);
  Func_020047dc();
}
