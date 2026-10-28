#include "types.h"
#include "ram_buffer.h"
#include "oam.h"
#include "dma.h"
#include "io_reg.h"
#include "mem.h"
#include "task.h"

void Func_08108030(void *panel);

struct ScreenWork {
  u8 unk000[0x5a4];
  u8 panel[0x84];
};

void Func_08040c60(void) {
  struct ScreenWork *work = Ram_HeapSlots->unkd0[0];

  Func_08108030(work->panel);
}

void Func_08040c78(void) {
  void *work = HeapSlot_AllocEwram(208, 0x628);

  Dma_Fill32(0, work, 0x628);
  Task_Add(Func_08040c60, TASK_PASS_VBLANK);
}

void Func_08040cb0(void) {
  Task_Remove(Func_08040c60);
  HeapSlot_Release(208);
}

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern u8 MsgFullPpAndMoney[];

struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 mode);
void Window_DrawDividerLine(struct Window *window, s32 x, s32 y, s32 width,
                              s32 row);
void Text_DrawMessage(s32 message, struct Window *window, s32 x, s32 y);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
s32 WindowSprite_Create(s32 resource, s32 flags, struct Window *window, s32 x,
                        s32 y);

struct Window *Func_08040cc4(void) {
  void *work = Ram_HeapSlots->unkd0[0];
  struct Window *window = Window_Create(6, 2, 18, 16, 2);
  s32 i;

  for (i = 0; i < 4; i++)
    Window_DrawDividerLine(window, 0, i * 3 + 3, 17, i * 3 + 3);
  Text_DrawMessage((s32)MsgFullPpAndMoney, window, 8, 4);
  Text_DrawMessage((s32)MsgFullPpAndMoney + 1, window, 8, 28);
  Text_DrawMessage((s32)MsgFullPpAndMoney + 2, window, 8, 52);
  Text_DrawMessage((s32)MsgFullPpAndMoney + 3, window, 8, 76);
  Text_DrawMessage((s32)MsgFullPpAndMoney + 4, window, 8, 100);
  WORD_AT(work, 1444) = WindowSprite_Create(
      Resource_LoadFixedBlockBIntoFreeSlot(), OUTPUT_SIZE_16X16, window, 0, 0);
  return window;
}
