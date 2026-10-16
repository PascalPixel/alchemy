// Draft, differ 171 of 119, a first attempt: the shape is the native's (two
// loops over the 8 by 11 cells, a statue placed or hidden for each occupied
// one), but the native keeps only the scene state's base in r5 and rebuilds
// every field offset where it is used, inside the loops, while this form loads
// the origin's addresses from the pool above them (r9, r11). The state pointer
// that keeps Func_020044d0's offsets inside its loop does not hold them here.
#include "types.h"
#include "object_runtime.h"

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

struct ObjectRuntime *Object_GetById(s32 id);
void Func_0200566c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

static __inline__ void Map_Put(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_0200566c(a, b, c, d, e, f);
}

void Func_02004288(s32 id, s32 show) {
  struct SceneState *state = &gSceneState;
  s32 row;
  s32 column;

  state->statuesReady = show;
  state->firstId = id;
  for (row = 0; row <= 7; row++) {
    for (column = 0; column <= 10; column++) {
      struct PuzzleCell *cell = &state->cells[row * 11 + column];

      if (cell->occupied != 0) {
        struct ObjectRuntime *object = Object_GetById(id);

        if (show != 0) {
          object->x = ((state->originX + column) << 20) + 0x80000;
          object->z = ((state->originZ + row) << 20) + 0x80000;
          Map_Put(0, 11, 1, 1, state->originX + column, state->originZ + row);
          cell->occupied = 2;
        } else {
          object->x = 0;
          object->z = 0;
          cell->occupied = 1;
          Map_Put(1, cell->kind, 1, 1, state->originX + column,
                  state->originZ + row);
        }
        id++;
      }
    }
  }
  state->endId = id;
}
