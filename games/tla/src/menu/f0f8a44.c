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

/// Places the hand cursor at (xOffset, yOffset) inside the menu's window,
/// bobbing; the twin of The Broken Seal's UiMenu_PositionCursor.
void Func_080f8a44(s32 xOffset, s32 yOffset) {
  struct OwnerMenuState *work =
      (struct OwnerMenuState *)Ram_HeapSlots->menuRuntime;
  struct RenderOutput *cursor = work->ownerCursors[0];

  cursor->oam.x = cursor->x = gUiMenu_CursorBobX[CURSOR_BOB_STEP()] + xOffset +
                              work->ownerWindow->x * 8 + 8;
  cursor->oam.y = cursor->y = gUiMenu_CursorBobY[CURSOR_BOB_STEP()] + yOffset +
                              work->ownerWindow->y * 8 + 8;
}
