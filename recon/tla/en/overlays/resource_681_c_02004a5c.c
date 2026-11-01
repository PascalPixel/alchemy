// Matches: differ 0 (tla-en, 2026-11-01)
// Stands the object on a cell of map layer 2, marks that cell's collision code
// 255 and centres the object on it, counting y from z. Its callers pass a
// leader's position. The one device is the barrier below, the same form and
// site as games/tla/src/field/object/f0d3378.c: without it the copy of object
// for the priority flags' address goes ahead of the motion store. Tried first,
// all 2 or worse: pointer, char and s8 forms of both stores, a u8 or s32 flag
// local, zero locals, a volatile store, one-pass blocks, static inlines, a dead
// copy of object, and a bare asm("") (which moves lsls instead).
#include "../../../../games/tla/include/types.h"
#include "../../../../games/tla/include/system/ram_buffer.h"
#include "../../../../games/tla/include/map.h"
#include "../../../../games/tla/include/actor.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);

void Func_02004a5c(s32 id, s32 x, s32 y, s32 z) {
  struct Actor *object = Actor_GetById(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[2].cells;
  s32 column = x / 0x100000;
  s32 row = (z - y + 0x7fff) / 0x100000;

  cell += column + (row << 7);
  cell->collisionCode = 255;
  object->x = (x & 0xfff00000) + 0x80000;
  object->z = (z & 0xfff00000) + 0x80000;
  object->y = object->z - (row << 20);
  object->motionFlags = 0;
  /* FAKEMATCH(barrier): an empty asm that reads the cleared motion flags and
   * rewrites object, so the priority flags' address is copied after the store;
   * as plain statements, 2 of 66 instructions differ in tla-en. */
  asm("" : "+r"(object) : "X"(object->motionFlags));
  object->priorityFlags |= 2;
  Actor_SetSingleChildFlags(object, 0);
}
