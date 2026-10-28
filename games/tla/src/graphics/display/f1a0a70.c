#include "types.h"
#include "mem.h"
#include "dma.h"
#include "task.h"

struct DisplayScrollObject {
  u32 attr01;
  u32 attr2;
};

extern u16 gDisplayScroll_Position;
extern s16 gDisplayScroll_Busy;
extern s16 gDisplayScroll_LastPosition;
extern struct DisplayScrollObject *gDisplayScroll_Objects;
extern const void *gDisplayScroll_LineTable[];

void Func_081a08c4(void);
void DisplayScroll_RenderEnteringLine(void);
s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align);

void Func_081a0a70(void) {
  /* FAKEMATCH(volatile): the DMA fill word volatile; as a plain u32, 6 of 152
   * instructions differ in tla-en. */
  volatile u32 fill;
  u32 *entry;
  u32 i;
  u32 j;

  void *objects =
      Runtime_BumpAllocateAlternatePool(128 * sizeof(struct DisplayScrollObject));
  void **cell = (void **)&gDisplayScroll_Objects;
  *cell = objects;
  fill = 0;
  Dma_Set((void *)&fill, (void *)0x06010000, 0x85001800,
          (volatile u32 *)0x040000d4);
  fill = 0x11111111;
  Dma_Set((void *)&fill, (void *)0x06016000, 0x85000040,
          (volatile u32 *)0x040000d4);

  entry = *(u32 **)cell;
  {
    u32 attr = 0x80004000;
  for (i = 0; i < 8; i++) {
    u32 *q = entry;

    *q++ = (i << 21) | attr;
    *q = 0x300;
    entry += 2;
  }
  }
  {
    u32 attr = 0x80004088;
    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = 0x300;
      entry += 2;
    }
  }
  {
    u32 attr = 0x40004098;
    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = 0x300;
      entry += 2;
    }
  }
  for (i = 0; i < 16; i++) {
    for (j = 0; j < 6; j++) {
      u32 *q = entry;
      u32 tile = (i * 3) * 8 + j * 4;

      *q++ = (i * 8 + 16) | ((j * 32 + 24) << 16) | 0x40004000;
      *q = tile;
      entry += 2;
    }
  }
  for (i = 0; i < 8; i++) {
    u32 *q = entry;

    *q++ = 0x00c000c0;
    *q = 0x300;
    entry += 2;
  }
  gDisplayScroll_Position = 0;
  gDisplayScroll_LastPosition = 0;
  gDisplayScroll_Busy = 0;
  Task_Add((s32)Func_081a08c4, TASK_PASS_FRAME);
  Task_Add((s32)DisplayScroll_RenderEnteringLine, TASK_PASS_VBLANK);
  for (i = 0; i < 32; i++)
    DisplayScroll_DrawLine(gDisplayScroll_LineTable[0], i * 24, 1);
}
