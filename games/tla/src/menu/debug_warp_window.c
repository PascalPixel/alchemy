#include "types.h"
#include "ram_buffer.h"

struct RenderInput;

struct ScreenWork {
  u8 unk000[0x5a4];
  void *cursor;
};

struct ScreenLine {
  const u8 *text;
  s32 unk4;
};

extern struct ScreenLine gDebugWarp_Presets[];

struct RenderInput *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                    s32 style);
void Window_DrawDividerLine(struct RenderInput *window, s32 x, s32 y,
                              s32 right, s32 bottom);
void Text_DrawStringInWindow(const u8 *text, struct RenderInput *window,
                               s32 x, s32 y);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
void *WindowSprite_Create(s32 slot, s32 attributes, struct RenderInput *window,
                          s32 x, s32 y);

/// Opens the debug warp menu's window: nine preset names 16 pixels apart,
/// with a divider under each but the last.
struct RenderInput *Func_08041068(void) {
  struct ScreenWork *work = Ram_HeapSlots->unkd0[0];
  struct RenderInput *window;
  struct ScreenLine *line;
  s32 i;
  s32 row;
  s32 y;

  window = Window_Create(0, 0, 30, 19, 2);
  y = 4;
  i = 0;
  row = 2;
  line = gDebugWarp_Presets;
  for (;;) {
    Text_DrawStringInWindow(line->text, window, 8, y);
    y += 16;
    line++;
    if (i == 8)
      break;
    Window_DrawDividerLine(window, 0, row, 29, row);
    row += 2;
    i++;
  }
  work->cursor = WindowSprite_Create(Resource_LoadFixedBlockBIntoFreeSlot(),
                                     0x40000000, window, 0, 0);
  return window;
}
