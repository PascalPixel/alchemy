// Draft, differ 62 of 95: in mode 1, marks the 11-by-8 grid cell under the object the effect
// work names (gSceneState's cells, four bytes each, from the grid's origin)
// with the mode, redraws it (Func_0200566c), parks the object, and when no
// cell is left marked 2 clears the grid's halfword after the origin. Written
// through the scene state's struct with the cell's kind read before the mark
// and the column search entered at its test (from 102). Left: allocation of x,
// z and the origin's address (the native juggles the origin through lr), the
// zero kept in r8 that also starts the row, and the first column's load, which
// jump2 merges into the search's here.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"

struct PuzzleCell {
  u8 kind;
  u8 blocked;
  u8 state;
  u8 occupied;
};

struct SceneState {
  struct PuzzleCell cells[88];
  s32 count;
  s16 firstId;
  s16 endId;
  u16 tileEntry;
  s8 redrawA;
  s8 redrawB;
  s16 originX;
  s16 originZ;
  s16 marked;
};

extern struct SceneState gSceneState;
struct ObjectRuntime *Object_GetById(u32 id);
void Func_0200566c(s32 a, s32 kind, s32 b, s32 c, s32 x, s32 z);

void Func_020043ec(s32 mode) {
  struct SceneState *state;
  struct ObjectRuntime *object;
  struct PuzzleCell *cell;
  s32 x, z;
  u32 row, col;
  s32 found;
  s32 occupied;

  if (mode != 1)
    return;
  state = &gSceneState;
  object = Object_GetById(*(s16 *)((u8 *)Ram_HeapSlots->effectWork + 26));
  x = (object->x >> 20) - state->originX;
  z = (object->z >> 20) - state->originZ;
  {
    s32 kind = state->cells[z * 11 + x].kind;

    cell = &state->cells[z * 11 + x];
    cell->occupied = mode;
    Func_0200566c(1, kind, 1, 1, state->originX + x, state->originZ + z);
  }
  object->x = 0;
  object->z = 0;
  found = 0;
  row = 0;
next:
  col = 0;
  occupied = state->cells[row * 11 + col].occupied;
  goto test;
step:
  col++;
  if (col > 10)
    goto done;
  occupied = state->cells[row * 11 + col].occupied;
test:
  if (occupied != 2)
    goto step;
  found = 1;
done:
  if (found)
    return;
  row++;
  if (row <= 7)
    goto next;
  state->marked = found;
}
