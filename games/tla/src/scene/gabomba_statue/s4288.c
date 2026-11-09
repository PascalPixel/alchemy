#include "types.h"
#include "actor.h"
#include "event_inline.h"

struct PuzzleCell {
  u8 kind;
  u8 blocked;
  u8 state;
  u8 occupied;
};

struct SceneState {
  struct PuzzleCell cells[88];
  u8 unk160[4];
  s16 firstId;
  s16 endId;
  u16 tileEntry;
  s8 redrawA;
  s8 redrawB;
  s16 originX;
  s16 originZ;
  s16 statuesReady;
  u8 unk172[2];
  s32 statueOffset;
};

extern struct SceneState gSceneState;


// Shows or hides the statues: gives each occupied cell's actor, numbered from
// id, its world position and draws its tile, or clears the actor and restores
// the cell's kind tile.
void Func_02004288(s32 id, s32 show) {
  struct SceneState *state = &gSceneState;
  s32 row;
  s32 column;

  state->statuesReady = show;
  state->firstId = id;
  row = 0;
nextRow:
  column = 0;
nextColumn: {
  struct PuzzleCell *cell = &state->cells[row * 11 + column];

  if (cell->occupied != 0) {
    struct Actor *object = Actor_GetById(id);
    s32 mapX;
    s32 mapZ;
    s32 kind;

    if (show != 0) {
      object->x = ((state->originX + column) << 20) + 0x80000;
      object->z = ((state->originZ + row) << 20) + 0x80000;
      {
        s32 x = state->originX;
        s32 z = state->originZ;

        /* FAKEMATCH(temporary): the origins read into x and z before the column
         * and row are added; as plain arguments, 12 of 119 instructions differ
         * in tla-en. */
        x += column;
        z += row;
        Map_CopyCellsRect(0, 11, 1, 1, x, z);
      }
      cell->occupied = 2;
    } else {
      object->x = 0;
      object->z = 0;
      // A byte store of the cell's last byte: sched2 orders it behind the next
      // load's constant only as a char access.
      ((u8 *)cell)[3] = 1;
      /* FAKEMATCH(temporary): kind and the origins read before the column and row
       * are added; as plain arguments, 37 of 119 instructions differ in tla-en. */
      kind = cell->kind;
      mapX = state->originX;
      mapZ = state->originZ;
      mapX += column;
      mapZ += row;
      Map_CopyCellsRect(1, kind, 1, 1, mapX, mapZ);
    }
    id++;
  }
}
  column++;
  if (column <= 10)
    goto nextColumn;
  row++;
  if (row <= 7)
    goto nextRow;
  state->endId = id;
}
