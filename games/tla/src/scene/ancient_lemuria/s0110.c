#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "flag_call.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 a, s32 b);
void Actor_CommitPositionAndActivate(s32 id, s32 dx, s32 dz);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
void Func_02000048(void);
void Func_0200195c(s32 id, s32 x, s32 z);
void Func_02001a24(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY, s32 layer);

static inline void MapCopy(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                           s32 destY, s32 layer) {
  /* FAKEMATCH(wrapper): as in scene.h, the inline parameters keep each stack
   * argument in its own register; as a direct call, 8 of 273 instructions
   * differ in tla-en. */
  Func_02001a24(srcX, srcY, width, height, destX, destY, layer);
}

static inline void PlaceActor(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): the inline orders the placement's arguments; as a
   * direct call, 2 of 273 instructions differ in tla-en. */
  Func_0200195c(id, x, z);
}

/// One hop of object 12 by (dx, dz).
static inline void Hop(s32 dx, s32 dz) {
  Actor_Jump(12, 5, 0);
  Actor_CommitPositionAndActivate(12, dx, dz);
  FieldEvent_Wait(3);
}

/// Once (flag 0x96c): object 12 spins for 30 frames while it grows from half
/// scale, then hops thirteen times while the leader follows, and object 13
/// takes Func_02000048 as its update.
void Func_02000110(void) {
  struct Actor *object;
  s32 i;

  MapCopy(7, 13, 1, 1, 5, 13, 0);
  if (Flag_IsSet(0x96c))
    return;
  object = Actor_GetById(12);
  Flag_Set(0x96c);
  Flag_Set(0x96e);
  Actor_SetSingleChildFlags(Actor_GetById(12), 0);
  PlaceActor(12, 176 << 15, 216 << 16);
  Actor_Jump(12, 9, 0);
  object->scaleY = 0x8000;
  object->scaleX = 0x8000;
  for (i = 29; i >= 0; i--) {
    object->angle += 0x1000;
    {
      /* FAKEMATCH(pin): the limit's first half stays in r2; as a plain
       * local, 30 of 273 instructions differ in tla-en. */
      register s32 limit asm("r2") = 255;

      /* FAKEMATCH(barrier): the barrier starts the 0xffff limit before the
       * scale's load and finishes it after; without it, 13 of 273
       * instructions differ in tla-en. */
      asm("" : "+r"(limit) : "r"(object->scaleX));
      if (object->scaleX <= (limit << 8) + 255)
        object->scaleX += 0xa00;
    }
    /* FAKEMATCH(barrier): the memory barrier keeps the native's reload of the
     * scale at the join; without it, 9 of 273 instructions differ in tla-en. */
    asm("" ::: "memory");
    object->scaleY = object->scaleX;
    wait(1);
  }
  object->angle = 0x3000;
  object->scaleY = 0x10000;
  object->scaleX = 0x10000;
  Actor_SetSingleChildFlags(Actor_GetById(12), 1);
  Actor_SetSpritePriority(12, 1);
  FieldEvent_Wait(8);
  Actor_Jump(12, 9, 0);
  Actor_SetPositionAndReset(12, 120, 200);
  Actor_GetById(12)->priorityFlags |= 1;
  Actor_SetPositionAndReset(12, 136, 200);
  FieldEvent_Wait(3);
  Actor_SetModeById(gAdventure.currentOwner, 1);
  Actor_ArmCallback(gAdventure.currentOwner, 0xe000, 0);
  Hop(0, -16);
  Hop(16, 0);
  Hop(16, 0);
  Hop(0, -16);
  Hop(16, 0);
  Hop(0, -16);
  Hop(16, 0);
  Hop(16, 0);
  Actor_ArmCallback(gAdventure.currentOwner, 0, 0);
  Hop(0, 16);
  Hop(16, 0);
  Hop(16, 0);
  Hop(16, 0);
  Hop(16, 0);
  Func_0200195c(12, 0, 0);
  Actor_GetById(13)->update = Func_02000048;
  Actor_GetById(13)->action = 0;
}
