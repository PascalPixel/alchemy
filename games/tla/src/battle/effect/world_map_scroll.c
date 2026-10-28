#include "types.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "bg_scroll.h"
#include "fixed_math.h"

/// The world map cursor's position (x, then y, 16.16 pixels).
extern s32 *Data_0202a62c;
/// Set while the world map shows its alternative picture, which does not
/// scroll.
extern s16 Data_0202a640;

s32 FixedSqrt(s32 value);
void Func_080ed030(s32 column, s32 row);
void Func_080ecf74(u32 column, s32 row);

#define SCROLL_X Ram_WorldMapScroll[0]
#define SCROLL_Y Ram_WorldMapScroll[1]

/// The world map screen's scroll follows the cursor. The target puts the
/// cursor 120 by 80 pixels in (clamped to the map); unless the map is held the
/// scroll moves a quarter of the way there each frame, at most 8 pixels, and
/// stops when nearer than a quarter pixel. Each time the scroll crosses an
/// 8-pixel column or row, the new column (Func_080ed030) or row
/// (Func_080ecf74) of the picture is drawn; BG1 scrolls with it. All values
/// are 16.16 pixels.
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
  s32 limit;
  s32 bottom = 0xe00000;

  if (x < 0)
    x = 0;
  limit = 0x1100000;
  if (x > limit)
    x = limit;
  if (y < 0)
    y = 0;
  if (y > bottom)
    y = bottom;
  if (Data_0202a640 == 0) {
    dx = (x - SCROLL_X) / Q16_ONE;
    dy = (y - SCROLL_Y) / Q16_ONE;
    distance = Iwram_Sqrt(dx * dx + dy * dy) << 16;
    dx = x - SCROLL_X;
    dy = y - SCROLL_Y;
    // Near the target the whole-pixel distance is too coarse.
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
      if (x > limit)
        x = limit;
      if (y > limit)
        y = limit;
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
