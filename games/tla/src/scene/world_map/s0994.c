#include "types.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "flag_call.h"
#include "text/msg_ids.h"

struct EventDestination {
  u8 unk000[0x154];
  s16 scene;
};

void WaitFrames(s32 frames);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004c88(s32 actor, s32 x, s32 z);
void Func_02004c98(s32 actor, s32 target);
void Func_02004cd8(s32 message);
void Func_02004cf0(s32 actor, s32 flags);
void Func_02004d00(s32 actor, s32 angle);
s32 Func_02004d08(s32 actor, s32 value, s32 frames);
void Func_02004d10(s32 actor, s32 value);
void Func_02004d40(s32 value);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004db8(s32 value);
void Func_02004df8(s32 value, s32 mode);
void Func_02004e30(s32 cue);
struct ObjectRuntime *Object_GetById(s32 id);

/// Brings the actor to the player character's position.
static __inline__ void Actor_JoinLeader(s32 actor) {
  struct ObjectRuntime *leader;

  Object_SetModeById(actor, 2);
  leader = Object_GetById(gAdventure.currentOwner);
  if (leader != 0)
    ObjectMotion_ResetAndSetPosition(actor, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  ObjectMotion_CommitCurrentPositionAndActivate(actor);
  Func_02004c88(actor, 0, 0);
}

static __inline__ void ObjectMotion_ArmCallback_(s32 actor, s32 angle,
                                                 s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_ArmCallback(actor, angle, frames);
}

static __inline__ void Func_02004df8_(s32 value, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004df8(value, mode);
}

static __inline__ void ObjectMotion_SetPositionAndReset_(s32 actor, s32 x,
                                                         s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetPositionAndReset(actor, x, z);
}

static __inline__ void Func_02004d00_(s32 actor, s32 angle) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004d00(actor, angle);
}

static __inline__ void Func_02004d10_(s32 actor, s32 value) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004d10(actor, value);
}

static __inline__ void Func_02004d08_(s32 actor, s32 value, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004d08(actor, value, frames);
}

void Func_02000994(void) {
  struct ObjectRuntime *object;
  s32 x;
  s32 z;
  s32 seen;

  if (Flag_IsSet(0xa21) && !Flag_IsSet(0x9d0)) {
    object = Object_GetById(gAdventure.currentOwner);
    x = object->x;
    z = object->z;
    Func_02004c20();
    Func_02004db8(0);
    seen = Flag_IsSet(0x9b3);
    x >>= 20;
    z >>= 20;
    x <<= 4;
    z <<= 4;
    if (seen) {
      Func_02004df8_(0x16666, 6);
      Func_02004c98(5, gAdventure.currentOwner);
      WaitFrames(1);
      ObjectMotion_SetPositionAndReset_(5, x + 24, z - 12);
      ObjectMotion_ArmCallback_(5, 0x6000, 0);
      Func_02004d00_(gAdventure.currentOwner, 0xe000);
      Func_02004da8();
      Func_02004cd8((s32)MsgWeShouldnT);
      Func_02004cf0(5, 0);
      Func_02004db0();
      Actor_JoinLeader(5);
      WaitFrames(1);
      ObjectMotion_SetPositionAndReset_(gAdventure.currentOwner, 7312, 0x2000);
      Func_02004df8_(0x10000, 6);
    } else {
      Func_02004df8_(0x16666, 6);
      Func_02004c98(5, gAdventure.currentOwner);
      WaitFrames(1);
      z -= 12;
      ObjectMotion_SetPositionAndReset_(5, x + 24, z);
      ObjectMotion_ArmCallback_(5, 0x6000, 0);
      Func_02004d00_(gAdventure.currentOwner, 0xe000);
      Func_02004da8();
      Func_02004cd8((s32)MsgYouReNot);
      Func_02004cf0(5, 0);
      Func_02004db0();
      ObjectMotion_ArmCallback_(5, 0x8000, 0);
      Func_02004d10_(gAdventure.currentOwner, 258);
      ObjectMotion_SetPositionAndReset_(gAdventure.currentOwner, x + 12, z);
      Func_02004d00_(gAdventure.currentOwner, 0);
      Object_SetModeById(5, 4);
      Func_02004da8();
      Func_02004cf0(5, 0);
      Func_02004d08_(5, 265, 0);
      Func_02004cf0(5, 0);
      Object_SetModeById(5, 3);
      Func_02004cf0(5, 0);
      Func_02004db0();
      Actor_JoinLeader(5);
      WaitFrames(1);
      ObjectMotion_SetPositionAndReset_(gAdventure.currentOwner, 7312, 0x2000);
      Flag_Set(0x9b3);
      Func_02004df8_(0x10000, 6);
    }
    WaitFrames(8);
    Func_02004c28();
  } else {
    struct EventDestination *work = Ram_HeapSlots->eventWork;

    Func_02004c20();
    Func_02004db8(0);
    Func_02004e30(123);
    Func_02004d40(work->scene);
  }
}
