// Under -Os, 5 November: differ 51 of 119 (tla-en); older figures below predate it.
// Draft, differ 126 of 119, skeleton gap 21 (tla-en, 2026-11-01)
// Two label-and-goto loops over the 8 by 11 cells keep every field offset in
// the body, as the native does; the old inline wrapper and real for loops are
// gone (171 to 157 for the loops and wrapper, 157 to 126 for mapX and mapZ).
// What is left is allocation. The native has state r5, cell r6, column r7,
// row r8, id r10, show r9. Here row takes r4 with caller-saves around both
// calls, because it has 19 weighted refs over 3 calls and 4 * 3 < 19, and that
// rotates every reload after it. Pinning row to r8 in a scratch copy, as a
// diagnostic only, gives differ 51 with this source. The native order is what
// flat refs would give: state, cell, column, row, id, show by priority, with
// row under the caller-save limit; loop-weighted refs give cell before state.
// No source form tried changes row's refs: reversed arms, declaration orders,
// cell and object at the top, a real outer for, `11 * row`, pointer arithmetic,
// s16 id, u16 show and an s32 return for Func_0200566c all tie or lose.
// Reading originX and originZ into their own locals before adding gives 118
// (47 pinned), matching the native's read, read, add, add. It only moves
// scheduling, so it is left out as a forced temporary.
#include "types.h"
#include "actor.h"

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

struct Actor *Actor_GetById(s32 id);
void Func_0200566c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

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

    if (show != 0) {
      object->x = ((state->originX + column) << 20) + 0x80000;
      object->z = ((state->originZ + row) << 20) + 0x80000;
      /* FAKEMATCH(temporary): mapX and mapZ kept; as plain arguments, differ 157
       * of 119 in tla-en against 126. */
      mapX = state->originX + column;
      mapZ = state->originZ + row;
      Func_0200566c(0, 11, 1, 1, mapX, mapZ);
      cell->occupied = 2;
    } else {
      object->x = 0;
      object->z = 0;
      cell->occupied = 1;
      mapX = state->originX + column;
      mapZ = state->originZ + row;
      Func_0200566c(1, cell->kind, 1, 1, mapX, mapZ);
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
