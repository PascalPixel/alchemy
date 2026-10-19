#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"
#include "map.h"
#include "event_work.h"
#include "object_runtime.h"

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

/// The heading of each direction-pad combination, 0xffff for none.
extern const u16 gData_0802ec5c[16];

void Vector_AddPolarOffset(s32 radius, s32 angle,
                           struct FieldPosition *position);
s32 Func_080c8710(struct ObjectRuntime *object);
s32 Func_080c8700(struct ObjectRuntime *object);
s32 Func_0802da24(s32 *position);
s32 Func_0802d45c(s32 terrainId, s32 x, s32 z);
void ObjectDispatch_SelectAnimation(struct ObjectRuntime *object,
                                            s32 argument);
s32 Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);

/* FAKEMATCH: the held keys are read through a volatile pointer, again for each
 * test; as a plain field one read serves several (differ 43). */
#define INPUT_HELD (((volatile struct InputState *)&gInput)->held)

/// The script command 0x1b: moves an object 8 pixels at a time by the
/// direction pad, with up and down changing its height (y) rather than its
/// depth. Refuses steps into an occupied cell, from or onto a cell without
/// attribute bit 6, or onto ground too high (up) or too low (down), and
/// records the held keys in the event work. Returns 1.
// As ☀️'s Object_MoveByKeys.
s32 Object_MoveByKeys(struct ObjectRuntime *object) {
  struct FieldPosition position;
  struct MapCell *sourceCell;
  struct MapCell *destinationCell;
  struct EventWork *work;
  u16 angle;
  s32 animation;
  s32 collisionFlags;
  s32 deltaZ;
  s32 terrainHeight;
  struct MapWork *map;
  u32 keys;
  s32 half;

  object->acceleration = 0x4000;
  animation = 12;
  half = 0x8000;
  object->speedLimit = half;
  if (gDebugMode)
    keys = INPUT_HELD;
  keys = INPUT_HELD;
  {
    const u16 *table = gData_0802ec5c;

    /* FAKEMATCH: the native loads the table's address right after the keys;
     * without the barrier it loads just before the read (differ 2). */
    asm("" : "+l"(table) : "l"(keys));
    angle = *(const u16 *)((const u8 *)table + (((keys >> 4) & 15) << 1));
  }
  if (angle == 0xffff) {
    collisionFlags = 4;
  } else {
    animation = 14;
    if ((angle & 0xf000) != 0) {
      animation = 15;
      if ((angle & 0xf000) != half)
        animation = 10;
    }
    collisionFlags = 0;
    position.x = 0;
    position.y = 0;
    position.z = 0;
    Vector_AddPolarOffset(0x80000, angle, &position);
    position.x += object->x;
    deltaZ = position.z;
    if (deltaZ < 0)
      object->angle = 0xc000;
    if (deltaZ > 0)
      object->angle = 0x4000;
    position.y = object->y - position.z;
    position.z = object->z;
    map = Ram_HeapSlots->mapWork;
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
    // Spanish and Italian builds name this far call differently, as in
    // cell_collision.c.
    sourceCell = &map->layers[object->terrainId].cells[Func_080c8700(object)];
#else
    sourceCell = &map->layers[object->terrainId].cells[Func_080c8710(object)];
#endif
    destinationCell =
        &map->layers[object->terrainId]
             .cells[position.x / 0x100000 + (position.z / 0x100000) * 128];
    if (Func_0802da24((s32 *)&position) != 0 ||
        !(sourceCell->attributeB & 64) || !(destinationCell->attributeB & 64)) {
      collisionFlags = 4;
      animation = 12;
    } else if (INPUT_HELD & 64) {
      terrainHeight =
          Func_0802d45c(object->terrainId, position.x, position.z - 0x100000);
      if (terrainHeight - object->y < 0x100000)
        goto refused;
    } else if (INPUT_HELD & 128) {
      terrainHeight = Func_0802d45c(object->terrainId, position.x, position.z);
      if (terrainHeight - object->y > -0x80000) {
      refused:
        collisionFlags = 1;
        animation = 12;
      }
    }
  }
  work = Ram_HeapSlots->eventWork;
  if (work != NULL) {
    if (collisionFlags & 3)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = INPUT_HELD;
  }
  ObjectDispatch_SelectAnimation(object, animation);
  if (collisionFlags != 0) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
  } else {
    Object_SetMoveTarget(object, position.x, position.y, position.z);
  }
  object->step++;
  return 1;
}
