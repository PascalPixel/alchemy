#include "types.h"
#include "actor.h"
#include "scene.h"

// Colosseum Final 1, 2 and 3, and Venus Lighthouse.

/// Returns the placed actor (8 to 65) whose cell, and 16-pixel step of
/// height, is that of the 16.16 position, or 0. self is not read.
static void *Actor_FindAtPosition(const struct FieldPosition *position,
                                  void *self) {
  struct FieldWork *work = gFieldWork;
  struct Actor **slots = work->actors;
  u32 i;

  for (i = ACTOR_FIRST_PLACED;
       i < sizeof(work->actors) / sizeof(work->actors[0]); i++) {
    struct Actor *p = slots[i];

    // UB: Actor_ClearAll leaves unused slots null.
#ifdef UBFIX
    if (p == NULL)
      continue;
#endif
    if ((position->x >> 20) == FIELD_CELL(p->x.fixed) &&
        (position->y >> 20) == FIELD_CELL(p->y.fixed) &&
        (position->z >> 20) == FIELD_CELL(p->z.fixed)) {
      return p;
    }
  }
  return 0;
}
