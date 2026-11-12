/// @file
/// Scene code of overlay 0x685, in ROM order, from s04d4's offset up to the next object the overlay links.

#include "scene.h"
#include "scene_ids.h"
#include "version.h"
#include "types.h"
#include "scene_tables.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

struct EventTransition {
  u8 unk000[0x1ac];
  s32 type;
};

void Func_02001774(void);
void Func_02001868(void);
void Actor_SetHorizontalPositionWithTerrain(s32 actor, s32 x, s32 z);
void Func_020035ac(s32 a);
void Func_080d3b28(s32 a, s32 b);

static __inline__ void Func_020034d4_(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as a direct call, 4 of 112
   * instructions differ in tla-en (Func_020004d4). */
  Actor_SetHorizontalPositionWithTerrain(actor, x, z);
}

static __inline__ void Func_0200361c_(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as a direct call, 2 of 112
   * instructions differ in tla-en (Func_020004d4). */
  Func_080d3b28(a, b);
}

s32 Func_020004d4(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

#if !TLA_PATCH_1
  ((struct EventTransition *)Ram_HeapSlots->eventWork)->type = 516;
#else
  ((struct EventTransition *)Ram_HeapSlots->eventWork)->type = 513;
#endif
  if (party->scene != (s32)&SceneId_Ov685Room1)
    Actor_GetById(party->currentOwner)->priorityFlags |= 32;
  Func_020035ac(0);
  if (party->scene == (s32)&SceneId_Ov685Room4) {
    if (party->entrance == 98)
      Func_02001774();
    if (party->entrance == 99) {
      party->secondary = 23;
      party->entrance = 23;
      Func_02001868();
    }
    if (GameFlag_Test(0x977) && !GameFlag_Test(0x978)) {
      Func_0200361c_(10, 326);
      Func_020034d4_(10, 0x2980000, 0xe80000);
      Actor_GetById(10)->motionFlags = 4;
    }
    if (GameFlag_Test(0x977))
      Func_020034d4_(8, 0x2980000, 0xc80000);
  } else if (party->scene == (s32)&SceneId_Ov685Room3) {
    if (GameFlag_Test(0x97e)) {
      Actor_SetModeById(8, 11);
      Actor_SetSingleChildFlags(Actor_GetById(8), 0);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The byte at 0x5a of an actor, as the field its bit 0 is set through.
struct ActorSolid {
  u8 unk00[0x5a];
  u8 solid : 8;
};

void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Actor_CommitPositionAndActivate(u32 objectId, s32 x_offset, s32 z_offset);

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
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags &= ~1;
  Actor_CommitPositionAndActivate(gMainRoutineWork.currentOwner, -16, -8);
  Event_Wait(1);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_EAST);
  Event_Wait(10);
}

void Func_0200067c(s32 value) {
  Actor_SetSpeedParameters(8, 0xcccc, 0x6666);
  Actor_GetById(8)->actionFlags &= 0xfe;
  Actor_CommitPositionAndActivate(8, 0, 6);
  Event_Wait(1);
  ((struct ActorSolid *)Actor_GetById(8))->solid |= 1;
  Event_Wait(10);
  Actor_SetModeById(8, 8);
  Event_Wait(15);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  Event_Wait(15);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  if (value == 0) {
    Actor_GetById(8)->actionFlags &= 0xfe;
    Actor_CommitPositionAndActivate_(8, 0, -6);
    Event_Wait(1);
    ((struct ActorSolid *)Actor_GetById(8))->solid |= 1;
    Event_Wait(10);
  }
}
