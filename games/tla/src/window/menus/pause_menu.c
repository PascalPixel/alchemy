/// @file
/// The save screen's selection, the portrait browser, and the flag grid drawn
/// into a tilemap.
// Part of menus.c.

#include "../menus_internal.h"
#include "item_icon.h"
#include "module/window/menus.h"
#include "module/window/window.h"

/// Runs the save screen's selection until a choice completes: saving (refused
/// while flag 0x17c or 0x17d is set), sleep mode or the options. Returns -1
/// when the screen is cancelled and 0 otherwise.
s32 PauseMenu_Run(void) {
  s32 selection = 0;

again:
  selection = PauseMenu_Choose(selection);
  if (selection == -1)
    return -1;
  if (selection == 0) {
    if (GameFlag_Test(0x17c)) {
      FieldMessage_Show((s32)MsgCannotSaveHere, 1);
      goto again;
    }
    if (GameFlag_Test(0x17d)) {
      FieldMessage_Show((s32)MsgNoPlaceForThat, 1);
      goto again;
    }
    if (Save_ToChosenFile() == -1)
      goto again;
  } else if (selection == 1) {
    FieldMessage_Show((s32)MsgSleepMode, 1);
    gSleepRequested = selection;
  } else if (selection == 2) {
    if (SettingsMenu_Run() == -1)
      goto again;
  }
  return 0;
}

void Render_SetTilemapFlagBitRect(const struct Window *object, s32 x, s32 y,
                                  s32 width, s32 height, u32 field) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;

  x += (s16)object->x + 1;
  y += (s16)object->y + 1;
  field &= 1;
  field <<= 12;
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29) {
    width = 30 - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29) {
    height = 20 - y;
  }
  if (width > 0 && height > 0) {
    do {
      u16 *pixel = &work->tilemap[y][x];
      s32 remaining = width;
      while (remaining != 0) {
        u32 value = *pixel;
        value &= 0xFFFFEFFF;
        value |= field;
        remaining--;
        *pixel = value;
        pixel++;
      }
      height--;
      y++;
    } while (height != 0);
    work->dirty = 1;
  }
}

/* VIEW(flag_bit_rect): routine Window_Create as struct RenderInput
 * *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 style); selrow has
 * struct Work *Window_Create(s32 kind, s32 x, s32 y, s32 width, s32 layer). */
extern struct RenderInput *
Window_Create__flag_bit_rect(s32 x, s32 y, s32 width, s32 height,
                             s32 style) __asm__("Window_Create");
/* VIEW(flag_bit_rect): routine Text_DrawStringInWindow as void
 * Text_DrawStringInWindow(u8 *text, struct RenderInput *window, u32 x, u32 y);
 * handle_selection_row_input has void Text_DrawStringInWindow(s32 text, s32
 * window, s32 x, s32 y). */
extern void Text_DrawStringInWindow__flag_bit_rect(
    u8 *text, struct RenderInput *window, u32 x,
    u32 y) __asm__("Text_DrawStringInWindow");
/* VIEW(flag_bit_rect): routine Text_DrawNumberInWindow as void
 * Text_DrawNumberInWindow(s32 value, s32 width, struct RenderInput *window, s32
 * x, s32 y); menus has void Text_DrawNumberInWindow(s32 value, s32 digits,
 * struct TargetWindow *window, s32 x, s32 y). */
extern void Text_DrawNumberInWindow__flag_bit_rect(
    s32 value, s32 width, struct RenderInput *window, s32 x,
    s32 y) __asm__("Text_DrawNumberInWindow");
void Icon_BuildItemIconTiles(s32 icon, s32 with_base, s32 *slot, s32 *tile,
                             s32 reuse);
void Icon_BuildMiscIconTiles(s32 pattern, s32 unused, s32 slot);

/// Browses pages of item, Psynergy and status-pattern icons, suspending the
/// field's actor system while the browser owns its window and cache entries.
/// L and R switch the kind, Left and Right turn its 8 pages of 32 icons
/// (item and Psynergy icons page * 32 on; the status patterns are 0 to 31 on
/// every page, whatever the heading says), A or B closes it. Returns 0. The
/// debug menus open it (debug/test.c, debug/level.c).
s32 DebugBrowser_Icons(void) {
  s32 redraw;
  s32 tile;
  s32 page;
  s32 mode;
  struct RenderInput *window;
  s32 slot;
  s32 base;
  s32 i;
  s32 row;
  s32 x;
  s32 y;

  window = NULL;
  page = 0;
  mode = 0;
  redraw = 1;
  Ram_HeapSlots->objectSystem->suspended = redraw;
  wait(1);

  while (1) {
    if (gInput.repeat & 0x20) {
      page--;
      redraw = 1;
    }
    if (gInput.repeat & 0x10) {
      redraw = 1;
      page++;
    }
    if (gInput.repeat & 0x200) {
      mode--;
      redraw = 1;
    }
    if (gInput.repeat & 0x100) {
      redraw = 1;
      mode++;
    }
    if (gInput.repeat & 1)
      break;
    if (gInput.repeat & 2)
      break;
    if (redraw != 0) {
      redraw = 0;
      page = (page + 12) % 12;
      mode = (mode + 3) % 3;
      Window_Close(window, 2);
      window = Window_Create__flag_bit_rect(10, 0, 18, 12, 2);
      if (mode == 0)
        Text_DrawStringInWindow__flag_bit_rect(gWorkspace_ItemTitle, window, 0,
                                               0);
      else if (mode == 1)
        Text_DrawStringInWindow__flag_bit_rect(gWorkspace_EnergyTitle, window,
                                               0, 0);
      else
        Text_DrawStringInWindow__flag_bit_rect(gWorkspace_StatusTitle, window,
                                               0, 0);
      Text_DrawStringInWindow__flag_bit_rect(gWorkspace_PageLabel, window, 0,
                                             8);
      Text_DrawNumberInWindow__flag_bit_rect(page, 0, window, 40, 8);
      base = page << 5;
      Text_DrawNumberInWindow__flag_bit_rect(base, 3, window, 64, 8);
      Text_DrawStringInWindow__flag_bit_rect(gWorkspace_RangeDash, window, 88,
                                             8);
      Text_DrawNumberInWindow__flag_bit_rect(base + 31, 3, window, 96, 8);
      for (i = 0; i <= 31; i++) {
        slot = -1;
        row = i / 8;
        x = (i - row * 8) * 16;
        y = row * 16 + 16;
        if (mode == 0) {
          Icon_BuildItemIconTiles(base + i, 1, &slot, &tile, mode);
          WindowSprite_Create(slot, 0x40000000, window, x, y);
        } else if (mode == 1) {
          Icon_BuildAbilityIconTiles(base + i, 1, &slot, &tile, 0);
          WindowSprite_Create(slot, 0x40000000, window, x, y);
        } else {
          slot = VramCache_FindFreeEntry();
          Icon_BuildMiscIconTiles(i, 0, slot);
          WindowSprite_Create(slot, 0x40000000, window, x, y);
        }
      }
    }
    wait(1);
  }
  Window_Close(window, 2);
  Ram_HeapSlots->objectSystem->suspended = 0;
  return 0;
}

extern s16 gPortrait_CharacterMap[];
extern s16 gPortrait_SpriteMap[];

struct RenderInput *Portrait_OpenWindow(s32 x, s32 y, s32 width, s32 height);
/* VIEW(flag_bit_rect): routine WindowSprite_PrepareForRedraw as void
 * WindowSprite_PrepareForRedraw(struct RenderInput *window); resource_work has
 * void WindowSprite_PrepareForRedraw(struct Window *). */
extern void WindowSprite_PrepareForRedraw__flag_bit_rect(
    struct RenderInput *window) __asm__("WindowSprite_PrepareForRedraw");

/// A debug browser of the portraits: left and right step through
/// gPortrait_CharacterMap and then gPortrait_SpriteMap, L and R by ten, each
/// drawn with its number and its name from MsgTestName; A or B leaves.
s32 DebugBrowser_Portraits(void) {
  struct WindowWork *work;
  struct RenderInput *portrait;
  s32 redraw;
  struct RenderInput *window;
  s32 count;
  s32 total;
  s32 index;
  s32 i;
  s32 glyph;
  s32 tile;
  s32 slot;
  s32 cancel;
  u32 slotOffset;
  s32 palette;

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  redraw = 1;
  portrait = Portrait_OpenWindow(0, 0, 10, 5);
  window = Window_Create__flag_bit_rect(10, 10, 14, 3, 2);
  index = 0;
  palette = 15;
  for (i = 0; gPortrait_CharacterMap[i * 2] != -1; i++) {
  }
  count = i;
  for (i = 0; gPortrait_SpriteMap[i * 2] != -1; i++) {
  }
  total = count + i;
  /* FAKEMATCH(dead): a zero from the count keeps the mask in r9 and work in
   * fp; as cancel = 2, 30 of 180 instructions differ in tla-en. */
  cancel = ((u32)(u16)count >> 16) | 2;
  slotOffset = offsetof(struct WindowWork, outputIds[1]);
  while (1) {
    if (gInput.repeat & 0x20) {
      redraw = 1;
      index--;
    }
    if (gInput.repeat & 0x10) {
      redraw = 1;
      index++;
    }
    if (gInput.repeat & 0x200) {
      redraw = 1;
      index -= 10;
    }
    if (gInput.repeat & 0x100) {
      redraw = 1;
      index += 10;
    }
    if (gInput.repeat & 1)
      break;
    if (gInput.repeat & cancel)
      break;
    if (redraw) {
      redraw = 0;
      index = (index + total) % total;
      WindowSprite_PrepareForRedraw__flag_bit_rect(window);
      if (index < count)
        glyph = gPortrait_CharacterMap[index * 2 + 1];
      else
        glyph = gPortrait_SpriteMap[(index - count) * 2 + 1] + 128;
      slot = *(u16 *)((u8 *)work + slotOffset);
      Icon_LoadWithPalette(glyph, 0, &slot, &tile, palette, 1);
      Text_DrawNumberInWindow__flag_bit_rect(index, 2, window, 0, 0);
      Text_DrawMessageAsTiles(index + (s32)MsgTestName, window, 24, 0);
    }
    wait(1);
  }
  Window_Close((struct Window *)window, 2);
  Window_Close((struct Window *)portrait, 2);
  wait(1);
  return 0;
}
