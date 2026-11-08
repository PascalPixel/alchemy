#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "scene_tables.h"

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ActorPlacement gActorPlacements[];

struct WarpTarget {
  s32 scene;
  u16 x;
  u16 z;
};

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
s32 ArcTan2(s32, s32);
struct Actor *Actor_GetById(s32 id);
void Animation_SetIndexAndInitializeObjects(struct Actor *object, s32 index);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_CenterOnColumn(s32 id, s32 column, s32 offset);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Actor_TurnToFaceEachOther(s32 id, s32 other, s32 mode);
void Actor_SetModeById(u32 id, s32 mode);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001b7c(struct Actor *object, s32 mode);
void Func_02001b84(s32 scene, s32 x, s32 z);
void Func_02001ba4(struct Actor *object, s32 value);
void Func_02001bcc(void);
void Func_02001bd4(void);
void Func_02001c2c(s32 id, s32 x, s32 z);
void Func_02001c6c(s32 message);
void Func_02001c7c(s32 id, s32 a, s32 b);
void Func_02001c84(s32 a, s32 b);
void Func_02001cac(s32 id, s32 a);
void Func_02001cd4(s32 a);
void Func_02001cf4(s32 a, s32 b, s32 c);
void Func_02001d04(s32 a);
void Func_02001d44(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
s32 Func_02001d54(struct Actor *object);
s32 Func_02001d5c(s32 x, s32 z, s32 layer);
void Func_02001d64(s32 x, s32 z, s32 layer, s32 value);
void Func_02001d7c(s32 a);

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

static __inline__ void Actor_ArmCallback_(s32 id, s32 angle, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(id, angle, flags);
}

static __inline__ void Actor_CenterOnColumn_(s32 id, s32 column, s32 offset) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_CenterOnColumn(id, column, offset);
}

static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx,
                                                           s32 dz) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_020001bc(void) {
  Func_02001c2c(12, PIXELS(488), PIXELS(1256));
  Animation_SetIndexAndInitializeObjects(Actor_GetById(12), 157);
  Actor_SetModeById(12, 3);
  Actor_ArmCallback_(12, FACING_WEST, 0);
}

void Func_020001f0(const struct WarpTarget *warp, s32 door, s32 mode) {
  s32 id;

  Func_02001bcc();
  Func_02001d04(0);
  Func_02001d7c(158);
  Func_02001b84(warp->scene, warp->x, warp->z);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  id = gMainRoutineWork.currentOwner;
  if (mode == 0)
    Actor_CenterOnColumn_(id, 2, -8);
  else
    Actor_OffsetPositionAndResetMotion_(id, 0, -8);
  FieldEvent_Wait(10);
  Func_02001cd4(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001bd4();
}

void Func_02000278(s32 door) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 18 of 52 instructions
   * differ in tla-en. */
  const struct WarpTarget *warp = &gData_020021bc;
  s32 mode = 0;

  if (door == 3 && GameFlag_Test(SPLIT_CONSTANT(0x96a))) {
    Func_02001c6c((s32)&MsgTheDoorIs);
    Func_02001c84(-1, 0);
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
