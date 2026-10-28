#include "types.h"
#include "field_event.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"

void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void Actor_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void Actor_WaitForMoveById(s32 actor);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Actor_SetAngleToward(s32 actor, s32 target, s32 frames);
void Actor_StartRepeatedMotion(s32 actor, s32 variant);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Motion_SetVarCbAndRefresh(s32 actor, s32 variant);
void Actor_TurnToFaceEachOther(s32 a, s32 b, s32 c);
void Func_02000ca8(s32 actor, s32 x, s32 z);
void Func_02000d08(s32 actor, s32 value, s32 frames);
void Func_02000d48(s32 actor, s32 dx, s32 dz, s32 angle);

/// Brings the actor to the player character's position.
static inline void Actor_JoinLeader(s32 actor) {
  struct FieldActor *leader;

  Import_ActorSetAnimation(actor, 2);
  leader = Import_ActorGet(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(actor, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(actor);
  Func_02000ca8(actor, 0, 0);
}

static __inline__ void Actor_ArmCallback_(s32 actor, s32 angle,
                                                 s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(actor, angle, frames);
}

static __inline__ void Func_02000d48_(s32 actor, s32 dx, s32 dz, s32 angle) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02000d48(actor, dx, dz, angle);
}

static __inline__ void Func_02000d08_(s32 actor, s32 value, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02000d08(actor, value, frames);
}

static __inline__ void Actor_SetPositionAndCommit_(s32 actor, s32 x,
                                                          s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPositionAndCommit(actor, x, z);
}

static __inline__ void Actor_SetPositionAndReset_(s32 actor, s32 x,
                                                         s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPositionAndReset(actor, x, z);
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
  Import_EventWait(30);
  Actor_SetPositionAndReset_(gMainRoutineWork.currentOwner, 504, 296);
  Actor_ArmCallback_(gMainRoutineWork.currentOwner, 0xa000, 0);
  Actor_SetPositionAndCommit_(27, 472, 280);
  Actor_ArmCallback_(27, 0x2000, 0);
  Import_EventWait(30);
  Func_02000d08_(27, 256, 0);
  Import_EventWait(60);
  Import_EventShowMessageAndWait(27, 0);
  Func_02000d48_(5, 8, 16, 0xa000);
  Func_02000d48_(6, 16, 0, 0xa000);
  Func_02000d48_(28, 32, -8, 0x8000);
  Import_EventWait(60);
  Import_EventShowMessageAndWait(27, 0);
  Import_EventWait(30);
  Func_02000d08_(5, 259, 0);
  Import_EventWait(60);
  Import_EventShowMessageAndWait(5, 0);
  Import_EventWait(30);
  Actor_ArmCallback_(27, 0, 0);
  Import_EventWait(30);
  Actor_ArmCallback_(27, 0x2000, 0);
  Import_EventWait(30);
  Actor_ArmCallback_(27, 0x4000, 0);
  Import_EventWait(30);
  Actor_ArmCallback_(27, 0x2000, 0);
  Import_EventWait(30);
  Import_EventShowMessageAndWait(27, 0);
  Import_EventWait(30);
  Actor_SetAngleToward(6, 28, 0);
  Import_EventWait(30);
  Import_ActorSetAnimation(6, 3);
  Import_EventWait(30);
  Import_EventShowMessageAndWait(6, 0);
  Import_EventWait(30);
  Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, 28, 0);
  Import_EventWait(30);
  Actor_SetAngleToward(28, 27, 0);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 27, 0);
  Import_EventWait(60);
  Import_EventShowMessageAndWait(28, 0);
  Import_EventWait(30);
  Actor_SetAngleToward(6, 27, 0);
  Import_ActorSetAnimation(27, 4);
  Import_EventWait(60);
  Import_EventShowMessageAndWait(27, 0);
  Import_EventWait(30);
  Actor_ArmCallback_(27, 0xc000, 0);
  Import_EventWait(60);
  Import_EventShowMessageAndWait(27, 0);
  Actor_SetAngleToward(27, gMainRoutineWork.currentOwner, 0);
  Import_EventWait(60);
  Actor_StartRepeatedMotion(28, 1);
  Import_EventWait(60);
  Import_EventShowMessageAndWait(28, 0);
  Import_EventWait(30);
  Import_ActorSetAnimation(27, 3);
  Import_EventWait(30);
  Import_EventShowMessageAndWait(27, 0);
  Import_EventWait(30);
  Actor_SetAngleToward(5, gMainRoutineWork.currentOwner, 0);
  Import_FieldMessageShowAtActor(5, 0);
  if (Import_EventAskYesNoWithActorAnimation(Import_PartyGetLeaderActor(), 0) == 0) {
    Import_EventWait(30);
    Import_EventSetMessageId(msg + 11);
    Import_EventShowMessageAndWait(27, 0);
    Import_EventWait(60);
    Func_02000d08_(5, 259, 0);
    Import_EventWait(60);
    Actor_SetAngleToward(5, 27, 0);
    Import_EventShowMessageAndWait(5, 0);
    Import_EventSetMessageId(msg + 15);
  } else {
    Import_EventWait(30);
    Import_EventSetMessageId(msg + 13);
    Import_EventShowMessageAndWait(27, 0);
    Import_EventWait(60);
    Motion_SetVarCbAndRefresh(5, 1);
    Actor_SetAngleToward(5, 27, 0);
    Import_EventShowMessageAndWait(5, 0);
  }
  Import_ActorSetAnimation(27, 4);
  Import_EventWait(60);
  Import_EventShowMessageAndWait(27, 0);
  Motion_SetVarCbAndRefresh(6, 1);
  Import_EventWait(60);
  Import_EventShowMessageAndWait(6, 0);
  Import_EventWait(60);
  Actor_SetAngleToward(27, 6, 0);
  Import_EventWait(60);
  Import_EventShowMessageAndWait(27, 0);
  Import_EventWait(60);
  Func_02000d08_(28, 258, 0);
  Import_EventWait(60);
  Import_EventShowMessageAndWait(28, 0);
  Import_EventWait(60);
  Import_ActorSetAnimation(27, 3);
  Import_EventWait(60);
  Import_EventShowMessageAndWait(27, 0);
  Actor_SetPositionAndReset_(27, 504, 328);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 27, 0);
  Actor_SetAngleToward(5, 27, 0);
  Actor_SetAngleToward(6, 27, 0);
  Actor_SetAngleToward(28, 27, 0);
  Actor_SetPositionAndReset_(27, 456, 328);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 27, 0);
  Actor_SetAngleToward(5, 27, 0);
  Actor_SetAngleToward(6, 27, 0);
  Actor_SetAngleToward(28, 27, 0);
  Actor_SetPositionAndReset_(27, 424, 472);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 27, 0);
  Actor_SetAngleToward(5, 27, 0);
  Actor_SetAngleToward(6, 27, 0);
  Actor_SetAngleToward(28, 27, 0);
  Import_EventWait(60);
  Func_02000ca8_(27, 0, 0);
  Import_EventWait(120);
  Actor_Jump(5, 4, 20);
  Actor_Jump(5, 4, 20);
  Import_EventWait(30);
  Func_02000d08_(5, 259, 0);
  Import_EventWait(60);
  Import_EventShowMessageAndWait(5, 0);
  Import_EventWait(60);
  Import_ActorSetAnimation(6, 3);
  Import_EventWait(60);
  Import_EventShowMessageAndWait(6, 0);
  Import_EventWait(60);
  Actor_SetAngleToward(28, gMainRoutineWork.currentOwner, 0);
  Import_EventWait(60);
  Actor_ArmCallback_(28, 0x4000, 0);
  Import_EventWait(30);
  Import_EventShowMessageAndWait(28, 0);
  Import_EventWait(60);
  Import_ActorSetAnimation(gMainRoutineWork.currentOwner, 3);
  Import_ActorSetAnimation(5, 3);
  Import_ActorSetAnimation(6, 3);
  Import_EventWait(120);
  Actor_JoinLeader(5);
  Actor_JoinLeader(6);
  Actor_JoinLeader(28);
  Import_EventEnd();
  Import_GameFlagSet(0x84b);
}
