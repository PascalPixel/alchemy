#include "scene_tables.h"
#include "scene.h"

/// The byte at 0x5a of an actor, as the field its bit 0 is set through.
struct ActorSolid {
  u8 unk00[0x5a];
  u8 solid : 8;
};

void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Actor_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);
void Func_0200361c(s32 a, s32 b);
void Func_020034d4(s32 a, s32 b, s32 c);

static __inline__ void Actor_CommitPositionAndActivate_(s32 actor,
                                                               s32 dx, s32 dz) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_CommitPositionAndActivate(actor, dx, dz);
}

void Func_020005e8(s32 actor) {
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 648, 200);
  Func_02003534(gMainRoutineWork.currentOwner, FACING_EAST);
  FieldEvent_Wait(5);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 6);
  FieldEvent_Wait(7);
  Func_0200361c(10, actor);
  Func_020034d4(10, 648 << 16, 196 << 16);
  Actor_GetById(gMainRoutineWork.currentOwner)->unk5a[0] &= ~1;
  Actor_CommitPositionAndActivate(gMainRoutineWork.currentOwner, -16, -8);
  FieldEvent_Wait(1);
  /* FAKEMATCH: the bit is set through the 8-bit field: read as a plain byte the
   * store aliases the owner's load that follows, which the scheduler then puts
   * before the zero (differ 2). */
  ((struct ActorSolid *)Actor_GetById(gMainRoutineWork.currentOwner))->solid |= 1;
  Func_02003534(gMainRoutineWork.currentOwner, FACING_EAST);
  FieldEvent_Wait(10);
}

void Func_0200067c(s32 value) {
  /* FAKEMATCH: declared in this order the bit takes r6 and the mask r7; as
   * literals they take the other's register (differ 12). */
  s32 bit = 1;
  s32 mask = 0xfe;

  Actor_SetSpeedParameters(8, 0xcccc, 0x6666);
  Actor_GetById(8)->unk5a[0] &= mask;
  Actor_CommitPositionAndActivate(8, 0, 6);
  FieldEvent_Wait(1);
  ((struct ActorSolid *)Actor_GetById(8))->solid |= bit;
  FieldEvent_Wait(10);
  Actor_SetModeById(8, 8);
  FieldEvent_Wait(15);
  Func_020034d4(10, 0, 0);
  FieldEvent_Wait(15);
  Actor_SetModeById(8, 1);
  FieldEvent_Wait(15);
  if (value == 0) {
    Actor_GetById(8)->unk5a[0] &= mask;
    Actor_CommitPositionAndActivate_(8, 0, -6);
    FieldEvent_Wait(1);
    ((struct ActorSolid *)Actor_GetById(8))->solid |= bit;
    FieldEvent_Wait(10);
  }
}
