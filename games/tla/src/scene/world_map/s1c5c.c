#include "types.h"
#include "actor.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

extern const u8 gData_02004e38[];
extern const u8 gData_02004e58[];
extern const u8 gData_02004e88[];

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004e00(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_SetSpeedParameters_(s32 actor, s32 speed,
                                                        s32 acceleration) {
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Actor_ArmCallback_(s32 actor, s32 angle,
                                                 s32 frames) {
  Actor_ArmCallback(actor, angle, frames);
}

static __inline__ void Func_02004c90_(s32 actor, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(actor, x, z, angle);
}

void Func_02001c5c(void) {
  s32 angle;

  Func_02004c20();
  Event_ResolvePendingActions(0);
  Actor_GetById(16)->motionFlags = 2;
  Actor_GetById(17)->motionFlags = 2;
  Actor_GetById(18)->motionFlags = 2;
  /* FAKEMATCH: held in a local, the facing angle is built once and shared by
   * the three placements as in the native; as a literal each forwarded call
   * builds it again. */
  angle = 0xe000;
  Func_02004c90_(16, PIXELS(8913), PIXELS(5853), angle);
  Func_02004c90_(17, PIXELS(8929), PIXELS(5869), angle);
  Func_02004c90_(18, PIXELS(8929), PIXELS(5885), angle);
  Event_StartScreenTransition();
  Actor_SetSpeedParameters_(16, 0x3333, 0x1999);
  Actor_SetSpeedParameters_(17, 0x3333, 0x1999);
  Actor_SetSpeedParameters_(18, 0x3333, 0x1999);
  Actor_OffsetPositionAndReset(16, 10, 0);
  Actor_OffsetPositionAndReset(17, 10, 0);
  Actor_OffsetPositionAndReset(18, 10, 0);
  Event_WaitTransitionFrames();
  Func_080d46a4(0x13333, 6);
  Func_02004e00();
  Func_02004da8();
  Actor_WaitForMoveById(16);
  Event_Wait(40);
  Actor_SetModeById(16, 3);
  Event_SetMessageId((s32)MsgYouDonT);
  Actor_ArmCallback_(17, 0xd000, 0);
  Actor_ArmCallback_(18, 0xd000, 0);
  Event_ShowMessageAndWait(16, 0);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(16, 0);
  Actor_SetModeById(16, 3);
  Actor_SetModeById(4, 3);
  Event_Wait(60);
  Actor_WatchAndSetScript(4, 18);
  Actor_SetSpeedParameters_(16, 0x6666, 0x3333);
  Actor_SetSpeedParameters_(17, 0x6666, 0x3333);
  Actor_SetSpeedParameters_(18, 0x6666, 0x3333);
  Actor_SetMovementTypeById(16, gData_02004e38);
  Event_Wait(40);
  Actor_SetMovementTypeById(17, gData_02004e58);
  Actor_SetMovementTypeById(18, gData_02004e88);
  Actor_RefreshSelectorById(16);
  Actor_RefreshSelectorById(17);
  Actor_RefreshSelectorById(18);
  Func_02004db0();
  Event_Wait(30);
  Func_080d46a4(0x10000, 6);
  Func_02004c28();
}
