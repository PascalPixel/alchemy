#include "types.h"
#include "field_event.h"
#include "adventure.h"
#include "text/msg_ids.h"

void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void ObjectMotion_SetVariantCallback(s32 actor, s32 variant);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void Motion_SetVarCbAndRefresh(s32 actor, s32 variant);
void Object_TurnToFaceEachOther(s32 a, s32 b, s32 c);
void Func_02000ca8(s32 actor, s32 x, s32 z);
void Func_02000d08(s32 actor, s32 value, s32 frames);
void Func_02000d48(s32 actor, s32 dx, s32 dz, s32 angle);

/// Brings the actor to the player character's position.
static inline void Actor_JoinLeader(s32 actor) {
  struct FieldActor *leader;

  Import_ActorSetAnimation(actor, 2);
  leader = Import_ActorGet(gAdventure.currentOwner);
  if (leader != 0)
    ObjectMotion_ResetAndSetPosition(actor, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  ObjectMotion_CommitCurrentPositionAndActivate(actor);
  Func_02000ca8(actor, 0, 0);
}

static __inline__ void ObjectMotion_ArmCallback_(s32 actor, s32 angle,
                                                 s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_ArmCallback(actor, angle, frames);
}

static __inline__ void Func_02000d48_(s32 actor, s32 dx, s32 dz, s32 angle) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02000d48(actor, dx, dz, angle);
}

static __inline__ void Func_02000d08_(s32 actor, s32 value, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02000d08(actor, value, frames);
}

static __inline__ void ObjectMotion_SetPositionAndCommit_(s32 actor, s32 x,
                                                          s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetPositionAndCommit(actor, x, z);
}

static __inline__ void ObjectMotion_SetPositionAndReset_(s32 actor, s32 x,
                                                         s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetPositionAndReset(actor, x, z);
}

static __inline__ void Func_02000ca8_(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02000ca8(actor, x, z);
}

void Func_020003a0(void) {
  s32 msg;

  if (Import_GameFlagIsSet(0x84b))
    return;
  if (Import_GameFlagIsSet(0x910))
    return;
  if (Import_GameFlagIsSet(7))
    return;
  Import_EventStart();
  Import_EventResolvePendingActions(0);
  msg = (s32)MsgDailaFelixCall;
  Import_EventSetMessageId(msg);
  Func_02000ca8_(27, 472 << 16, 264 << 16);
  Import_EventWaitFrames(30);
  ObjectMotion_SetPositionAndReset_(gAdventure.currentOwner, 504, 296);
  ObjectMotion_ArmCallback_(gAdventure.currentOwner, 0xa000, 0);
  ObjectMotion_SetPositionAndCommit_(27, 472, 280);
  ObjectMotion_ArmCallback_(27, 0x2000, 0);
  Import_EventWaitFrames(30);
  Func_02000d08_(27, 256, 0);
  Import_EventWaitFrames(60);
  Import_EventShowMessageAndWait(27, 0);
  Func_02000d48_(5, 8, 16, 0xa000);
  Func_02000d48_(6, 16, 0, 0xa000);
  Func_02000d48_(28, 32, -8, 0x8000);
  Import_EventWaitFrames(60);
  Import_EventShowMessageAndWait(27, 0);
  Import_EventWaitFrames(30);
  Func_02000d08_(5, 259, 0);
  Import_EventWaitFrames(60);
  Import_EventShowMessageAndWait(5, 0);
  Import_EventWaitFrames(30);
  ObjectMotion_ArmCallback_(27, 0, 0);
  Import_EventWaitFrames(30);
  ObjectMotion_ArmCallback_(27, 0x2000, 0);
  Import_EventWaitFrames(30);
  ObjectMotion_ArmCallback_(27, 0x4000, 0);
  Import_EventWaitFrames(30);
  ObjectMotion_ArmCallback_(27, 0x2000, 0);
  Import_EventWaitFrames(30);
  Import_EventShowMessageAndWait(27, 0);
  Import_EventWaitFrames(30);
  ObjectMotion_SetAngleToward(6, 28, 0);
  Import_EventWaitFrames(30);
  Import_ActorSetAnimation(6, 3);
  Import_EventWaitFrames(30);
  Import_EventShowMessageAndWait(6, 0);
  Import_EventWaitFrames(30);
  Object_TurnToFaceEachOther(gAdventure.currentOwner, 28, 0);
  Import_EventWaitFrames(30);
  ObjectMotion_SetAngleToward(28, 27, 0);
  ObjectMotion_SetAngleToward(gAdventure.currentOwner, 27, 0);
  Import_EventWaitFrames(60);
  Import_EventShowMessageAndWait(28, 0);
  Import_EventWaitFrames(30);
  ObjectMotion_SetAngleToward(6, 27, 0);
  Import_ActorSetAnimation(27, 4);
  Import_EventWaitFrames(60);
  Import_EventShowMessageAndWait(27, 0);
  Import_EventWaitFrames(30);
  ObjectMotion_ArmCallback_(27, 0xc000, 0);
  Import_EventWaitFrames(60);
  Import_EventShowMessageAndWait(27, 0);
  ObjectMotion_SetAngleToward(27, gAdventure.currentOwner, 0);
  Import_EventWaitFrames(60);
  ObjectMotion_SetVariantCallback(28, 1);
  Import_EventWaitFrames(60);
  Import_EventShowMessageAndWait(28, 0);
  Import_EventWaitFrames(30);
  Import_ActorSetAnimation(27, 3);
  Import_EventWaitFrames(30);
  Import_EventShowMessageAndWait(27, 0);
  Import_EventWaitFrames(30);
  ObjectMotion_SetAngleToward(5, gAdventure.currentOwner, 0);
  Import_FieldMessageShowAtActor(5, 0);
  if (Import_EventAskYesNoWithActorAnimation(Import_PartyGetLeaderActor(), 0) == 0) {
    Import_EventWaitFrames(30);
    Import_EventSetMessageId(msg + 11);
    Import_EventShowMessageAndWait(27, 0);
    Import_EventWaitFrames(60);
    Func_02000d08_(5, 259, 0);
    Import_EventWaitFrames(60);
    ObjectMotion_SetAngleToward(5, 27, 0);
    Import_EventShowMessageAndWait(5, 0);
    Import_EventSetMessageId(msg + 15);
  } else {
    Import_EventWaitFrames(30);
    Import_EventSetMessageId(msg + 13);
    Import_EventShowMessageAndWait(27, 0);
    Import_EventWaitFrames(60);
    Motion_SetVarCbAndRefresh(5, 1);
    ObjectMotion_SetAngleToward(5, 27, 0);
    Import_EventShowMessageAndWait(5, 0);
  }
  Import_ActorSetAnimation(27, 4);
  Import_EventWaitFrames(60);
  Import_EventShowMessageAndWait(27, 0);
  Motion_SetVarCbAndRefresh(6, 1);
  Import_EventWaitFrames(60);
  Import_EventShowMessageAndWait(6, 0);
  Import_EventWaitFrames(60);
  ObjectMotion_SetAngleToward(27, 6, 0);
  Import_EventWaitFrames(60);
  Import_EventShowMessageAndWait(27, 0);
  Import_EventWaitFrames(60);
  Func_02000d08_(28, 258, 0);
  Import_EventWaitFrames(60);
  Import_EventShowMessageAndWait(28, 0);
  Import_EventWaitFrames(60);
  Import_ActorSetAnimation(27, 3);
  Import_EventWaitFrames(60);
  Import_EventShowMessageAndWait(27, 0);
  ObjectMotion_SetPositionAndReset_(27, 504, 328);
  ObjectMotion_SetAngleToward(gAdventure.currentOwner, 27, 0);
  ObjectMotion_SetAngleToward(5, 27, 0);
  ObjectMotion_SetAngleToward(6, 27, 0);
  ObjectMotion_SetAngleToward(28, 27, 0);
  ObjectMotion_SetPositionAndReset_(27, 456, 328);
  ObjectMotion_SetAngleToward(gAdventure.currentOwner, 27, 0);
  ObjectMotion_SetAngleToward(5, 27, 0);
  ObjectMotion_SetAngleToward(6, 27, 0);
  ObjectMotion_SetAngleToward(28, 27, 0);
  ObjectMotion_SetPositionAndReset_(27, 424, 472);
  ObjectMotion_SetAngleToward(gAdventure.currentOwner, 27, 0);
  ObjectMotion_SetAngleToward(5, 27, 0);
  ObjectMotion_SetAngleToward(6, 27, 0);
  ObjectMotion_SetAngleToward(28, 27, 0);
  Import_EventWaitFrames(60);
  Func_02000ca8_(27, 0, 0);
  Import_EventWaitFrames(120);
  ObjectMotion_Launch(5, 4, 20);
  ObjectMotion_Launch(5, 4, 20);
  Import_EventWaitFrames(30);
  Func_02000d08_(5, 259, 0);
  Import_EventWaitFrames(60);
  Import_EventShowMessageAndWait(5, 0);
  Import_EventWaitFrames(60);
  Import_ActorSetAnimation(6, 3);
  Import_EventWaitFrames(60);
  Import_EventShowMessageAndWait(6, 0);
  Import_EventWaitFrames(60);
  ObjectMotion_SetAngleToward(28, gAdventure.currentOwner, 0);
  Import_EventWaitFrames(60);
  ObjectMotion_ArmCallback_(28, 0x4000, 0);
  Import_EventWaitFrames(30);
  Import_EventShowMessageAndWait(28, 0);
  Import_EventWaitFrames(60);
  Import_ActorSetAnimation(gAdventure.currentOwner, 3);
  Import_ActorSetAnimation(5, 3);
  Import_ActorSetAnimation(6, 3);
  Import_EventWaitFrames(120);
  Actor_JoinLeader(5);
  Actor_JoinLeader(6);
  Actor_JoinLeader(28);
  Import_EventEnd();
  Import_GameFlagSet(0x84b);
}
