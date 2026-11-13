#ifndef TLA_DAILA_INTERIORS_DAILA_H
#define TLA_DAILA_INTERIORS_DAILA_H

#include "types.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_Wait(s32 frames);
void Import_EventSetMessageId(s32 message);
void Import_EventShowMessageAndWait(s32 actor, s32 flags);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Actor_SetHorizontalPositionWithTerrain(s32 actor, s32 x, s32 z);
void Func_02000d94(s32 actor, s32 jump, s32 frames);
void Map_CopyCellsRect(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline s32 Daila_FlagCheck(s32 flag) { return GameFlag_Test(flag); }

static inline void Daila_ObjWalk(s32 actor, s32 x, s32 z) {
  Actor_SetPositionAndReset(actor, x, z);
}

static inline void Daila_ObjPlace(s32 actor, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(actor, x, z);
}

static inline void Daila_Jump(s32 actor, s32 height, s32 frames) {
  s32 h = height;
  s32 f = frames;
  s32 a = actor;

  /* FAKEMATCH(barrier): an empty asm over the three copies finishes height and
   * frames before the actor; as Func_02000d94(actor, height, frames), the actor
   * is copied two instructions early and, in Func_020000ac, 2 of 62
   * instructions differ in tla-en. */
  __asm__("" : "+l"(h), "+l"(f), "+l"(a));
  Func_02000d94(a, h, f);
}

static inline void Daila_CopyCells(s32 srcX, s32 srcY, s32 destX, s32 destY,
                                   s32 width, s32 height) {
  Map_CopyCellsRect(srcX, srcY, destX, destY, width, height);
}

#endif
