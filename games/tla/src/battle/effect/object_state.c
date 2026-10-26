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

  height = Map_GetTerrainHeightFar(0, state->x, state->z);
  state->y = height;
  state->terrainHeight = height;
}
