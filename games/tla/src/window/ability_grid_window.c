#include "types.h"
#include "ram_buffer.h"
#include "oam.h"

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern const s16 gData_080aa170[4][9];

struct Window;
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 mode);
void Window_DrawDividerLine(struct Window *window, s32 x1, s32 y1, s32 x2,
                              s32 y2);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
s32 WindowSprite_Create(s32 resource, s32 flags, struct Window *window, s32 x,
                        s32 y);
void Func_08041268(s32 ability, struct Window *window, s32 x, s32 y);

/// Opens the ability grid's window: eight dividers, then the four columns
/// of gData_080aa170 in nine rows, as Func_08040cc4 draws its grid.
struct Window *Func_0804128c(void) {
  void *work = Ram_HeapSlots->unkd0[0];
  struct Window *window = Window_Create(0, 0, 30, 19, 2);
  const s16 *abilities;
  s32 i;
  s32 y;
  s32 rowY;

  for (i = 0, rowY = 2; i < 8; i++, rowY += 2)
    Window_DrawDividerLine(window, 0, rowY, 29, rowY);
  for (y = 0, i = 0; i < 9; y += 16, i++) {
    Func_08041268(gData_080aa170[0][i], window, 8, y);
    Func_08041268(gData_080aa170[1][i], window, 64, y);
    Func_08041268(gData_080aa170[2][i], window, 120, y);
    Func_08041268(gData_080aa170[3][i], window, 176, y);
  }
  WORD_AT(work, 1444) = WindowSprite_Create(
      Resource_LoadFixedBlockBIntoFreeSlot(), OUTPUT_SIZE_16X16, window, 0, 0);
  return window;
}
