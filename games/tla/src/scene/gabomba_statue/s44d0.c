#include "types.h"
#include "actor.h"
#include "event_inline.h"

struct SceneState {
  u8 unk000[0x164];
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

void Func_02004380(s32 x, s32 z, s32 a);
void Func_020056b4(void);
void Func_020056bc(void);
void Func_020044b4(s32 mode);

void Func_020044d0(s32 value) {
  struct SceneState *state = &gSceneState;
  s32 id;

  for (id = state->firstId; id < state->endId; id++) {
    struct Actor *object = Actor_Get(id);

    Func_02004380((object->x >> 20) - state->originX,
                  (object->z >> 20) - state->originZ, value);
  }
}

/// Redraws side A once; with the statues ready, moves their offset down by
/// 0x333 a frame for twenty frames, then clears it.
void Func_02004530(void) {
  struct SceneState *state = &gSceneState;
  s32 i;

  if (state->redrawA == 0) {
    MapGrid_CopyCellsLayered(1, 13, 1, 1, state->originX + 3,
                             state->originZ - 2);
    if (state->statuesReady != 0) {
      Event_PlaySound(248);
      Func_020056b4();
      FieldEvent_ResolvePendingActions(0);
      Func_020044b4(3);
      i = 19;
      do {
        s32 *offset = &state->statueOffset;

        *offset -= 0x333;
        System_Wait(1);
        i--;
      } while (i >= 0);
      Func_020044d0(0);
      Func_020056bc();
      state->statueOffset = 0;
    }
    state->redrawA = 1;
  }
}
