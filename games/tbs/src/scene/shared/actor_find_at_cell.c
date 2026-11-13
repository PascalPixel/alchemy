#include "actor_step.h"

// Bilibin, its cave, Kalay and Sol Sanctum's entrance.

/// Returns the placed actor (8 to 65) standing on cell (x, z), or 0.
static struct Actor *Actor_FindAtCell(s32 x, s32 z) {
  struct Actor **actors = gFieldWork->actors;
  u32 i;

  for (i = 8; i <= 65; i++) {
    struct Actor *actor = actors[i];

    if (x == FIELD_CELL(actor->x.fixed) && z == FIELD_CELL(actor->z.fixed)) {
      return actor;
    }
  }
  return 0;
}
