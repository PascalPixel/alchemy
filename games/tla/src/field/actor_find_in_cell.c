/// @file
/// Finding the actor that stands in a cell.

#include "types.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "field/field_sequence_event.h"

/// Returns the first of objects 8 to 63 standing in the same 16-pixel cell
/// as position (x, y and z), or NULL. Callers pass the object asking; the
/// search does not use it.
struct Actor *Actor_FindInCell(s32 *position, struct Actor *self) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u32 i;
  void **objects;
  struct Actor *object;

  i = 8;
  objects = &work->actors[8];
  for (; i < 64; i++) {
    object = *objects++;
    if (FIELD_CELL(position[0]) == FIELD_CELL(object->x.fixed) &&
        FIELD_CELL(position[1]) == FIELD_CELL(object->y.fixed) &&
        FIELD_CELL(position[2]) == FIELD_CELL(object->z.fixed))
      return object;
  }
  return NULL;
}
