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
#include "adventure.h"
#include "scene_tables.h"

extern u8 gOverlayArea[];

void BattleEffect_LoadResourceGroup(void) {
  const struct Scene *table = gScenes;
  s16 scene = gAdventure.scene;

  Resource_LoadCode(table[scene].resourceId, gOverlayArea);
}
