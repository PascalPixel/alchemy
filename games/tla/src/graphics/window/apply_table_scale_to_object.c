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
#include "iwram_call.h"
#include "scene.h"
#include "uiwindow.h"

void UiWindow_DrawThreeTileColumn(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
  s32 tile_offset = arg3 * 2;
  s32 tile = tile_offset + 0xF315;

  UiWindow_SetTilemapEntry((struct UiWindow *)arg0, 0x400 | tile, arg1, arg2,
                           0);
  UiWindow_SetTilemapEntry((struct UiWindow *)arg0, tile_offset + 0xF314,
                           arg1 + 1, arg2, 0);
  UiWindow_SetTilemapEntry((struct UiWindow *)arg0, tile, arg1 + 2, arg2, 0);
}
