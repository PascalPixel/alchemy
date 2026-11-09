#include "types.h"
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
};

extern struct SceneState gSceneState;

void Func_020046c8(void) {
  if (gSceneState.redrawB != 0) {
    MapGrid_CopyCellsLayered(0, 12, 1, 1, gSceneState.originX + 7,
                             gSceneState.originZ - 2);
    gSceneState.redrawB = 0;
  }
}
