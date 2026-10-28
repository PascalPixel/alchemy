// Under -mthumb-slow-loads (2026-10-26): differ 2 of 50 in tla-en, instruction
// order only. No empty asm before any one statement lowers it.
// Near miss, differ 4 of 50 (two reordered pairs at the start):
// ObjectEffect_PrepareContextEffect's sibling that takes the object's id and a
// side (mode 26 for side 1, else 25) and registers effect 267, in the twin's
// shape (a shared zero and kind) with the side's test kept in mode before it is
// subtracted from 26, which gives the native registers. The native copies the
// object (adds r5, r0) before the test's eor and loads the animation before its
// neg; this has each one row later. A barrier or one-pass block around the
// animation's load measured 6, one around the mode's two statements 16 and 6.
#include "types.h"
#include "actor.h"

struct ContextAnimation {
  u8 unk00[0x1a];
  u8 flags;
};

struct ContextEffect {
  u8 unk00[5];
  u8 param;
};

void *ResourceMetadata_RegisterFar(void *, s32);
void wait(s32 frames);

void Func_080d5d70(s32 id, s32 side) {
  u32 zero;
  u8 kind;
  struct Actor *object;
  struct ContextAnimation *context;
  struct ContextEffect *effect;
  s32 mode;

  object = ObjectTable_Get(id);
  context = object->animation;
  mode = (side != 1);
  mode = 26 - mode;
  effect = ResourceMetadata_RegisterFar(context, 267);
  zero = 0;
  context->flags = zero;
  /* FAKEMATCH: the barrier keeps the flag's store ahead of the effect's kind
   * (differ 2 without it). */
  __asm__("");
  kind = 15;
  effect->param = kind;
  object->x = (object->x & 0xFFF00000) + 0x80000;
  object->z = (object->z & 0xFFF00000) + 0x100000;
  object->velocityX = zero;
  object->velocityZ = zero;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  Actor_SetMode(object, mode);
  wait(18);
}
