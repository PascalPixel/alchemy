#include "scene_tables.h"
#include "scene.h"
#include "scene_ids.h"

void Actor_SetHorizontalPositionWithTerrain(s32 actor, s32 x, s32 z);
void Actor_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);

extern const struct WarpTarget gData_02003d18;

static __inline__ void Actor_CommitPositionAndActivate_(s32 actor,
                                                               s32 dx, s32 dz) {
  Actor_CommitPositionAndActivate(actor, dx, dz);
}

static __inline__ void Actor_SetSpeedParameters_(s32 actor, s32 speed,
                                                        s32 acceleration) {
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Func_020034d4_(s32 actor, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(actor, x, z);
}

void Func_02000208(void) {
  const struct WarpTarget *warp;
  s32 msg;

  warp = &gData_02003d10;
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 168, 424);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_EAST);
  Sound_Play(158);
  Map_PlayMapEdit(warp->scene, warp->x, warp->z);
  Actor_GetById(8)->motionFlags = 0;
  Func_020034d4_(8, 184 << 16, 204 << 17);
  Actor_SetSpeedParameters_(8, 0x1cccc, 0xe666);
  Actor_CommitPositionAndActivate(8, 0, 16);
  Actor_ArmCallbackAndRefresh(8, FACING_WEST);
  msg = (s32)MsgHasMyGrandson;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(8, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(8, 0);
  Actor_SetSpeedParameters_(8, 0xcccc, 0x6666);
  Actor_CommitPositionAndActivate_(8, 0, -14);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  warp = &gData_02003d18;
  Map_PlayMapEdit(warp->scene, warp->x, warp->z);
  Sound_Play(158);
}
