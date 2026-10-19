#include "types.h"
#include "field_obj.h"
#include "game_state.h"

// The Colosso finals' three courses (Colosso Log, River and Wall).

// Return the placed actor standing one cell, or else two cells, ahead of the
// actor the player moves (straight ahead in the nearest of the four
// directions), or 0 when there is none.
void *Colosso_FindActorAhead(void) {
  struct FieldActor *record;
  s32 facing;
  s32 position[3];
  s32 *occupant;

  record = Object_GetById(gGameState.selectedActor);

  facing = (record->facing + 0x2000) & 0xc000;

  position[0] = (record->x.fixed & 0xfff00000) + 0x80000;
  position[1] = record->y.fixed;
  position[2] = (record->z.fixed & 0xfff00000) + 0x80000;
  Vector_AddPolarOffset(0x100000, facing, position);

  occupant = SceneData_FindSlotAtPosition(position, record);
  if (occupant == 0) {
    position[0] = (record->x.fixed & 0xfff00000) + 0x80000;
    position[1] = record->y.fixed;
    position[2] = (record->z.fixed & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x200000, facing, position);

    occupant = SceneData_FindSlotAtPosition(position, record);
  }

  return occupant;
}
