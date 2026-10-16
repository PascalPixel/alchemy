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
#include "animspr.h"
#include "render_input.h"

extern u8 gData_0805f8f0[];
extern u8 gData_0805f8f8[];
extern u8 gData_0805f900[];
extern u8 gData_0805f908[];
extern u8 gData_0805f910[];

void WaitFrames(s32 frames);
void UiWork_Finalize(struct RenderInput *window, s32 release);
struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                    s32 style);
void UiText_DrawStringInWindow(u8 *text, struct RenderInput *window, u32 x,
                               u32 y);
void UiText_DrawNumberInWindow(s32 value, s32 width, struct RenderInput *window,
                               s32 x, s32 y);
void Func_0803d5c4(s32 icon, s32 with_base, s32 *slot, s32 *tile, s32 reuse);
void UiIcon_BuildAbilityIconTiles(s32 icon, s32 with_base, s32 *slot, s32 *tile,
                                  s32 reuse);
s32 Resource_FindFreeEntry(void);
void Ui_BuildPatternToSlot(s32 pattern, s32 unused, s32 slot);

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
  WaitFrames(1);

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
      UiWork_Finalize(window, 2);
      window = UiWindow_Create(10, 0, 18, 12, 2);
      if (mode == 0)
        UiText_DrawStringInWindow(gData_0805f8f0, window, 0, 0);
      else if (mode == 1)
        UiText_DrawStringInWindow(gData_0805f8f8, window, 0, 0);
      else
        UiText_DrawStringInWindow(gData_0805f900, window, 0, 0);
      UiText_DrawStringInWindow(gData_0805f908, window, 0, 8);
      UiText_DrawNumberInWindow(page, 0, window, 40, 8);
      base = page << 5;
      UiText_DrawNumberInWindow(base, 3, window, 64, 8);
      UiText_DrawStringInWindow(gData_0805f910, window, 88, 8);
      UiText_DrawNumberInWindow(base + 31, 3, window, 96, 8);
      for (i = 0; i <= 31; i++) {
        slot = -1;
        row = i / 8;
        x = (i - row * 8) * 16;
        y = row * 16 + 16;
        if (mode == 0) {
          Func_0803d5c4(base + i, 1, &slot, &tile, mode);
          RenderOutput_Create(slot, 0x40000000, window, x, y);
        } else if (mode == 1) {
          UiIcon_BuildAbilityIconTiles(base + i, 1, &slot, &tile, 0);
          RenderOutput_Create(slot, 0x40000000, window, x, y);
        } else {
          slot = Resource_FindFreeEntry();
          Ui_BuildPatternToSlot(i, 0, slot);
          RenderOutput_Create(slot, 0x40000000, window, x, y);
        }
      }
    }
    WaitFrames(1);
  }
  UiWork_Finalize(window, 2);
  Ram_HeapSlots->objectSystem->suspended = 0;
  return 0;
}
