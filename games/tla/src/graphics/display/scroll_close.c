#include "types.h"
#include "dma.h"

extern const void *gDisplayScroll_LineTable[];
extern u32 *gDisplayScroll_Objects;
s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align);
s32 Task_Remove(void (*func)(void));
void Func_081a0cac(void);
void Func_081a0d78(void);

void Func_081a0fb8(void) {
  s32 i;
  u32 *objects;

  for (i = 0; i < 32; i++)
    DisplayScroll_DrawLine(gDisplayScroll_LineTable[0], i * 24, 1);
  Task_Remove(Func_081a0cac);
  Task_Remove(Func_081a0d78);
  objects = gDisplayScroll_Objects;
  for (i = 0; i != 0x100; i++)
    *objects++ = 0;
  Dma_Set(gDisplayScroll_Objects, OAM, DMA_COPY_WORDS(256), REG_DMA3);
}
