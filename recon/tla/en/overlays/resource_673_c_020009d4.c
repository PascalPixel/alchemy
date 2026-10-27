// Draft, differ 126 of 388: a Gabomba Catacombs scene: the camera moves to the
// statue (cue 26), object 8 starts (cue 0x240), MsgByReachingThis is shown
// and object 0x40 is raised 16 pixels (cue 220) while a glow (object type
// 412, its update Func_020009bc) sits above it for 15 frames; the leader
// and object 9 (the follower) are placed side by side, turn, emote (258,
// 261, 265, 257) and talk in turn, each line the next of the event work's
// halfword 0x1c4 (Func_0200118c), then object 9 joins the leader again;
// sets flag 0x9ce.
// Left: registers: the native saves only r8 to r10 (the statue in r6, a 0 shared by the glow's stores in r8, then 0x10000 in r6 for the follower's speed limit, &gPartyState.currentOwner in r5), where the draft also saves r11 and r7.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

extern const u8 MsgByReachingThis[];
extern const u8 gData_020012a4[];
struct ObjectRuntime *Object_GetById(u32 id);
void Func_0200119c(void);
void Func_020011a4(void);
void Func_0200125c(s32 skip);
void Func_02001244(s32 a, s32 b);
s32 Func_0200123c(s32 a, s32 b);
s32 Func_0200124c(s32 frames);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02001284(s32 cue);
void FieldEvent_WaitFrames(s32 frames);
void Func_020011e4(const u8 *message);
void Func_0200118c(s32 line, s32 a, s32 b);
void Func_020011cc(s32 actor, s32 x, s32 z);
struct ObjectRuntime *Func_0200115c(s32 type, s32 x, s32 y, s32 z);
void Func_02001154(struct ObjectRuntime *object, const u8 *script);
void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Func_020009bc(void);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Func_020011d4(s32 actor, s32 target);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_020011ec(s32 actor, s32 a);
s32 Func_020011fc(s32 actor, s32 emote, s32 frames);
void GameFlag_SetBit(s32 flag);
s32 Func_02001264(void);
void Func_02001214(s32 actor, s32 a);

#define TALK() Call3(Func_0200118c, (s16)(*(u16 *)(work + 0x1c4))++, 0, -3)

void Func_020009d4(void) {
  struct ObjectRuntime *follower = Object_GetById(9);
  struct ObjectRuntime *statue = Object_GetById(0x40);
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;
  struct ObjectRuntime *glow, *leader;

  Func_0200119c();
  Func_0200125c(0);
  Call2(Func_02001244, 0x10000, 0);
  Func_0200123c(0x403108, 1);
  Func_0200124c(60);
  Call4(Camera_MoveWithinMapBounds, 0xa80000, -1, 0x9e0000, 1);
  Func_02001284(26);
  FieldEvent_WaitFrames(60);
  Object_SetModeById(8, 2);
  Func_02001284(0x240);
  FieldEvent_WaitFrames(60);
  Func_020011e4(MsgByReachingThis);
  TALK();
  Call3(Func_020011cc, 0x40, 0xa80000, 0x980000);
  statue->motionFlags = 6;
  statue->terrainHeight += 0x100000;
  Func_02001284(220);
  glow = Func_0200115c(412, 0, 0, 0);
  Func_02001154(glow, gData_020012a4);
  Animation_ApplyChildValues(glow, 1);
  glow->motionFlags = 0;
  glow->x = statue->x;
  glow->y = statue->y + 0x80000;
  glow->z = statue->z;
  glow->terrainHeight = statue->terrainHeight;
  glow->update = Func_020009bc;
  FieldEvent_WaitFrames(15);
  glow->x = 0;
  glow->y = 0;
  glow->z = 0;
  glow->update = 0;
  TALK();
  ObjectMotion_ResetAndSetPosition(gPartyState.currentOwner, 166, 188);
  ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
  Func_020011d4(9, gPartyState.currentOwner);
  follower->speedLimit = 0x10000;
  ObjectMotion_ResetAndSetPosition(9, 184, 188);
  ObjectMotion_CommitCurrentPositionAndActivate(9);
  Call3(ObjectMotion_ArmCallback, 9, 0x8000, 0);
  Func_020011ec(9, 0);
  Call3(ObjectMotion_ArmCallback, gPartyState.currentOwner, 0, 0);
  Object_SetModeById(gPartyState.currentOwner, 3);
  Call3(ObjectMotion_ArmCallback, gPartyState.currentOwner, 0xc000, 0);
  TALK();
  Func_020011fc(9, 258, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0xb000, 0);
  FieldEvent_WaitFrames(30);
  Func_020011ec(9, 0);
  TALK();
  Func_020011fc(9, 261, 0);
  FieldEvent_WaitFrames(30);
  TALK();
  Func_020011fc(9, 265, 0);
  FieldEvent_WaitFrames(30);
  Func_020011ec(9, 0);
  TALK();
  Call3(ObjectMotion_ArmCallback, gPartyState.currentOwner, 0, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0x8000, 0);
  FieldEvent_WaitFrames(10);
  Func_020011fc(gPartyState.currentOwner, 257, 0);
  Func_020011fc(9, 257, 0);
  FieldEvent_WaitFrames(60);
  TALK();
  Call3(ObjectMotion_ArmCallback, gPartyState.currentOwner, 0xc000, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0xb000, 0);
  Func_020011fc(gPartyState.currentOwner, 261, 0);
  Func_020011fc(9, 261, 0);
  FieldEvent_WaitFrames(60);
  TALK();
  FieldEvent_WaitFrames(40);
  Object_SetModeById(gPartyState.currentOwner, 3);
  FieldEvent_WaitFrames(40);
  Object_SetModeById(9, 3);
  Func_020011ec(9, 0);
  Object_SetModeById(8, 2);
  TALK();
  FieldEvent_WaitFrames(30);
  Object_SetModeById(8, 4);
  Object_SetModeById(9, 2);
  leader = Object_GetById(gPartyState.currentOwner);
  if (leader != 0)
    ObjectMotion_ResetAndSetPosition(9, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  ObjectMotion_CommitCurrentPositionAndActivate(9);
  Func_020011cc(9, 0, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(9);
  GameFlag_SetBit(0x9ce);
  Func_02001264();
  Value2(Func_0200123c, 0x10000, 0);
  Func_0200124c(60);
  Func_02001214(gPartyState.currentOwner, 1);
  FieldEvent_WaitFrames(60);
  Func_020011a4();
}
