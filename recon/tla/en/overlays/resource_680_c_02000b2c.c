// Draft of Func_02000b2c (overlay 680, 66 instructions), differ 28: places
// actor id on the cell under (x, z), lifted by y, and marks that map cell (byte
// 2 = 255). Written from the listing (the twin map's ☀️ SceneActor_PlaceAtTile
// is a false match). Register allocation differs: the native keeps y in r8 and
// the cell x in r7 and loads the cell table into r0; here y takes r7, the cell
// x r0 and the table ip, so the native's r8 save is missing. Measured: the cell
// store in a one-pass block (differ 30). alchemy permute, 5 minutes: differ 16
// at best, only with id as a short (sign-extended), which the callers do not
// suggest.
// Measured against its copies: overlay 681's Func_02004a5c (differ 28 of 66).
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);

struct MapCell {
  u8 unk0[2];
  u8 mark;
  u8 unk3;
};

void Func_02000b2c(s32 id, s32 x, s32 y, s32 z) {
  struct ObjectRuntime *actor = Object_GetById(id);
  u8 *cells;
  s32 cellX;
  s32 cellZ;

  cells = *(u8 **)((u8 *)Ram_HeapSlots->mapWork + 424);
  cellX = x / 0x100000;
  cellZ = (z - y + 0x7fff) / 0x100000;

  ((struct MapCell *)cells)[cellZ * 128 + cellX].mark = 255;
  actor->x = (x & 0xfff00000) + 0x80000;
  actor->z = (z & 0xfff00000) + 0x80000;
  actor->y = actor->z - (cellZ << 20);
  actor->motionFlags = 0;
  actor->priorityFlags |= 2;
  ObjectDispatch_SetSingleChildFlags(actor, 0);
}
