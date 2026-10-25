// Near miss, differ 2 of 65 (27 before alchemy permute moved the line
// pointer's set-up after the row's and RenderOutput_Create took its fifth
// argument): the debug warp menu's window, nine preset names 16 pixels
// apart with a divider under each but the last. Left: the native loads
// gDebugWarp_Presets from the pool (ldr r7) before it copies y's 4 to sl;
// this copies first. Every order of the four set-ups and of the
// declarations measures 2 or more; an indexed for loop with the divider
// skipped on the first pass is differ 59.
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

struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                    s32 style);
void UiWindow_DrawDividerLine(struct RenderInput *window, s32 x, s32 y,
                              s32 right, s32 bottom);
void UiText_DrawStringInWindow(const u8 *text, struct RenderInput *window,
                               s32 x, s32 y);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
void *RenderOutput_Create(s32 slot, s32 attributes, struct RenderInput *window,
                          s32 x, s32 y);

struct RenderInput *Func_08041068(void) {
  struct ScreenWork *work = Ram_HeapSlots->unkd0[0];
  struct RenderInput *window;
  struct ScreenLine *line;
  s32 i;
  s32 row;
  s32 y;

  window = UiWindow_Create(0, 0, 30, 19, 2);
  y = 4;
  i = 0;
  row = 2;
  line = gDebugWarp_Presets;
  for (;;) {
    UiText_DrawStringInWindow(line->text, window, 8, y);
    y += 16;
    line++;
    if (i == 8)
      break;
    UiWindow_DrawDividerLine(window, 0, row, 29, row);
    row += 2;
    i++;
  }
  work->cursor = RenderOutput_Create(Resource_LoadFixedBlockBIntoFreeSlot(),
                                     0x40000000, window, 0, 0);
  return window;
}
