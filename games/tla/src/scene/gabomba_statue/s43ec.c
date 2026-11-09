#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "event_inline.h"

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

void Func_020043ec(s32 mode) {
  struct SceneState *state;
  struct Actor *object;
  struct PuzzleCell *cell;
  s32 x, z;
  u32 row, col;
  s32 found;

  if (mode != 1)
    return;
  state = &gSceneState;
  object = Actor_GetById(*(s16 *)((u8 *)Ram_HeapSlots->effectWork + 26));
  x = (object->x >> 20) - state->originX;
  z = (object->z >> 20) - state->originZ;
  {
    s32 kind = state->cells[z * 11 + x].kind;

    cell = &state->cells[z * 11 + x];
    cell->occupied = mode;
    MapGrid_CopyCells(1, kind, 1, 1, state->originX + x, state->originZ + z);
  }
  object->x = 0;
  object->z = 0;
  found = 0;
  row = 0;
  for (row = 0; row <= 7; row++) {
    for (col = 0; col <= 10; col++) {
      if (state->cells[row * 11 + col].occupied == 2) {
        found = 1;
        break;
      }
    }
    if (found)
      return;
  }
  state->marked = found;
}
