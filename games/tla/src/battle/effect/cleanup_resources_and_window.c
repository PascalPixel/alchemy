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

struct WorldMapWork {
  u16 resources[2];
  u8 unk04[0x18];
  u32 window;
};

extern void UiWork_FinalizeFar(u32 arg0, u32 arg1);

void BattleEffect_CleanupResourcesAndWindow(void) {
  Resource_ResetEntry(gWorldMapWork.resources[0]);
  Resource_ResetEntry(gWorldMapWork.resources[1]);
  UiWork_FinalizeFar(gWorldMapWork.window, 2);
}
