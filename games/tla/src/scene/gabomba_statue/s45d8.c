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
  s16 statuesReady;
  u8 unk172[2];
  s32 statueOffset;
};

extern struct SceneState gSceneState;

void Func_020056b4(void);
void Func_020056bc(void);
void Func_020044b4(s32 mode);
void Func_020044d0(s32 value);

void Func_020045d8(void) {
  if (gSceneState.redrawA != 0) {
    MapGrid_CopyCellsLayered(1, 12, 1, 1, gSceneState.originX + 3,
                             gSceneState.originZ - 2);
    gSceneState.redrawA = 0;
  }
}

/// Side B's twin of Func_02004530: the offset moves up instead.
void Func_02004620(void) {
  struct SceneState *state = &gSceneState;
  s32 i;

  if (state->redrawB == 0) {
    MapGrid_CopyCellsLayered(0, 13, 1, 1, state->originX + 7,
                             state->originZ - 2);
    if (state->statuesReady != 0) {
      Event_PlaySound(248);
      Func_020056b4();
      FieldEvent_ResolvePendingActions(0);
      Func_020044b4(2);
      i = 19;
      do {
        s32 *offset = &state->statueOffset;

        *offset += 0x333;
        System_Wait(1);
        i--;
      } while (i >= 0);
      Func_020044d0(1);
      Func_020056bc();
      state->statueOffset = 0;
    }
    state->redrawB = 1;
  }
}
