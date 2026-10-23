#include "types.h"
#include "object_event.h"
#include "field_runtime.h"
#include "game_state.h"

// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3).

/// Returns the placed object event standing one cell, or else two cells, ahead
/// of the object event the player moves (straight ahead in the nearest of the
/// four directions), or 0 when there is none.
void *Colosso_FindObjectEventAhead(void) {
  struct ObjectEvent *record;
  s32 facing;
  struct FieldPosition position;
  struct ObjectEvent *occupant;

  record = ObjectEvent_GetById(gGameState.selectedObjectEvent);

  facing = (record->facing + 0x2000) & 0xc000;

  position.x = (record->x.fixed & 0xfff00000) + 0x80000;
  position.y = record->y.fixed;
  position.z = (record->z.fixed & 0xfff00000) + 0x80000;
  Vector_AddPolarOffset(0x100000, facing, &position);

  occupant = MapData_FindSlotAtPosition(&position, record);
  if (occupant == 0) {
    position.x = (record->x.fixed & 0xfff00000) + 0x80000;
    position.y = record->y.fixed;
    position.z = (record->z.fixed & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x200000, facing, &position);

    occupant = MapData_FindSlotAtPosition(&position, record);
  }

  return occupant;
}
