// Draft, differ 102 of 95: in mode 1, marks the 11-by-8 grid cell under the
// object the effect work names (gSceneState's cells, four bytes each, from
// the grid's origin at 0x16c) with the mode, redraws it (Func_0200566c),
// parks the object, and when no cell is left marked 2 clears the grid's
// halfword at 0x170. Left: the native keeps the grid row offsets' fields in
// r12 and lr and copies the cell address through lr; the search's layout
// and registers differ.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"

extern u8 gSceneState[];
struct ObjectRuntime *Object_GetById(u32 id);
void Func_0200566c(s32 a, s32 cell, s32 b, s32 c, s32 x, s32 z);

void Func_020043ec(s32 mode) {
  struct ObjectRuntime *object;
  s32 x, z;
  u8 *cell;
  u32 row, col;
  s32 found;

  if (mode != 1)
    return;
  object = Object_GetById(*(s16 *)((u8 *)Ram_HeapSlots->effectWork + 26));
  x = (object->x >> 20) - *(s16 *)(gSceneState + 0x16c);
  z = (object->z >> 20) - *(s16 *)(gSceneState + 0x16e);
  cell = gSceneState + (z * 11 + x) * 4;
  cell[3] = mode;
  Func_0200566c(1, cell[0], 1, 1, *(s16 *)(gSceneState + 0x16c) + x,
                *(s16 *)(gSceneState + 0x16e) + z);
  object->x = 0;
  object->z = 0;
  found = 0;
  row = 0;
next:
  col = 0;
  if (gSceneState[row * 11 * 4 + 3] == 2)
    goto hit;
step:
  col++;
  if (col > 10)
    goto done;
  if (gSceneState[(row * 11 + col) * 4 + 3] != 2)
    goto step;
hit:
  found = 1;
done:
  if (found)
    return;
  row++;
  if (row <= 7)
    goto next;
  *(s16 *)(gSceneState + 0x170) = found;
}
