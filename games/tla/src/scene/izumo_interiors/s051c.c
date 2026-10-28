#include "types.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern const u8 gData_02002b40[];
extern const u8 gData_02002cf0[];
extern const u8 gData_02002d80[];
extern const u8 gData_02002f78[];
extern const u8 gData_02003164[];
extern const u8 gData_0200323c[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void FieldEvent_WaitFrames(s32 frames);
void Func_020021c4(void);
void Func_020021cc(void);
void Func_020022bc(s32 skip);
void Func_02002244(s32 message);
void Func_02002254(s32 speaker, s32 flags);
s32 Func_0200226c(s32 actor, s32 animation, s32 frames);
s32 Func_020022dc(void);
void Func_02002304(s32 slot, s32 actor);
void Func_0200230c(s32 actor);
void Func_02002314(s32 slot, s32 actor);
void Func_0200061c(s32 mode);
void ObjectMotion_SetVariantCallback(s32 actor, s32 variant);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_OffsetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
s32 PartyInventory_FindOwner(s32 item);
void UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);

static inline s32 FlagCheck(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static inline void FlagSet(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static inline void ActorTurn(s32 actor, s32 angle, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void ActorAnimate(s32 actor, s32 animation, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200226c(actor, animation, frames);
}

static inline void LeaderAnimate(s32 animation, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200226c(gAdventure.currentOwner, animation, frames);
}

s32 Func_0200051c(void) {
  Func_020021c4();
  Func_020022bc(0);
  if (!FlagCheck(0x925)) {
    Func_02002244((s32)MsgSusaDonT);
    Func_02002254(17, 0);
    FieldEvent_WaitFrames(60);
    Func_02002254(17, 0);
    FieldEvent_WaitFrames(60);
    FieldEvent_WaitFrames(15);
    Func_02002254(17, 0);
    FieldEvent_WaitFrames(60);
    FieldEvent_WaitFrames(15);
    Func_02002254(17, 0);
    FieldEvent_WaitFrames(60);
    ObjectMotion_SetVariantCallback(17, 1);
    FieldEvent_WaitFrames(30);
    Func_02002254(17, 0);
    FieldEvent_WaitFrames(60);
    FlagSet(0x925);
  } else {
    Func_02002244((s32)MsgSusaPleaseDon);
    Func_02002254(17, 0);
  }
  Object_GetById(gAdventure.currentOwner)->actionFlags &= ~1;
  ObjectMotion_OffsetPositionAndReset(gAdventure.currentOwner, 0, 16);
  ObjectMotion_CommitCurrentPositionAndActivate(gAdventure.currentOwner);
  Object_GetById(gAdventure.currentOwner)->actionFlags |= 1;
  Func_020021cc();
  return 0;
}

void Func_02000604(void) { Func_0200061c(0); }

void Func_02000610(void) { Func_0200061c(1); }

void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 follow);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Func_02002274(s32 actor, s32 animation);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 x, s32 z);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void ObjectMotion_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);
void Func_020022cc(s32 a, s32 b);
void Object_LinkPair(s32 firstId, s32 secondId, s32 wait);
void Func_02001d8c(void);
void Func_02002214(s32 actor, s32 x, s32 z);
void Camera_FollowActor(s32 id, s32 flag);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speedLimit,
                                     s32 acceleration);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);

/// The scene that opens with MsgWarriorsAppearedFrom: the warriors confront
/// the party in front of persons 20, 21 and 22 through three yes-or-no
/// answers, then persons 5, 6, 7 and 23 join the leader and flags 0x928 and
/// 0x350 are set. Mode 0 and mode 1 (Func_02000604 and Func_02000610) start
/// the party at different spots.
void Func_0200061c(s32 mode) {
  struct ObjectRuntime *person, *leader;
  s32 party, owner, answer, message;

  Func_020021c4();
  Call1((void (*)())Func_020022bc, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x8f0000, -1, 0x2300000, 1);
  party = (s32)&gAdventure;
  if (mode == 0) {
    Call3((void (*)())ObjectMotion_SetPositionAndReset, *(s32 *)(party + 532),
          184, 0x238);
  } else {
    Call3((void (*)())ObjectMotion_SetPositionAndReset, *(s32 *)(party + 532),
          104, 0x238);
  }
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  Call1((void (*)())Func_02002244, (s32)MsgWarriorsAppearedFrom);
  Call2((void (*)())Func_02002254, 0x800f, 0);
  Call2((void (*)())Object_SetModeById, 22, 3);
  Call2((void (*)())Func_02002254, 0x8016, 0);
  ActorAnimate(15, 0x100, 30);
  Call2((void (*)())Func_02002254, 0x800f, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Object_SetModeById, 22, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  Call2((void (*)())Func_02002254, 0x8016, 0);
  ActorAnimate(15, 0x103, 30);
  Call2((void (*)())ObjectMotion_SetVariantCallback, 22, 3);
  Call2((void (*)())Func_02002274, 22, 0x102);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 0x8016, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 20, 0, 30);
  Call2((void (*)())Object_SetModeById, 20, 4);
  Call2((void (*)())Func_02002254, 0x8014, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 15, 0x8000, 30);
  Call2((void (*)())Object_SetModeById, 15, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 0x800f, 0);
  Call2((void (*)())Object_SetModeById, 22, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  Call2((void (*)())Func_02002254, 0x8016, 0);
  Call2((void (*)())ObjectMotion_SetVariantCallback, 20, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  Call2((void (*)())Func_02002254, 0x8014, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 15, gAdventure.currentOwner,
        0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion, 15, 0, 16);
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 15);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  ActorAnimate(15, 0x100, 30);
  if (mode == 0) {
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 15, 168, 0x218);
  } else {
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 15, 120, 0x218);
  }
  Call3((void (*)())ObjectMotion_ArmCallback, 15, 0x4000, 0);
  owner = (s32)&gAdventure;
  owner = owner + 532;
  Call3((void (*)())ObjectMotion_SetAngleToward, 20, *(s32 *)owner, 0);
  Call2((void (*)())Func_02002254, 0x800f, 0);
  ActorAnimate(15, 0x107, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 0x800f, 0);
  Call3((void (*)())ObjectMotion_Launch, 22, 4, 0);
  Call2((void (*)())Func_02002274, 22, 0x101);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 20, 0x4000, 30);
  ActorAnimate(20, 0x101, 30);
  Call2((void (*)())Func_02002254, 0x8014, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 22, 20, 0);
  Call2((void (*)())Func_02002274, 22, 0x100);
  Call2((void (*)())Func_02002254, 22, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 0x800f, 0);
  Call2((void (*)())ObjectMotion_SetVariantCallback, 20, 2);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_SetAngleToward, 20, *(s32 *)owner, 0);
  Call2((void (*)())Func_02002254, 0x8014, 0);
  ActorAnimate(15, 0x108, 30);
  Call2((void (*)())Func_02002254, 0x800f, 0);
  Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 15, 144, 0x208);
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 15);
  Call3((void (*)())ObjectMotion_ArmCallback, 15, 0x4000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 20, 0x4000, 0);
  Call2((void (*)())Object_SetModeById, *(s32 *)owner, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  if (mode == 0) {
    Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 22, 104, 0x238);
  } else {
    Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 22, 184, 0x238);
  }
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 22);
  if (mode == 0) {
    Call3((void (*)())ObjectMotion_ArmCallback, 22, 0, 0);
  } else {
    Call3((void (*)())ObjectMotion_ArmCallback, 22, 0x8000, 0);
  }
  party = (s32)&gAdventure;
  owner = party + 532;
  Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, *(s32 *)owner, 144,
        0x230);
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate,
        *(s32 *)owner);
  Call3((void (*)())ObjectMotion_ArmCallback, *(s32 *)owner, 0xc000, 0);
  Call4((void (*)())Func_020022cc, 5, 8, 24, 0xc000);
  Call4((void (*)())Func_020022cc, 6, 24, 24, 0xc000);
  Call4((void (*)())Func_020022cc, 7, -8, 24, 0xc000);
  Call4((void (*)())Func_020022cc, 23, -24, 24, 0xc000);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call2((void (*)())Func_02002254, 0x8014, 0);
  Call2((void (*)())Object_SetModeById, 23, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  Call2((void (*)())Object_SetModeById, *(s32 *)owner, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Object_SetModeById, 15, 3);
  Call2((void (*)())Object_SetModeById, 20, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_SetAngleToward, 23, 7, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  Call2((void (*)())Object_SetModeById, 7, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 7, 0);
  Call2((void (*)())Object_SetModeById, 15, 3);
  Call2((void (*)())Object_SetModeById, 20, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Object_SetModeById, 5, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 5, 0);
  Call2((void (*)())Object_SetModeById, 6, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Object_SetModeById, 15, 3);
  Call2((void (*)())Object_SetModeById, 20, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_SetAngleToward, 5, 23, 0);
  Call2((void (*)())Object_SetModeById, 23, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  Call2((void (*)())Object_SetModeById, 20, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 0x8014, 0);
  Call2((void (*)())Object_SetModeById, 6, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 6, 0);
  ActorAnimate(15, 0x101, 30);
  Call2((void (*)())Func_02002254, 0x800f, 0);
  Call3((void (*)())Object_LinkPair, 15, 20, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call3((void (*)())ObjectMotion_SetAngleToward, 15, *(s32 *)owner, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 20, *(s32 *)owner, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Object_SetModeById, 20, 4);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 0x8014, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, *(s32 *)owner, 23, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 5, 23, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 6, 23, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 7, 23, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_SetAngleToward, 23, *(s32 *)owner, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_SetAngleToward, 23, 15, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  ActorAnimate(15, 0x100, 0);
  ActorAnimate(20, 0x100, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 0x800f, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, *(s32 *)owner, 15, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 5, 15, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 6, 15, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 7, 15, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Object_SetModeById, *(s32 *)owner, 3);
  Call2((void (*)())Object_SetModeById, 5, 3);
  Call2((void (*)())Object_SetModeById, 6, 3);
  Call2((void (*)())Object_SetModeById, 7, 3);
  Call2((void (*)())Object_SetModeById, 23, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  ActorAnimate(15, 0x102, 30);
  Call2((void (*)())Func_02002254, 0x800f, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call3((void (*)())ObjectMotion_ArmCallback, 15, 0, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Func_02001d8c();
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_ArmCallback, 15, 0x4000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 0x800f, 0);
  ActorAnimate(*(s32 *)owner, 0x100, 0);
  ActorAnimate(5, 0x100, 0);
  ActorAnimate(6, 0x100, 0);
  ActorAnimate(7, 0x100, 0);
  ActorAnimate(23, 0x100, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())ObjectMotion_SetVariantCallback, 5, 1);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  Call2((void (*)())Func_02002254, 5, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 15, 20, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_SetAngleToward, 15, *(s32 *)owner, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 0x800f, 0);
  ActorAnimate(20, 0x101, 30);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Value2((s32 (*)())UiText_OpenMessageAtObject, 0x8014, 0);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 1) {
    Call1((void (*)())Func_02002244, (s32)MsgIThoughtSo);
    ActorAnimate(20, 0x100, 30);
    Call2((void (*)())Func_02002254, 0x8014, 0);
    Call2((void (*)())Object_SetModeById, 6, 3);
    Call2((void (*)())Object_SetModeById, 5, 3);
    Call1((void (*)())FieldEvent_WaitFrames, 30);
    Call3((void (*)())ObjectMotion_SetAngleToward, 7, *(s32 *)owner, 0);
    Call1((void (*)())FieldEvent_WaitFrames, 15);
    Call2((void (*)())Object_SetModeById, 7, 4);
    Call1((void (*)())FieldEvent_WaitFrames, 30);
    Call2((void (*)())Func_02002254, 7, 0);
  }
  Call1((void (*)())Func_02002244, (s32)MsgSoYouCall);
  Call2((void (*)())Object_SetModeById, 20, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 0x8014, 0);
  Call2((void (*)())Object_SetModeById, *(s32 *)owner, 3);
  Call2((void (*)())Object_SetModeById, 5, 3);
  Call2((void (*)())Object_SetModeById, 6, 3);
  Call2((void (*)())Object_SetModeById, 7, 3);
  Call2((void (*)())Object_SetModeById, 23, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Object_SetModeById, 15, 3);
  Call2((void (*)())Object_SetModeById, 20, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  ActorAnimate(20, 0x101, 30);
  Call3((void (*)())ObjectMotion_SetAngleToward, 20, *(s32 *)owner, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 0x8014, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 90);
  Call2((void (*)())Func_02002254, 21, 0);
  Call3((void (*)())Func_02002214, 21, 0x900000, 0x2a80000);
  Call3((void (*)())ObjectMotion_SetAngleToward, *(s32 *)owner, 21, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 5, 21, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 6, 21, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 7, 21, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 23, 21, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 22, 21, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 120);
  Call2((void (*)())Func_02002254, 0x800f, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Camera_FollowActor, 21, 1);
  Call1((void (*)())FieldEvent_WaitFrames, 120);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 21, 0x9999, 0x4ccc);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 21, 0x9999, 0x4ccc);
  Object_GetById(21)->priorityFlags &= ~1;
  Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 21, 144, 0x288);
  if (mode == 0) {
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 21, 88, 0x288);
    Call3((void (*)())ObjectMotion_SetAngleToward, *(s32 *)owner, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 5, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 6, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 7, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 23, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 15, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 20, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 22, 21, 0);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 21, 56, 0x268);
    Call3((void (*)())ObjectMotion_SetAngleToward, *(s32 *)owner, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 5, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 6, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 7, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 23, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 15, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 20, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 22, 21, 0);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 21, 56, 0x248);
    Object_GetById(22)->actionFlags &= ~1;
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 22, 88, 0x238);
    Call1((void (*)())FieldEvent_WaitFrames, 1);
    Object_GetById(22)->actionFlags |= 1;
    Call3((void (*)())ObjectMotion_ArmCallback, 22, 0, 0);
    Call2((void (*)())ObjectMotion_SetActionVariant, 21, 1);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 21, 104, 0x238);
    Call3((void (*)())ObjectMotion_SetAngleToward, *(s32 *)owner, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 5, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 6, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 7, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 23, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 15, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 20, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 22, 21, 0);
    Call2((void (*)())ObjectMotion_SetActionVariant, 23, 1);
    Call2((void (*)())ObjectMotion_SetActionVariant, 21, 2);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 21, 120, 0x238);
    Call2((void (*)())ObjectMotion_SetActionVariant, 21, 3);
    Call2((void (*)())ObjectMotion_SetActionVariant, 23, 3);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 21, 120, 0x218);
    Call3((void (*)())ObjectMotion_SetAngleToward, *(s32 *)owner, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 5, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 6, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 7, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 23, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 15, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 20, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 22, 21, 0);
    Call2((void (*)())Object_SetModeById, 22, 3);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 22, 56, 0x248);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 22, 88, 0x288);
    Call3((void (*)())Func_02002214, 22, 0, 0);
  } else {
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 21, 200, 0x288);
    Call3((void (*)())ObjectMotion_SetAngleToward, *(s32 *)owner, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 5, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 6, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 7, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 23, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 15, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 20, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 22, 21, 0);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 21, 232, 0x268);
    Call3((void (*)())ObjectMotion_SetAngleToward, *(s32 *)owner, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 5, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 6, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 7, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 23, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 15, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 20, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 22, 21, 0);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 21, 232, 0x248);
    Object_GetById(22)->actionFlags &= ~1;
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 22, 200, 0x238);
    Call1((void (*)())FieldEvent_WaitFrames, 1);
    Object_GetById(22)->actionFlags |= 1;
    Call3((void (*)())ObjectMotion_ArmCallback, 22, 0x8000, 0);
    Call2((void (*)())ObjectMotion_SetActionVariant, 21, 1);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 21, 184, 0x238);
    Call3((void (*)())ObjectMotion_SetAngleToward, *(s32 *)owner, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 5, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 6, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 7, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 23, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 15, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 20, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 22, 21, 0);
    Call2((void (*)())ObjectMotion_SetActionVariant, 21, 2);
    Call2((void (*)())ObjectMotion_SetActionVariant, 6, 1);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 21, 168, 0x238);
    Call2((void (*)())ObjectMotion_SetActionVariant, 21, 3);
    Call2((void (*)())ObjectMotion_SetActionVariant, 6, 3);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 21, 168, 0x218);
    Call3((void (*)())ObjectMotion_SetAngleToward, *(s32 *)owner, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 5, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 6, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 7, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 23, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 15, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 20, 21, 0);
    Call3((void (*)())ObjectMotion_SetAngleToward, 22, 21, 0);
    Call2((void (*)())Object_SetModeById, 22, 3);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 22, 232, 0x248);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 22, 200, 0x288);
    Call3((void (*)())Func_02002214, 22, 0, 0);
  }
  person = Object_GetById(21);
  {
    u8 *priority = &person->priorityFlags;
    s32 flags = *priority | 1;

    owner = (s32)&gAdventure;
    /* FAKEMATCH(barrier): the leader's address loads before the byte store, as
     the native schedules it; without it, 2 of 2325 instructions differ in
     tla-en. */
    asm("" : : "r"(owner), "r"(flags), "r"(priority));
    *priority = flags;
  }
  owner = owner + 532;
  Call3((void (*)())ObjectMotion_ArmCallback, *(s32 *)owner, 0xc000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 5, 0xc000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 6, 0xc000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0xc000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 23, 0xc000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  Call2((void (*)())Func_02002254, 20, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  ActorAnimate(20, 0x108, 30);
  Call2((void (*)())Func_02002254, 20, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 21, 20, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 21, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 21, *(s32 *)owner, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call3((void (*)())ObjectMotion_SetAngleToward, 21, 20, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 21, 0);
  Call3((void (*)())Object_LinkPair, 15, 20, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_SetAngleToward, 15, *(s32 *)owner, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 20, *(s32 *)owner, 0);
  Call2((void (*)())Func_02002254, 15, 0);
  Call2((void (*)())Object_SetModeById, 15, 3);
  Call2((void (*)())Object_SetModeById, 20, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Object_SetModeById, *(s32 *)owner, 3);
  Call2((void (*)())Object_SetModeById, 5, 3);
  Call2((void (*)())Object_SetModeById, 6, 3);
  Call2((void (*)())Object_SetModeById, 7, 3);
  Call2((void (*)())Object_SetModeById, 23, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  ActorAnimate(15, 0x101, 30);
  Call2((void (*)())Func_02002254, 15, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 21, *(s32 *)owner, 0);
  Call2((void (*)())Func_02002254, 21, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 15, *(s32 *)owner, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 20, *(s32 *)owner, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  ActorAnimate(*(s32 *)owner, 0x102, 30);
  Call2((void (*)())Object_SetModeById, 23, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  Call2((void (*)())Object_SetModeById, 15, 3);
  Call2((void (*)())Object_SetModeById, 20, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Object_SetModeById, 23, 3);
  Call2((void (*)())Func_02002254, 23, 0);
  ActorAnimate(15, 0x101, 0);
  ActorAnimate(20, 0x101, 0);
  ActorAnimate(21, 0x101, 0);
  ActorAnimate(22, 0x101, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  ActorAnimate(23, 0x100, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  ActorAnimate(21, 0x101, 30);
  Call2((void (*)())Func_02002254, 21, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 23, 21, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  ActorAnimate(20, 0x106, 30);
  Call2((void (*)())Func_02002254, 20, 0);
  ActorAnimate(23, 0x108, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  Call2((void (*)())Object_SetModeById, 23, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  ActorAnimate(15, 0x101, 30);
  Call2((void (*)())Func_02002254, 15, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 7, 23, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 7, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 23, 7, 0);
  Call2((void (*)())Object_SetModeById, 23, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  ActorAnimate(15, 0x101, 30);
  Call3((void (*)())ObjectMotion_SetAngleToward, 23, 15, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 7, 15, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 15, 0);
  ActorAnimate(6, 0x108, 30);
  Call2((void (*)())Func_02002254, 6, 0);
  ActorAnimate(21, 0x100, 30);
  Call2((void (*)())Func_02002254, 21, 0);
  Call2((void (*)())Object_SetModeById, 5, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 5, 0);
  Call2((void (*)())Object_SetModeById, 20, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 20, 0);
  Call2((void (*)())Object_SetModeById, *(s32 *)owner, 3);
  Call2((void (*)())Object_SetModeById, 5, 3);
  Call2((void (*)())Object_SetModeById, 6, 3);
  Call2((void (*)())Object_SetModeById, 7, 3);
  Call2((void (*)())Object_SetModeById, 23, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 90);
  Call2((void (*)())Object_SetModeById, 15, 3);
  Value2((s32 (*)())UiText_OpenMessageAtObject, 15, 0);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 0) {
    Call1((void (*)())Func_02002244, (s32)MsgRegardlessOfYour);
    ActorAnimate(15, 0x108, 30);
    Call2((void (*)())Func_02002254, 15, 0);
  } else {
    Call1((void (*)())Func_02002244, (s32)MsgYouTookPity);
    Call2((void (*)())Func_02002254, 15, 0);
  }
  message = (s32)MsgYouReNotImplying;
  Call1((void (*)())Func_02002244, message);
  Call2((void (*)())ObjectMotion_SetVariantCallback, 23, 1);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 15, 23, 0);
  ActorAnimate(15, 0x101, 30);
  Call2((void (*)())Object_SetModeById, 23, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 23, 21, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  Call3((void (*)())Object_LinkPair, 15, 20, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 21, 0);
  Call2((void (*)())Object_SetModeById, 7, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 7, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 15, 0x4000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 20, 0x4000, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Object_SetModeById, 6, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 6, 0);
  ActorAnimate(5, 0x108, 30);
  Call2((void (*)())Func_02002254, 5, 0);
  ActorAnimate(15, 0x100, 30);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  Call3((void (*)())ObjectMotion_SetAngleToward, 15, 21, 0);
  Call2((void (*)())Func_02002254, 15, 0);
  ActorAnimate(21, 0x108, 30);
  Call2((void (*)())Func_02002254, 21, 0);
  ActorAnimate(6, 0x106, 30);
  owner = (s32)&gAdventure;
  owner = owner + 532;
  Call3((void (*)())ObjectMotion_SetAngleToward, 6, *(s32 *)owner, 0);
  Value2((s32 (*)())UiText_OpenMessageAtObject, 6, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 5, *(s32 *)owner, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 6, *(s32 *)owner, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 7, *(s32 *)owner, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 23, *(s32 *)owner, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 15, *(s32 *)owner, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 20, *(s32 *)owner, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 21, *(s32 *)owner, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 0) {
    Call1((void (*)())Func_02002244, message + 10);
    Call2((void (*)())Object_SetModeById, 21, 4);
    Call1((void (*)())FieldEvent_WaitFrames, 30);
    Call2((void (*)())Func_02002254, 21, 0);
  } else {
    Call1((void (*)())Func_02002244, message + 11);
    Call2((void (*)())Object_SetModeById, 21, 3);
    Call1((void (*)())FieldEvent_WaitFrames, 30);
    Call2((void (*)())Func_02002254, 21, 0);
  }
  message = (s32)MsgIDisagree2;
  Call1((void (*)())Func_02002244, message);
  Call3((void (*)())ObjectMotion_SetAngleToward, 23, 21, 0);
  ActorAnimate(23, 0x101, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  Call2((void (*)())Object_SetModeById, 23, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  owner = (s32)&gAdventure;
  owner = owner + 532;
  Call2((void (*)())Object_SetModeById, *(s32 *)owner, 3);
  Call2((void (*)())Object_SetModeById, 5, 3);
  Call2((void (*)())Object_SetModeById, 6, 3);
  Call2((void (*)())Object_SetModeById, 7, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_SetAngleToward, 23, 15, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 15, *(s32 *)owner, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 23, *(s32 *)owner, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Value2((s32 (*)())UiText_OpenMessageAtObject, 23, 0);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 0) {
    Call1((void (*)())Func_02002244, message + 4);
    Call2((void (*)())Object_SetModeById, 5, 3);
    Call2((void (*)())Object_SetModeById, 6, 3);
    Call2((void (*)())Object_SetModeById, 7, 3);
    Call1((void (*)())FieldEvent_WaitFrames, 30);
    Call2((void (*)())Func_02002254, 5, 0);
    Call2((void (*)())Func_02002254, 6, 0);
    Call2((void (*)())Func_02002254, 7, 0);
  } else {
    Call1((void (*)())Func_02002244, message + 7);
    Call2((void (*)())Object_SetModeById, 5, 4);
    Call2((void (*)())Object_SetModeById, 6, 4);
    Call2((void (*)())Object_SetModeById, 7, 4);
    Call1((void (*)())FieldEvent_WaitFrames, 30);
    Call2((void (*)())Func_02002254, 5, 0);
    Call2((void (*)())Func_02002254, 6, 0);
    Call2((void (*)())Func_02002254, 7, 0);
  }
  Call1((void (*)())Func_02002244, (s32)MsgAsYouCanSee);
  Call3((void (*)())ObjectMotion_SetAngleToward, 23, 15, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  Call3((void (*)())Object_LinkPair, 15, 20, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Object_SetModeById, 15, 3);
  Call2((void (*)())Object_SetModeById, 20, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  party = (s32)&gAdventure;
  owner = party + 532;
  Call3((void (*)())ObjectMotion_SetAngleToward, 15, *(s32 *)owner, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 20, *(s32 *)owner, 0);
  Call2((void (*)())ObjectMotion_SetVariantCallback, 21, 1);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  ActorAnimate(21, 0x105, 30);
  Call3((void (*)())ObjectMotion_SetAngleToward, 23, *(s32 *)owner, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, *(s32 *)owner, 23, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  Call2((void (*)())Object_SetModeById, *(s32 *)owner, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Object_SetModeById, 23, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 5, 23, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 6, 23, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 7, 23, 0);
  Call2((void (*)())Object_SetModeById, 5, 3);
  Call2((void (*)())Object_SetModeById, 6, 3);
  Call2((void (*)())Object_SetModeById, 7, 3);
  Call2((void (*)())Object_SetModeById, *(s32 *)owner, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  ActorAnimate(15, 0x100, 0);
  ActorAnimate(20, 0x100, 0);
  ActorAnimate(21, 0x100, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 20, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, *(s32 *)owner, 15, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 5, 15, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 6, 15, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 7, 15, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 23, 15, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 23, 0);
  ActorAnimate(15, 0x109, 30);
  Call2((void (*)())Func_02002254, 15, 0);
  Call2((void (*)())Object_SetModeById, 5, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 5, 0);
  Call2((void (*)())Object_SetModeById, 20, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 20, 0);
  ActorAnimate(6, 0x105, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 6, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 6, 5, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 6, 0);
  Call2((void (*)())ObjectMotion_SetVariantCallback, 7, 1);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 7, 0);
  Call3((void (*)())Object_LinkPair, *(s32 *)owner, 7, 0);
  Call2((void (*)())Object_SetModeById, *(s32 *)owner, 3);
  Call2((void (*)())Object_SetModeById, 7, 3);
  Call3((void (*)())Object_LinkPair, 5, 6, 0);
  Call2((void (*)())Object_SetModeById, 5, 3);
  Call2((void (*)())Object_SetModeById, 6, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  Call3((void (*)())Object_LinkPair, *(s32 *)owner, 5, 0);
  Call3((void (*)())Object_LinkPair, 7, 6, 0);
  Call2((void (*)())Object_SetModeById, *(s32 *)owner, 3);
  Call2((void (*)())Object_SetModeById, 7, 3);
  Call2((void (*)())Object_SetModeById, 5, 3);
  Call2((void (*)())Object_SetModeById, 6, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())ObjectMotion_SetAngleToward, *(s32 *)owner, 15, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 5, 15, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 6, 15, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 7, 15, 0);
  Call3((void (*)())ObjectMotion_SetAngleToward, 23, 15, 0);
  Call2((void (*)())Object_SetModeById, 15, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02002254, 15, 0);
  Call2((void (*)())Object_SetModeById, *(s32 *)owner, 3);
  Call2((void (*)())Object_SetModeById, 7, 3);
  Call2((void (*)())Object_SetModeById, 5, 3);
  Call2((void (*)())Object_SetModeById, 6, 3);
  Call2((void (*)())Object_SetModeById, 23, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 15);
  Call2((void (*)())Object_SetModeById, 15, 3);
  Call2((void (*)())Object_SetModeById, 20, 3);
  Call2((void (*)())Object_SetModeById, 21, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call2((void (*)())Object_SetModeById, 5, 2);
  leader = Object_GetById(*(s32 *)owner);
  if (leader != 0)
    Call3((void (*)())ObjectMotion_ResetAndSetPosition, 5,
          ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 5);
  Call3((void (*)())Func_02002214, 5, 0, 0);
  Call2((void (*)())Object_SetModeById, 6, 2);
  leader = Object_GetById(*(s32 *)owner);
  if (leader != 0)
    Call3((void (*)())ObjectMotion_ResetAndSetPosition, 6,
          ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 6);
  Call3((void (*)())Func_02002214, 6, 0, 0);
  Call2((void (*)())Object_SetModeById, 7, 2);
  leader = Object_GetById(*(s32 *)owner);
  if (leader != 0)
    Call3((void (*)())ObjectMotion_ResetAndSetPosition, 7,
          ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 7);
  Call3((void (*)())Func_02002214, 7, 0, 0);
  Call2((void (*)())Object_SetModeById, 23, 2);
  leader = Object_GetById(*(s32 *)owner);
  if (leader != 0)
    Call3((void (*)())ObjectMotion_ResetAndSetPosition, 23,
          ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 23);
  Call3((void (*)())Func_02002214, 23, 0, 0);
  Call1((void (*)())GameFlag_SetBit, 0x928);
  Call1((void (*)())GameFlag_SetBit, 0x350);
  Func_020021cc();
}
