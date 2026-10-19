/// @file
/// Item menu: position an inventory icon on a grid.

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
#include "m7_interfaces.h"

/// Places item i (or 0 if above 15) in a grid with cols columns of
/// 24x16-pixel cells.
/// x0, y0: top left of the grid.
void ItemMenu_PlaceIcon(struct RenderOutput **slot, s32 i, s32 x0, s32 y0,
                       s32 cols) {
  struct RenderOutput *obj;

  if (i > 15)
    i = 0;
  obj = *slot;
  obj->y = i / cols * 16 + y0;
  obj->x = i % cols * 24 + x0;
  UiIcon_PrepareObject(obj);
}
