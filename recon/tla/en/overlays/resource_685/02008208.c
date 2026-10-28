// differ 2: the native loads the first warp target into r6 two instructions
// after the owner address (between the halves of the constant 424); here that
// load has the lowest priority before the first call and comes last.
#include "../../../../../games/tla/src/scene/champa_cliff_caves/scene.h"

extern const struct WarpTarget gData_02003d18;

void Actor_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);
void Func_020034d4(s32 actor, s32 x, s32 z);

static __inline__ void Actor_CommitPositionAndActivate_(s32 actor,
                                                               s32 dx, s32 dz) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Actor_CommitPositionAndActivate(actor, dx, dz);
}

static __inline__ void Actor_SetSpeedParameters_(s32 actor, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Func_020034d4_(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_020034d4(actor, x, z);
}

void Func_02000208(void) {
  const struct WarpTarget *warp = &gData_02003d10;
  s32 msg;

  Actor_SetPositionAndReset(gAdventure.currentOwner, 168, 424);
  Func_02003534(gAdventure.currentOwner, 0);
  Func_0200362c(158);
  Func_02003414(warp->scene, warp->x, warp->z);
  Actor_GetById(8)->motionFlags = 0;
  {
    s32 x = 184 << 16;

    Func_020034d4_(8, x, 408 << 16);
  }
  Actor_SetSpeedParameters_(8, 0x1cccc, 0xe666);
  Actor_CommitPositionAndActivate(8, 0, 16);
  Func_02003534(8, 0x8000);
  msg = (s32)MsgHasMyGrandson;
  Func_0200350c(msg);
  FieldMessage_ShowAtActor(8, 0);
  if (Event_AskYesNoWithActorAnimation(Func_020035f4(), 0) == 0) {
    FieldEvent_WaitFrames(10);
    Func_0200350c(msg + 1);
  } else {
    FieldEvent_WaitFrames(20);
    Func_0200350c(msg + 2);
  }
  Func_02003524(8, 0);
  Actor_SetSpeedParameters_(8, 0xcccc, 0x6666);
  Actor_CommitPositionAndActivate_(8, 0, -14);
  Func_020034d4(8, 0, 0);
  warp = &gData_02003d18;
  Func_02003414(warp->scene, warp->x, warp->z);
  Func_0200362c(158);
}
