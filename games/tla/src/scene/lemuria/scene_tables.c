#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "scene_tables.h"
#include "event_inline.h"

extern const u32 gSceneExits[];

struct WarpTarget {
  s32 scene;
  u16 x;
  u16 z;
};

s32 ArcTan2(s32, s32);
void Animation_SetIndexAndInitializeObjects(struct Actor *object, s32 index);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001b7c(struct Actor *object, s32 mode);
void Func_02001ba4(struct Actor *object, s32 value);
void Func_02001bcc(void);
void Func_02001bd4(void);
void Func_02001cf4(s32 a, s32 b, s32 c);
void Func_02001d44(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
s32 Func_02001d54(struct Actor *object);
s32 Func_02001d5c(s32 x, s32 z, s32 layer);
void Func_02001d64(s32 x, s32 z, s32 layer, s32 value);

extern const struct WarpTarget gData_020021bc;
extern const struct WarpTarget gData_020021c4;
extern const struct WarpTarget gData_020021cc;
extern const struct WarpTarget gData_020021d4;
extern const struct WarpTarget gData_020021dc;
extern const struct WarpTarget gData_020021e4;
extern const struct WarpTarget gData_020021ec;

extern u8 MsgTheDoorIs;
extern u8 MsgItHasBeen;
extern u8 MsgEveryoneSpoilsMe;
extern u8 MsgWeAreThe;
extern u8 MsgAreThereOther;

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_ArmCallback_(s32 id, s32 angle, s32 flags) {
  Actor_ArmCallback(id, angle, flags);
}

static __inline__ void Actor_CenterOnColumn_(s32 id, s32 column, s32 offset) {
  Actor_CenterOnColumn(id, column, offset);
}

static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx,
                                                           s32 dz) {
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_020001bc(void) {
  Actor_SetPosition(12, PIXELS(488), PIXELS(1256));
  Animation_SetIndexAndInitializeObjects(Actor_Get(12), 157);
  Actor_SetAnimation(12, 3);
  Actor_ArmCallback_(12, FACING_WEST, 0);
}

void Func_020001f0(const struct WarpTarget *warp, s32 door, s32 mode) {
  s32 id;

  Func_02001bcc();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(158);
  MapGrid_AnimateCells(warp->scene, warp->x, warp->z);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  id = gMainRoutineWork.currentOwner;
  if (mode == 0)
    Actor_CenterOnColumn_(id, 2, -8);
  else
    Actor_OffsetPositionAndResetMotion_(id, 0, -8);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001bd4();
}

void Func_02000278(s32 door) {
  const struct WarpTarget *warp = &gData_020021bc;
  s32 mode = 0;

  if (door == 3 && Event_GetFlag(0x96a)) {
    FieldEvent_SetMessageId((s32)&MsgTheDoorIs);
    Event_ShowMessage(-1, 0);
    return;
  }
  if (door == 1) {
    warp = &gData_020021bc;
    mode = 0;
  }
  if (door == 2) {
    warp = &gData_020021c4;
    mode = 0;
  }
  if (door == 3) {
    warp = &gData_020021cc;
    mode = 1;
  }
  if (door == 12) {
    warp = &gData_020021d4;
    mode = 0;
  }
  if (door == 11) {
    warp = &gData_020021dc;
    mode = 1;
  }
  if (door == 10) {
    warp = &gData_020021e4;
    mode = 1;
  }
  if (door == 16) {
    warp = &gData_020021ec;
    mode = 0;
  }
  Func_020001f0(warp, door, mode);
}
