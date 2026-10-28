// Under -mthumb-slow-loads (2026-10-26): differ 10 of 238 in tla-en,
// instruction order only. An empty asm before the loop lowers it to 8. Written
// with redraw set first, the decrements' -1 comes from the mask's 32 (subs r2,
// #33) and the registers swap (differ 29).
// Near miss, differ 10 of 238 (five reordered pairs), 2026-10-15: The
// Broken Seal's DebugMenu_BrowseIcons with twelve pages, gData_0805f8f0 to
// gData_0805f910 for the labels and the object system's suspend flag.
// Remaining: the native stores redraw (spilled to [sp, #4]) before it
// builds the zero for page, mode and the window, and in the two decrement
// cases builds redraw's 1, then the -1's 1, then stores redraw before it
// negates; this negates and adds first. Written with redraw set first the
// constants are shared (differ 29 and 39).

#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "sprite.h"
#include "render_input.h"

extern u8 gData_0805f8f0[];
extern u8 gData_0805f8f8[];
extern u8 gData_0805f900[];
extern u8 gData_0805f908[];
extern u8 gData_0805f910[];

void wait(s32 frames);
void Window_Close(struct RenderInput *window, s32 release);
struct RenderInput *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                    s32 style);
void Text_DrawStringInWindow(u8 *text, struct RenderInput *window, u32 x,
                               u32 y);
void Text_DrawNumberInWindow(s32 value, s32 width, struct RenderInput *window,
                               s32 x, s32 y);
void Icon_BuildItemIconTiles(s32 icon, s32 with_base, s32 *slot, s32 *tile, s32 reuse);
void Icon_BuildAbilityIconTiles(s32 icon, s32 with_base, s32 *slot, s32 *tile,
                                  s32 reuse);
s32 Resource_FindFreeEntry(void);
void Icon_BuildMiscIconTiles(s32 pattern, s32 unused, s32 slot);

s32 Func_0804e1d8(void) {
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
      window = Window_Create(10, 0, 18, 12, 2);
      if (mode == 0)
        Text_DrawStringInWindow(gData_0805f8f0, window, 0, 0);
      else if (mode == 1)
        Text_DrawStringInWindow(gData_0805f8f8, window, 0, 0);
      else
        Text_DrawStringInWindow(gData_0805f900, window, 0, 0);
      Text_DrawStringInWindow(gData_0805f908, window, 0, 8);
      Text_DrawNumberInWindow(page, 0, window, 40, 8);
      base = page << 5;
      Text_DrawNumberInWindow(base, 3, window, 64, 8);
      Text_DrawStringInWindow(gData_0805f910, window, 88, 8);
      Text_DrawNumberInWindow(base + 31, 3, window, 96, 8);
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
          slot = Resource_FindFreeEntry();
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
