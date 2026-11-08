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
  // Once-only block: keeps the original's order of the code around it.
  do {
  } while (0);

  place.height = 0;

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
