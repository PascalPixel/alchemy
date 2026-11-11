#include "scene_tables.h"
#include "scene.h"

/// The byte at 0x5a of an actor, as the field its bit 0 is set through.
struct ActorSolid {
  u8 unk00[0x5a];
  u8 solid : 8;
};

void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Actor_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);
void Func_080d3b28(s32 a, s32 b);
void Actor_SetHorizontalPositionWithTerrain(s32 a, s32 b, s32 c);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_CommitPositionAndActivate_(s32 actor,
                                                               s32 dx, s32 dz) {
  Actor_CommitPositionAndActivate(actor, dx, dz);
}

void Func_020005e8(s32 actor) {
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 648, 200);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_EAST);
  Event_Wait(5);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 6);
  Event_Wait(7);
  Func_080d3b28(10, actor);
  Actor_SetHorizontalPositionWithTerrain(10, 648 << 16, 196 << 16);
  Actor_GetById(gMainRoutineWork.currentOwner)->unk5a[0] &= ~1;
  Actor_CommitPositionAndActivate(gMainRoutineWork.currentOwner, -16, -8);
  Event_Wait(1);
  /* FAKEMATCH(signature): the byte declared as an 8-bit bitfield and the bit
   * set through it; as a plain byte the store aliases the owner's load that
   * follows, which the scheduler then puts before the zero, differ 2. */
  ((struct ActorSolid *)Actor_GetById(gMainRoutineWork.currentOwner))->solid |= 1;
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_EAST);
  Event_Wait(10);
}

void Func_0200067c(s32 value) {
  /* FAKEMATCH(temporary): the bit and the mask in locals, declared in this
   * order so that the bit takes r6 and the mask r7; as literals they take the
   * other's register, differ 12. */
  s32 bit = 1;
  s32 mask = 0xfe;

  Actor_SetSpeedParameters(8, 0xcccc, 0x6666);
  Actor_GetById(8)->unk5a[0] &= mask;
  Actor_CommitPositionAndActivate(8, 0, 6);
  Event_Wait(1);
  ((struct ActorSolid *)Actor_GetById(8))->solid |= bit;
  Event_Wait(10);
  Actor_SetModeById(8, 8);
  Event_Wait(15);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  Event_Wait(15);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  if (value == 0) {
    Actor_GetById(8)->unk5a[0] &= mask;
    Actor_CommitPositionAndActivate_(8, 0, -6);
    Event_Wait(1);
    ((struct ActorSolid *)Actor_GetById(8))->solid |= bit;
    Event_Wait(10);
  }
}
