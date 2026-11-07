#include "../../../../../../games/tla/src/scene/taopo_swamp/local.h"

void Func_02002114(void) { Func_02001ff0(100, 100); }

void Func_02002120(void) {
  struct SceneState *state = &gSceneState;

  if (GameFlag_Test(0x109) == 0)
    Iwram_ClearWords(state, 12);
}
