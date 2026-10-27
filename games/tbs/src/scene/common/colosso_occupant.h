#include "types.h"
#include "actor.h"
#include "scene.h"
#include "persistent_state.h"

// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3).

/// Returns the placed actor standing one cell, or else two cells, ahead
/// of the actor the player moves (straight ahead in the nearest of the
/// four directions), or 0 when there is none.
void *Colosso_FindActorAhead(void) {
  struct Actor *record;
  s32 facing;
  struct FieldPosition position;
  struct Actor *occupant;

  record = Actor_GetById(gPersistentState.leaderActorId);

  facing = (record->facing + 0x2000) & 0xc000;

  position.x = (record->x.fixed & 0xfff00000) + 0x80000;
  position.y = record->y.fixed;
  position.z = (record->z.fixed & 0xfff00000) + 0x80000;
  Vector_AddPolarOffset(0x100000, facing, &position);

  occupant = Actor_FindAtPosition(&position, record);
  if (occupant == 0) {
    position.x = (record->x.fixed & 0xfff00000) + 0x80000;
    position.y = record->y.fixed;
    position.z = (record->z.fixed & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x200000, facing, &position);

    occupant = Actor_FindAtPosition(&position, record);
  }

  return occupant;
}
