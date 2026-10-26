// Under -mthumb-slow-loads (2026-10-26): differ 2 of 82; left: the plane (r5,
// #76) loads after the leader's z, before it in the native; operand orders, a
// plane local with a barrier or a one-pass block were worse.
// Draft, differ 12 of 82: the leader's reflection in the floor, written from
// the listing (no twin). Above the mirror plane it copies the leader's x and y,
// mirrors z about the plane, flips the angle and copies three sprite fields;
// BG2 takes priority 1 below the plane and 2 above it. Left: three scheduling
// swaps. sched2 sees no dependence between the object stores and the BG2CNT
// accesses, and in each branch the native reads BG2CNT (ldrh r2, [r1]) before
// the zero stores or the mask, and loads the HImode priority constant before
// the second read; this draft does both later (priority ties broken by insn
// order). In the copy branch the native loads the plane (r5, #76) before the
// leader's z. Measured: the stores after the register writes (19, crossjumped
// with the flag branch), the read into a u32 local first (14: first block
// right, but the mask becomes a pool load), the masked read into a local (16),
// an inline priority setter (52), a mirror local (20), ~3 for the mask (12),
// alchemy permute 5 minutes (10, a block around the animation load).

#include "types.h"
#include "io_reg.h"
#include "object_runtime.h"
#include "party_state.h"

struct ReflectionSprite {
  u8 unk00[2];
  u16 unk02;
  u8 unk04[12];
  s32 unk10;
  u8 unk14;
};

struct ReflectionAnimation {
  u8 unk00[0x28];
  struct ReflectionSprite *sprite;
};

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);

void Func_020010d4(struct ObjectRuntime *object) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct ReflectionAnimation *leaderAnimation = leader->animation;
  struct ReflectionAnimation *animation = object->animation;

  if (leader->z <= object->unk4c) {
    object->z = 0;
    object->x = 0;
    REG_BG2CNT &= 0xfffc;
    REG_BG2CNT |= 1;
  } else {
    REG_BG2CNT &= 0xfffc;
    REG_BG2CNT |= 2;
    if (GameFlag_Test(0x212)) {
      object->z = 0;
      object->x = 0;
    } else {
      struct ReflectionSprite *sprite = animation->sprite;
      struct ReflectionSprite *source;

      object->x = leader->x;
      object->y = leader->y;
      object->z = object->unk4c - (leader->z - object->unk4c);
      object->angle = ~leader->angle;
      source = leaderAnimation->sprite;
      sprite->unk10 = source->unk10;
      sprite->unk02 = source->unk02;
      sprite->unk14 = source->unk14;
    }
  }
}
