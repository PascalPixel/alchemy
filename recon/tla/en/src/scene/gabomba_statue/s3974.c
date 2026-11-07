#include "types.h"
#include "call.h"

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
};

extern struct SceneState gSceneState;
s32 GameFlag_Test(s32 flag);
void Func_0200566c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02003974(void) {
  struct SceneState *state = &gSceneState;
  u32 row;
  u32 col;

  for (row = 0; row <= 7; row++)
    for (col = 0; col <= 10; col++) {
      struct PuzzleCell *cell = &state->cells[row * 11 + col];
      s32 occupied = cell->occupied;

      if (GameFlag_Test(0x8fb) && cell->occupied == 2) {
        cell->occupied = 1;
        occupied = 1;
      }
      if (occupied == 2)
        Call6((void (*)())Func_0200566c, 0, 11, 1, 1, state->originX + col,
              state->originZ + row);
      else if (occupied == 1)
        Call6((void (*)())Func_0200566c, 1, state->cells[row * 11 + col].kind,
              1, 1, state->originX + col, state->originZ + row);
      else
        Call6((void (*)())Func_0200566c, occupied,
              state->cells[row * 11 + col].kind, 1, 1, state->originX + col,
              state->originZ + row);
    }
}
