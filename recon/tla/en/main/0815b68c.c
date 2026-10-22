// Near miss of Func_0815b68c, decompiled from the listing (101 instructions,
// draft 101, differ 6; 12 without the tagged one-pass block): the native
// starts the 0x6e4 offset (movs r2, #192) before the zero for the position
// is set, and it stores x and y (str r3, [r5] and [r6]) one row after the
// next offset is started; here those rows come later. sched2 ranks each
// store (priority 10) below the next offset's chain (12). Tried: one-pass
// blocks or an empty asm around the x or y update (8 to 14), the updates
// through locals (6 or 14), the scale copied after the zeros (12). Matched
// on the way: the particle's fields read through work at each use (a
// pointer to the particle shares one base) and position[3] and position[1]
// cleared first.
#include "types.h"
#include "battle.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "ram_buffer.h"

struct PlacementScale {
  s32 x;
  s32 y;
};

// A scale of one in 16.16 on both axes.
extern const struct PlacementScale gData_08196e4c;

s32 Animation_ApplyChildArgumentFar(void *object, s32 argument);

// A frame callback of summon 17 (BattleEffect_RunUlysses adds and removes it): places
// effect object 7 at the last particle's point (x across, y in depth) at a
// scale of one, moves the particle by its velocity (its depth speed grows
// for 20 frames, then shrinks, and its sideways speed grows from frame 12),
// and steps the object's animation through cels 7 to 14, seven frames each.
void Func_0815b68c(void) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  struct PlacementScale scale = gData_08196e4c;
  s32 position[4];

  position[3] = 0;
  /* FAKEMATCH: the one-pass block puts the position's address (add r1, sp,
     #8) before the scale's stores and the second zero store with the first;
     plain, differ 12 (here 6). */
  do {
    position[1] = 0;
  } while (0);
  position[0] = work->particles[63].x;
  position[2] = work->particles[63].y;
  Render_ApplyProjectedPlacementFar(work->objects[7], position, (s32 *)&scale,
                                    0);
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
