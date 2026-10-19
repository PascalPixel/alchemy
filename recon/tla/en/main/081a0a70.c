// Near miss: differ 2 of 152 (was 10 with the one-pass block on the table
// store). Pinning the table cell to r5 and each loop's OAM attribute word to
// r0 behind a volatile barrier issues the native's pool loads first. Left: the
// grid loop's preheader, where the native loads 0x40004000 (ldr r3, then r10)
// between 16's movs and its copy to lr; a forced temporary for the word loads
// it first but swaps r2 and r3 (differ 8), a pin to r10 the same, an explicit
// x += 8 column worse (differ 45). Three permuter minutes found nothing.
#include "types.h"
#include "mem.h"
#include "dma.h"
#include "callback_scheduler.h"

struct DisplayScrollObject {
  u32 attr01;
  u32 attr2;
};

extern u16 gDisplayScroll_Position;
extern s16 gDisplayScroll_Busy;
extern s16 gDisplayScroll_LastPosition;
extern struct DisplayScrollObject *gDisplayScroll_Objects;
extern const void *gDisplayScroll_LineTable[];

void DisplayScroll_UpdateObjects(void);
void DisplayScroll_RenderEnteringLine(void);
s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align);

void DisplayScroll_InitObjectTable(void) {
  /* FAKEMATCH: retain the existing pointer-cell and postincrement word
     stores; direct typed stores in f4f9d28 change the 376-byte body to 368. */
  volatile u32 fill;
  u32 *entry;
  u32 i;
  u32 j;

  void *objects =
      Runtime_BumpAllocateAlternatePool(128 * sizeof(struct DisplayScrollObject));
  register void **cell asm("r5") = (void **)&gDisplayScroll_Objects;

  __asm__ __volatile__("" : "+l"(cell));
  *cell = objects;
  fill = 0;
  Dma_Set((void *)&fill, (void *)0x06010000, 0x85001800,
          (volatile u32 *)0x040000d4);
  fill = 0x11111111;
  Dma_Set((void *)&fill, (void *)0x06016000, 0x85000040,
          (volatile u32 *)0x040000d4);

  entry = *(u32 **)cell;
  {
    register u32 attr asm("r0") = 0x80004000;

    __asm__ __volatile__("" : "+l"(attr));
  for (i = 0; i < 8; i++) {
    u32 *q = entry;

    *q++ = (i << 21) | attr;
    *q = 0x300;
    entry += 2;
  }
  }
  {
    register u32 attr asm("r0") = 0x80004088;

    __asm__ __volatile__("" : "+l"(attr));
    for (i = 0; i < 8; i++) {
      u32 *q = entry;

      *q++ = (i << 21) | attr;
      *q = 0x300;
      entry += 2;
    }
  }
  {
    register u32 attr asm("r0") = 0x40004098;

    __asm__ __volatile__("" : "+l"(attr));
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
  Scheduler_AddOrUpdateCallback((s32)DisplayScroll_UpdateObjects, 0xc80);
  Scheduler_AddOrUpdateCallback((s32)DisplayScroll_RenderEnteringLine, 0x480);
  for (i = 0; i < 32; i++)
    DisplayScroll_DrawLine(gDisplayScroll_LineTable[0], i * 24, 1);
}
