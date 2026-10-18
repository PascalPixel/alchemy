// First draft of Func_080f8a44: places the hand cursor at (xOffset, yOffset)
// inside the menu's window, bobbing; the twin of ☀️'s UiMenu_PositionCursor
// (games/tbs/src/menu/menu.c). Near miss, differ 6 of 43, instruction order
// only: the native copies the window to ip and loads the 0xffff mask before
// the x sum is finished (before lsls r3, r3, #3 and adds r2, #8); here both
// come one step later.
#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct OwnerMenuState {
  u8 unk000[16];
  struct RenderInput *ownerWindow;
  struct RenderOutput *ownerCursors[2];
};

extern u32 gFrameCount;
extern u8 gUiMenu_CursorBobX[];
extern u8 gUiMenu_CursorBobY[];

#define CURSOR_BOB_STEP() ((gFrameCount >> 1) & 7)

void Func_080f8a44(s32 xOffset, s32 yOffset) {
  struct OwnerMenuState *work =
      (struct OwnerMenuState *)Ram_HeapSlots->menuRuntime;
  struct RenderOutput *cursor = work->ownerCursors[0];

  cursor->oam.x = cursor->x = gUiMenu_CursorBobX[CURSOR_BOB_STEP()] + xOffset +
                              work->ownerWindow->x * 8 + 8;
  cursor->oam.y = cursor->y = gUiMenu_CursorBobY[CURSOR_BOB_STEP()] + yOffset +
                              work->ownerWindow->y * 8 + 8;
}
