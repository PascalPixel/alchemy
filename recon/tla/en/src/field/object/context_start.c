#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"

struct ContextAnimation {
  u8 unk00[0x1a];
  u8 flags;
};

struct ContextEffect {
  u8 unk00[5];
  u8 param;
};

struct ContextEffect *ResourceMetadata_RegisterFar(void *context, s32 id);
void GameFlag_SetBit(s32 flag);

/// Starts the leader's animation for leaving through an exit: centres it on
/// its tile, at the tile's south edge, stops it and gives it the animation's
/// mode, then waits 18 frames. The leader's sprite gains a second part
/// (metadata 267) for it.
// As ☀️'s function of the same name.
void ObjectEffect_PrepareContextEffect(s32 animation) {
  u32 zero;
  u8 param;
  struct Actor *object;
  struct ContextAnimation *context;
  struct ContextEffect *effect;
  struct MainRoutineWork *party;
  s32 mode;

  party = &gMainRoutineWork;
  /* FAKEMATCH: the empty asm takes gMainRoutineWork's address before the
   * argument is copied to r8 (differ 2 without it: the native loads the
   * address first). */
  asm("" : "=r"(mode) : "0"(animation), "r"(party));
  object = ObjectTable_Get(party->currentOwner);
  context = object->animation;
  effect = ResourceMetadata_RegisterFar(context, 267);
  zero = 0;
  context->flags = zero;
  /* FAKEMATCH: the one-pass block stores the cleared flags before the kind is
   * loaded (differ 6 without it). */
  do {
  } while (0);
  param = 15;
  effect->param = param;
  object->x = (object->x & 0xFFF00000) + 0x80000;
  object->z = (object->z & 0xFFF00000) + 0x100000;
  object->velocityX = zero;
  object->velocityZ = zero;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  Actor_SetMode(object, mode);
  wait(18);
}

void ObjectEffect_StartContextEffect26(void) {
  ObjectEffect_PrepareContextEffect(0x1A);
  GameFlag_SetBit(0x120);
}

void ObjectEffect_StartContextEffect25(void) {
  ObjectEffect_PrepareContextEffect(0x19);
  GameFlag_SetBit(0x121);
}
