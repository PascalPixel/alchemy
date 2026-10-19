/// @file
/// Display: place an ability icon in a window as a sprite.

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "render_input.h"
#include "types.h"
#include "resource.h"
#include "vram_tab.h"

/// Load the icon into a free VRAM cache slot and draw a 16x16 sprite.
/// Do nothing if no slot is free.
/// input: destination window.
/// x, y: position within the window, in pixels.
/// ability: ability index.
s32 Ui_CreateOutputFromResourceSlot(struct RenderInput *input, s32 x, s32 y,
                                    s32 ability) {
  s32 slot;

  slot = Resource_FindFreeEntry();
  if (slot != VRAM_CACHE_ENTRY_COUNT) {
    Resource_LoadIndexedEntryToBuffer(ability, slot);
    RenderOutput_Create(slot, 0x40000000, input, x, y); // 16x16 OBJ size.
  }
}
