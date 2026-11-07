#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "resource.h"
#include "field/world_map.h"

extern void Window_CloseFar(u32 arg0, u32 arg1);

void BattleEffect_CleanupResourcesAndWindow(void) {
  Resource_ResetEntry(gWorldMapWork.vramBlock);
  Resource_ResetEntry(gWorldMapWork.windowSlot);
  Window_CloseFar((u32)gWorldMapWork.window, 2);
}
