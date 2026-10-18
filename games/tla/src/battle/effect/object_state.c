#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "object_runtime.h"
#include "edition.h"

void ObjectMotion_SnapToTerrain(void *object) {
  struct ObjectRuntime *state = object;
  s32 height;

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||                      \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
  height = Func_08020190(0, state->x, state->z);
#else
  height = Map_GetTerrainHeightFar(0, state->x, state->z);
#endif
  state->y = height;
  state->terrainHeight = height;
}
