// Near miss, differ 2 of 122: the twin of The Broken Seal's UiMenu_SlideCursor.
// The native loads the 0x1ff mask of the sprite's x field before it subtracts
// 56 (adds r1, r1, r3; ldr r3, [pc]; subs r1, #56); here the subtraction comes
// first. The mask belongs to the bit-field store, so no source value can carry
// a barrier; splitting the sum from the subtraction changes nothing.
#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct OwnerMenuState {
  u8 unk000[16];
  struct RenderInput *ownerWindow;
  struct RenderOutput *ownerCursors[2];
  s8 ownerIndex[2];
  u8 unk01e[0x1fe];
  u16 page;
  u16 skipSlide;
};

void WaitFrames(s32 frames);

void Func_080f8ab4(s32 x, s32 y) {
  struct OwnerMenuState *work =
      (struct OwnerMenuState *)Ram_HeapSlots->menuRuntime;
  struct RenderOutput *cursor;
  s32 steps;
  s32 startX;
  s32 startY;
  s32 px;
  s32 py;
  s32 dx;
  s32 dy;

  steps = 2;
  if (work->skipSlide != 0) {
    work->skipSlide = 0;
    return;
  }
  cursor = work->ownerCursors[0];
  {
    s32 spriteX = cursor->oam.x;
    s32 spriteY = cursor->oam.y;

    cursor->x = spriteX + 64;
    cursor->y = spriteY + 64;
  }
  x += 64;
  y += 64;
  if (cursor->x - 8 > 0)
    cursor->x -= 8;
  py = cursor->y;
  if (py - 8 > 0) {
    cursor->y -= 8;
    py = cursor->y;
  }
  startX = cursor->x << 4;
  dx = ((x << 4) - startX + 1) / steps;
  startY = py << 4;
  dy = ((y << 4) - startY + 1) / steps;
  px = startX;
  py = startY;
  do {
    px += dx;
    {
      s32 spriteX = (px >> 4) + (work->ownerWindow->x << 3);

      cursor->oam.x = cursor->x = spriteX - 56;
    }
    py += dy;
    cursor->oam.y = cursor->y = (py >> 4) + (work->ownerWindow->y << 3) - 56;
    steps--;
    if (steps != 0)
      WaitFrames(1);
  } while (steps != 0);
}
