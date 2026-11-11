#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

struct VenusActor {
  u8 unk00[0xa];
  s16 xPixel;
  u8 unk0c[6];
  s16 zPixel;
};

void Func_02001c4c(void);
void Func_02001cd4(s32 actor);
void Func_02001ccc(s32 actor, s32 value);
void Party_RemoveActiveOwner(s32 owner);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_StartScreenTransition(void);
void VenusLighthouse_RunSetupStep(void);
void SceneState_RunActor13AtColumn42Setup(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void Venus_ObjWalk(s32 actor, s32 x, s32 z) {
  Actor_SetPositionAndReset(actor, x, z);
}

static inline void Venus_ObjTurn(s32 actor, s32 angle, s32 frames) {
  Actor_ArmCallback(actor, angle, frames);
}

static inline void Venus_ObjWalk2(s32 actor, s32 x, s32 z) {
  Actor_ResetAndSetPositionInMode2(actor, x, z);
}

static inline void Venus_Jump(s32 actor, s32 height, s32 frames) {
  /* FAKEMATCH(pin): the height pinned to r1, so the actor loads after the
   * height and frames as the native does; as Func_080d47b4(actor, height,
   * frames). */
  register s32 h __asm__("r1") = height;
  /* FAKEMATCH(pin): the frames pinned to r2, for the same order; as
   * Func_080d47b4(actor, height, frames). */
  register s32 f __asm__("r2") = frames;

  Func_080d47b4(actor, h, f);
}

static inline void Venus_CamMove(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static inline void Venus_MessagePut(s32 actor, s32 value) {
  Event_ShowMessageAndWait(actor, value);
}

static inline void Venus_ObjSetPos(s32 actor, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(actor, x, z);
}

static inline void Venus_ObjSpeed(s32 actor, s32 speed, s32 acceleration) {
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static inline void Venus_ObjCommit(s32 actor, s32 x, s32 z) {
  Actor_CommitPositionAndActivate(actor, x, z);
}

static inline void Venus_ObjEffectSet(s32 actor, s32 value) {
  Func_02001ccc(actor, value);
}

void Func_02000b2c(void) {
  struct EventWork *work;

  Venus_ObjSetPos(4, PIXELS(712), PIXELS(816));
  Venus_ObjSetPos(5, PIXELS(696), PIXELS(840));
  Venus_ObjSetPos(8, PIXELS(728), PIXELS(840));
  Venus_ObjTurn(4, FACING_SOUTH, 0);
  Venus_ObjTurn(5, FACING_NORTH, 0);
  Venus_ObjTurn(8, FACING_NORTH, 0);
  Venus_ObjSpeed(4, 0x10000, 0x8000);
  Venus_ObjSpeed(5, 0x10000, 0x8000);
  Venus_ObjSpeed(8, 0x10000, 0x8000);
  Venus_ObjSpeed(9, 0x10000, 0x8000);
  Venus_CamMove(PIXELS(664), -1, PIXELS(824), 0);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 256;
  work->delay = 60;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  Venus_ObjWalk(4, 664, 824);
  Venus_ObjTurn(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Venus_ObjWalk(8, 712, 824);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  VenusLighthouse_RunSetupStep();
  SceneState_RunActor13AtColumn42Setup();
  FieldEvent_Wait(20);
  Venus_ObjWalk(8, 728, 840);
  Venus_ObjTurn(8, FACING_NORTH, 0);
  Venus_ObjTurn(5, FACING_NORTH, 0);
  Venus_ObjWalk(4, 712, 816);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  FieldActor_WatchAndSetScript(5, 4);
  FieldActor_WatchAndSetScript(8, 4);
  Venus_ObjWalk(4, 744, 816);
  Actor_WalkToAndWait(4, 744, 780);
  Event_PlaySound(129);
  Func_02001ccc(4, -1);
  FieldEvent_Wait(50);
  Venus_ObjTurn(5, FACING_NORTH, 0);
  Venus_ObjTurn(8, FACING_NORTH, 0);
  Actor_SetPosition(4, 0, 0);
}

void Func_02000d30(void) {
  struct VenusActor *actor;
  struct EventWork *work;

  FieldEvent_SetMessageId((s32)MsgVenusLighthouseAreYouSureWeShouldBeLeaving);
  FieldCamera_FollowActor(8, 1);
  Func_02001c4c();
  FieldEvent_Wait(30);
  Venus_ObjWalk(5, 664, 840);
  Actor_FaceDirection(5, 0, 0);
  Venus_ObjWalk(8, 696, 840);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(10);
  Venus_ObjWalk2(8, 696, 920);
  FieldEvent_Wait(10);
  Venus_ObjWalk(5, 664, 888);
  FieldActor_WaitForMoveById(8);
  Venus_Jump(8, 256, 30);
  Venus_ObjTurn(8, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Venus_ObjWalk(8, 696, 888);
  Actor_FaceDirection(5, 0, 0);
  Venus_ObjTurn(8, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Venus_CamMove(PIXELS(696), -1, PIXELS(920), 1);
  Func_02001c4c();
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Venus_Jump(8, 257, 50);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Venus_Jump(8, 257, 30);
  Venus_MessagePut(0x2008, 0);
  FieldEvent_Wait(10);
  Venus_ObjTurn(5, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Venus_Jump(8, 258, 40);
  Venus_MessagePut(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(30);
  Event_PlaySound(198);
  Venus_ObjSetPos(9, PIXELS(680), PIXELS(920));
  Venus_ObjTurn(9, 0xb000, 0);
  Func_02001cd4(9);
  Venus_ObjTurn(9, 0xb000, 0);
  FieldEvent_Wait(20);
  Venus_ObjTurn(5, FACING_SOUTH, 0);
  Venus_ObjTurn(8, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  FieldActor_StartRepeatedMotion(5, 2);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(20);
  Actor_FaceEachOther(5, 8, 40);
  Venus_Jump(9, 256, 30);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Venus_ObjTurn(5, FACING_SOUTH, 0);
  Venus_ObjTurn(8, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Venus_Jump(8, 258, 40);
  Venus_MessagePut(0x2008, 0);
  FieldEvent_Wait(10);
  Venus_Jump(9, 257, 30);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0, 0);
  FieldEvent_Wait(30);
  Venus_ObjTurn(9, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(9, FACING_NORTH, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(20);
  Venus_ObjSpeed(5, 0x8000, 0x4000);
  FieldActor_WatchAndSetScript(8, 5);
  Actor_WalkByAndWait(5, 0, -32);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Venus_Jump(9, 261, 30);
  Venus_ObjCommit(9, -8, -8);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  Venus_ObjTurn(8, 0x5000, 0);
  FieldEvent_Wait(20);
  Venus_MessagePut(0x2008, 0);
  FieldEvent_Wait(10);
  Venus_Jump(9, 256, 30);
  Venus_ObjTurn(9, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Venus_Jump(9, 257, 30);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Venus_Jump(8, 258, 0);
  Venus_Jump(5, 258, 40);
  Venus_MessagePut(0x2008, 0);
  FieldEvent_Wait(10);
  Venus_Jump(9, 264, 30);
  Venus_ObjTurn(9, 0xf000, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(20);
  Venus_Jump(5, 262, 40);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Venus_ObjTurn(9, 0xb000, 0);
  FieldEvent_Wait(20);
  Venus_ObjTurn(5, 0x3000, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Venus_Jump(9, 265, 30);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_Wait(10);
  Venus_ObjSpeed(5, 0x10000, 0x8000);
  Actor_WalkByAndWait(5, 0, 16);
  Venus_ObjTurn(5, 0x3000, 0);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Venus_Jump(5, 263, 30);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(9, 7);
  FieldEvent_Wait(30);
  Venus_ObjTurn(8, 0x9000, 0);
  FieldEvent_Wait(20);
  Venus_Jump(8, 258, 40);
  Venus_MessagePut(0x2008, 0);
  FieldEvent_Wait(10);
  Venus_ObjTurn(5, 0x1000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(5, 4);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Venus_ObjTurn(5, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(5, 0, 16);
  Venus_ObjTurn(5, 0x3000, 0);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Venus_Jump(5, 261, 50);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  Venus_MessagePut(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(5, 0, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Venus_ObjTurn(5, 0x3000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(30);
  Venus_Jump(8, 258, 40);
  Venus_MessagePut(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(5, 0, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Venus_Jump(8, 265, 30);
  Venus_MessagePut(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Venus_Jump(5, 258, 40);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Venus_Jump(8, 261, 30);
  Venus_MessagePut(0x2008, 0);
  FieldEvent_Wait(10);
  Venus_Jump(5, 263, 30);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Venus_ObjTurn(8, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  Venus_MessagePut(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(8, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Venus_ObjCommit(5, 0, -16);
  FieldEvent_Wait(10);
  Venus_ObjTurn(5, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Venus_ObjTurn(9, FACING_NORTHEAST, 0);
  Venus_ObjTurn(8, 0x5000, 0);
  FieldEvent_Wait(50);
  Venus_ObjTurn(8, 0x9000, 0);
  Actor_FaceDirection(9, 0xd000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(5, 0x1000, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Venus_ObjTurn(9, 0xb000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Venus_Jump(5, 256, 30);
  Actor_FaceDirection(5, 0x3000, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Venus_ObjTurn(9, 0xf000, 0);
  FieldEvent_Wait(25);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Venus_ObjTurn(8, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  Venus_MessagePut(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Venus_Jump(5, 258, 40);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(9, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_FaceEachOther(5, 8, 50);
  Venus_ObjTurn(9, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  Venus_Jump(9, 264, 30);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(9, 6);
  FieldEvent_Wait(30);
  Venus_Jump(8, 257, 30);
  FieldEvent_Wait(10);
  Actor_FaceDirection(8, 0x5000, 0);
  FieldEvent_Wait(20);
  Venus_MessagePut(0x2008, 0);
  FieldEvent_Wait(10);
  Venus_Jump(9, 256, 30);
  Venus_ObjTurn(9, 0xf000, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Venus_Jump(5, 257, 30);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Venus_Jump(9, 258, 40);
  Venus_ObjTurn(9, 0xb000, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0xf000, 0);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(10);
  Venus_MessagePut(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0xb000, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(9, 4);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Event_ShowMessage(9, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(8, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Event_ShowMessage(0x2008, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(5, 0x1000, 0);
  FieldEvent_Wait(30);
  Venus_Jump(5, 261, 30);
  Event_ShowMessage(5, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimation(9, 3);
  Actor_SetAnimationAndWait(8, 3);
  FieldEvent_Wait(20);
  Venus_ObjSpeed(8, 0x13333, 0x9999);
  Actor_SetAnimation(8, 2);
  actor = (struct VenusActor *)Actor_Get(5);
  if (actor != NULL)
    Actor_SetDestination(8, actor->xPixel, actor->zPixel);
  FieldActor_WaitForMoveById(8);
  Actor_SetPosition(8, 0, 0);
  Venus_ObjSpeed(9, 0x13333, 0x9999);
  Actor_SetAnimation(9, 2);
  actor = (struct VenusActor *)Actor_Get(5);
  if (actor != NULL)
    Actor_SetDestination(9, actor->xPixel, actor->zPixel);
  FieldActor_WaitForMoveById(9);
  Actor_SetPosition(9, 0, 0);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(5, 16, 0);
  Venus_ObjWalk(5, 680, 944);
  FieldEvent_Wait(15);
  Actor_FaceDirection(5, 0xd000, 0);
  FieldEvent_Wait(50);
  Actor_FaceDirection(5, 0, 0);
  FieldEvent_Wait(15);
  Venus_ObjWalk(5, 728, 944);
  Venus_ObjWalk(5, 728, 928);
  Actor_SetPosition(5, PIXELS(728), PIXELS(912));
  Event_PlaySound(129);
  Venus_ObjEffectSet(5, -1);
  FieldEvent_Wait(40);
  MapGrid_CopyCellsTo(53, 50, 42, 49, 1, 1);
  MapGrid_CopyCellsTo(55, 117, 41, 117, 3, 5);
  Event_ClearFlag(513);
  Venus_ObjSetPos(10, PIXELS(664), PIXELS(792));
  Venus_CamMove(-1, -1, -1, 0);
  gMainRoutineWork.currentOwner = 5;
  Party_RemoveActiveOwner(4);
  FieldEvent_Wait(50);
  work = Ram_HeapSlots->eventWork;
  work->delay = 60;
  work->screenEffect = 256;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(22);
}
