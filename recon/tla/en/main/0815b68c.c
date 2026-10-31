// Near miss, differ 6 of 101 (tla-en, 2026-10-31): scheduler order. The
// native starts the particle's offset (movs r2, #192) before the position's
// zero stores, and stores the new x and y (str r3, [r5] and [r6]) right
// after starting the next offset; here each store comes after that offset is
// built. In sched2 each such store ranks two below the offset chain that
// starts beside it (the x store 13 against 15, its link to the next load
// costing 2 against the chain's 4); the native's order at both places needs
// a gap of one (as at the zero stores without the one-pass block: 34 against
// the chain's 36), which points to the cost of a load after a store rather
// than the C.

// Under -mthumb-slow-loads (2026-10-26): differ 6 of 101 with the one-pass
// block, 12 without it (10 with the position and scale read through
// struct SpritePlacement and struct SpriteScale, 2026-10-31).

// Near miss of Func_0815b68c, decompiled from the listing (101 instructions,
// draft 101, differ 6; 10 without the tagged one-pass block). Tried: one-pass
// blocks or an empty asm around the x or y update (8 to 14), the updates
// through locals (6 or 14), the scale copied after the zeros (12), the
// permuter for 4 minutes (only blocks, 8). Matched on the way: the particle's
// fields read through work at each use (a pointer to the particle shares one
// base) and the position's groundHeight and height cleared first.
#include "types.h"
#include "battle.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "ram_buffer.h"
#include "render_placement.h"

// A scale of one in 16.16 on both axes.
extern const struct SpriteScale gData_08196e4c;

s32 Animation_ApplyChildArgumentFar(void *object, s32 argument);

// A frame callback of summon 17 (BattleEffect_RunUlysses adds and removes it):
// places effect object 7 at the last particle's point (x across, y in depth)
// at a scale of one, moves the particle by its velocity (its depth speed
// grows for 20 frames, then shrinks, and its sideways speed grows from frame
// 12), and steps the object's animation through cels 7 to 14, seven frames
// each.
void Func_0815b68c(void) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  struct SpriteScale scale = gData_08196e4c;
  struct SpritePlacement place;

  place.groundHeight = 0;
  /* FAKEMATCH(one-pass): the one-pass block puts the placement's address
     (add r1, sp, #8) before the scale's stores and the second zero store
     with the first; as a plain statement, 10 of 101 instructions differ in
     tla-en. */
  do {
    place.height = 0;
  } while (0);
  place.x = work->particles[63].x;
  place.y = work->particles[63].y;
  Render_ApplyProjectedPlacementFar(work->objects[7], &place.x, &scale.x, 0);
  work->particles[63].x += work->particles[63].velocityX;
  work->particles[63].y += work->particles[63].velocityY;
  if (work->frame <= 19)
    work->particles[63].velocityY += 0x1000;
  else
    work->particles[63].velocityY -= 0x1000;
  if (work->frame > 11)
    work->particles[63].velocityX += 0x1000;
  Animation_ApplyChildArgumentFar(work->objects[7], work->frame / 7 % 8 + 7);
  work->frame++;
}
