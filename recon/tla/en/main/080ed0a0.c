// First draft of Func_080ed0a0, written from the listing: the world map
// screen's scroll follows the cursor. The target puts the cursor 120 by 80
// pixels in (clamped to the map); unless the map is held (Data_0202a640) the
// scroll moves a quarter of the way there each frame, at most 8 pixels, and
// stops when nearer than a quarter pixel. Each time the scroll crosses an
// 8-pixel column or row, the new column (Func_080ed030) or row
// (Func_080ecf74) of the picture is drawn; BG1 scrolls with it. All values
// are 16.16 pixels.
//
// Differ 72 of 233 (draft 231). The scroll is read at fixed addresses: the
// native loads 0x0202a634 from its own literal, or as 0x0202a630 + 4 where
// that is live, which only constants give (as extern scalars or an array,
// differ 116 and 120); were it adopted, the scroll would need a fixed-address
// entry under O2. Left: the native builds each clamp's bound afresh in its
// own register (r1, r2) where this reuses r3, and hoists the 0xe00000 bound
// to the top.
#include "types.h"
#include "iwram_call.h"
#include "bg_scroll.h"

// The cursor's position.
extern s32 *Data_0202a62c;
extern s16 Data_0202a640;

s32 FixedSqrt(s32 value);
void Func_080ed030(s32 column, s32 row);
void Func_080ecf74(u32 column, s32 row);

// The scroll, at fixed addresses.
#define SCROLL_X (*(s32 *)0x0202a630)
#define SCROLL_Y (*(s32 *)0x0202a634)

void Func_080ed0a0(void) {
  s32 *cursor = Data_0202a62c;
  s32 x = *cursor++ - (120 << 16);
  s32 y = *cursor - (80 << 16);
  s32 dx;
  s32 dy;
  s32 distance;
  s32 step;
  u32 column;
  u32 row;

  if (x < 0)
    x = 0;
  if (x > 0x1100000)
    x = 0x1100000;
  if (y < 0)
    y = 0;
  if (y > 0xe00000)
    y = 0xe00000;
  if (Data_0202a640 == 0) {
    dx = (x - SCROLL_X) / 0x10000;
    dy = (y - SCROLL_Y) / 0x10000;
    distance = Iwram_Sqrt(dx * dx + dy * dy) << 16;
    dx = x - SCROLL_X;
    dy = y - SCROLL_Y;
    if (distance < 0x400000)
      distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy));
    step = distance / 4;
    if (step > 0x80000)
      step = 0x80000;
    if (distance >= 0x4000) {
      if (distance > step) {
        dx = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dx), step);
        dy = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dy), step);
      }
      x = SCROLL_X + dx;
      y = SCROLL_Y + dy;
      if (x < 0)
        x = 0;
      if (y < 0)
        y = 0;
      if (x > 0x1100000)
        x = 0x1100000;
      if (y > 0x1100000)
        y = 0x1100000;
    }
    column = (u32)x >> 19;
    row = (u32)y >> 19;
    if ((SCROLL_X ^ x) & 0x80000) {
      if (SCROLL_X < x)
        Func_080ed030(column + 30, row);
      else
        Func_080ed030(column, row);
    }
    if ((SCROLL_Y ^ y) & 0x80000) {
      if (SCROLL_Y < y)
        Func_080ecf74(column, row + 20);
      else
        Func_080ecf74(column, row);
    }
    gBgScroll[1].x = x >> 16;
    gBgScroll[1].y = y >> 16;
  }
  SCROLL_X = x;
  SCROLL_Y = y;
}
