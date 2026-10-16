// Near miss, differ 2 of 59 (was 6): Effect_AdvanceMotion with the party
// leader's sprite priority copied at the end; overlay 697's Func_02001464 and
// Func_02004a14 are the same function. One row is left: the native reads both
// priority bytes before it masks (ldrb r3; ldrb r1; movs r2, #12; ands r2, r3);
// this draft reads the destination byte after the first mask.
// Tried 2026-10-12 for that row: both sprites in pointer temporaries behind a
// barrier (the destination takes r0, not r4; with the destination loaded
// before the owner's sprite it keeps r4 but the two pointer loads swap,
// differ 4); the two bytes read through u8 casts before a barrier and merged
// by hand (other masks, differ 17); five permuter minutes.
#include "../../../../../games/tla/src/scene/yampi_desert/scene.h"

void Func_02000b04(struct ObjectRuntime *object) {
  s32 vx = object->bounce;
  s32 vz = object->unk4c;
  struct FieldSprite *sprite;
  s32 step;
  struct PartyState *party;

  object->x += vx;
  object->y += object->gravity;
  object->z += vz;
  object->bounce = vx - vx / 18;
  object->unk4c = vz - vz / 16;
  /* FAKEMATCH: the ARM9 scheduling would load the scale step before storing the
   * Z velocity, as in Effect_AdvanceMotion. */
  __asm__("");
  sprite = object->animation;
  step = object->speedLimit;
  object->scaleX += step;
  object->scaleY += object->acceleration;
  /* FAKEMATCH: and would address the spin before storing the Y scale. */
  __asm__("");
  sprite->rotation += object->action;
  party = &gPartyState;
  /* FAKEMATCH: the native loads the party state address before it starts the
   * owner offset (differ 4 without the temporary and barrier). */
  __asm__("");
  ((struct FieldSprite *)object->animation)->priority =
      Object_GetById(party->currentOwner)->sprite->priority;
}
