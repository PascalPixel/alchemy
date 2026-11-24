/// @file
/// Finding the actor that stands in a cell.

#include "types.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "field/field_sequence_event.h"
#include "field/field_state.h"
#include "field/scene_tables.h"

/// Returns the first placed actor (ACTOR_FIRST_PLACED up to the treasure spots,
/// FIELD_TREASURE_ACTOR) standing in the same 16-pixel cell as position (x, y
/// and z), or NULL. Callers pass the actor asking; the search does not use it.
struct Actor *Actor_FindInCell(s32 *position, struct Actor *self) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  u32 i;
  struct Actor **objects;
  struct Actor *actor;

  i = ACTOR_FIRST_PLACED;
  objects = &state->actors[ACTOR_FIRST_PLACED];
  for (; i < FIELD_TREASURE_ACTOR; i++) {
    actor = *objects++;
    // UB: ObjectTable_Clear leaves unused slots null.
#ifdef UBFIX
    if (actor == NULL)
      continue;
#endif
    if (FIELD_CELL(position[0]) == FIELD_CELL(actor->x.fixed) &&
        FIELD_CELL(position[1]) == FIELD_CELL(actor->y.fixed) &&
        FIELD_CELL(position[2]) == FIELD_CELL(actor->z.fixed))
      return actor;
  }
  return NULL;
}
