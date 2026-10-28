#include "types.h"
#include "graphics.h"
#include "resource.h"
#include "render_input.h"

struct OwnerMenuState {
  u8 unk000[16];
  struct RenderInput *window;
  struct WindowSprite *cursors[2];
  s8 tabIndex[2];
  u8 unk01e[0x15a];
  struct WindowSprite *arrows[2];
};

extern char ResourceId_ShopUpArrowTiles, ResourceId_ShopDownArrowTiles;

s32 Window_UpdateOrCreate(struct RenderInput **handle, s32 x, s32 y, s32 w,
                            s32 h, s32 mode);
struct WindowSprite *
Icon_CreateWithResourceVariant(struct RenderInput *window, s32 x, s32 y);

/// Opens the menu's top-left window (13 by 5 tiles) with a divider, puts the
/// hand cursor in it, hidden, and adds the up and down arrow icons at its right
/// edge when a VRAM entry is free. Returns the window.
// The twin of ☀️'s UiMenu_CreateCursor (games/tbs/src/menu/menu.c), which has
// no divider or arrows.
struct RenderInput *Func_080f88d0(struct OwnerMenuState *work) {
  struct RenderInput *window;
  struct WindowSprite *icon;
  s32 zero = 0;
  s32 sentinel;
  s32 slot;

  work->window = NULL;
  Window_UpdateOrCreate(&work->window, 0, 0, 13, 5, 2);
  window = work->window;
  Window_DrawDividerLineFar((s32)window, 0, 3, 12, 3);
  icon = Icon_CreateWithResourceVariant(window, -8, 11);
  icon->active = 13;
  work->tabIndex[0] = 255;
  work->cursors[0] = icon;
  sentinel = 254;
  icon->sentinel = sentinel;
  sentinel -= 255;
  work->tabIndex[1] = zero;
  work->cursors[1]->sentinel = sentinel;
  slot = Resource_FindFreeEntry();
  if (slot <= 95) {
    VramBlock_LoadResourceFar(slot, 128, (s32)&ResourceId_ShopUpArrowTiles);
    icon = WindowSprite_CreateFar(slot, OUTPUT_SIZE_16X16, window, 84, -4);
    icon->active = 13;
    work->arrows[0] = icon;
  }
  slot = Resource_FindFreeEntry();
  if (slot <= 95) {
    VramBlock_LoadResourceFar(slot, 128, (s32)&ResourceId_ShopDownArrowTiles);
    icon = WindowSprite_CreateFar(slot, OUTPUT_SIZE_16X16, window, 84, 8);
    icon->active = 13;
    work->arrows[1] = icon;
  }
  return window;
}
