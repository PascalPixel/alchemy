#include "../../../../games/tla/src/scene/champa_cliff_caves/scene.h"
#include "scene_ids.h"
#include "version.h"

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
