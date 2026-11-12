/// @file
/// Scene code of overlay 0x65f, in ROM order, from s02a4's offset up to the next object the overlay links.

#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "dma.h"
#include "io_reg.h"
#include "event_inline.h"
#include "../../../../games/tla/src/scene/garoh/garoh.h"
#include "palette_buffers.h"
#include "system/io_reg.h"
#include "garoh.h"

extern u32 gFrameCount;
extern u16 Data_02006340[];

void Func_020002a4(void) {
  // Declared first: x then wins its allocation tie with sine.
  s32 x;
  // Math_Sine is an 8-byte far-call veneer: its target address is the word
  // after its first instruction.
  s32 (*sine)(s32) = *(s32 (**)(s32))(((u32)Math_Sine & ~3) + 4);
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  /* FAKEMATCH(pin): line held in r5; as a plain local, 70 of 110 instructions
   * differ in tla-en. */
  register u16 *line asm("r5") = Data_02006340;

  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xca4) == 0) {
    u32 frame;
    s32 z, a, b;
    s32 stepA, stepB;
    s32 i;
    volatile u16 *dma;

    *(s32 *)&layer->unk08[20] = 0;
    frame = *(u8 *)&gFrameCount;
    i = 0;
    /* FAKEMATCH(barrier): asm("" : "+r"(i)) keeps the loop's top test; as i = 0
     * alone, 48 of 110 instructions differ in tla-en. */
    asm("" : "+r"(i));
    z = *(s16 *)((u8 *)layer + 6);
    x = *(s16 *)((u8 *)layer + 2);
    // Each step held in a local for its whole life: as z * 0x3300 and
    // z * 0x1320 the starting phases are built differently.
    stepA = 0x3300;
    stepB = 0x1320;
    a = z * stepA + (frame << 10);
    b = z * stepB - (frame << 10);
    for (; i != 160; i++) {
      /* FAKEMATCH(pin): s1 held in r8; as a plain local, 57 of 110 instructions
       * differ in tla-en. */
      register s32 s1 asm("r8") = sine(a);
      s32 s2 = sine(b);
      *line++ = ((s1 * 3 + s2 * 7) >> 19) + x;
      a += 0x3300;
      b += 0x1320;
    }
    line = Data_02006340;
    REG_BG2HOFS = line[0];
    dma = REG_DMA0;
    dma[5] &= 0xc5ff;
    dma[5] &= 0x7fff;
    (void)dma[5];
    Dma_Set(line + 1, (void *)&REG_BG2HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
            (volatile u32 *)dma);
  }
}

extern const u16 gData_020054ee[];
extern const u16 gData_020054b8[];

// Build BG palette 1 colors 8-15 from the table.
void Func_02000390(void) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;
  const u8 *table = (const u8 *)gData_020054ee;
  u32 i = 0;
  u16 *dst = &buffers->palette[24];
  s32 blue = 4;
  s32 red = 0;
  volatile u16 *color;
  s32 green;

loop:
  color = BG_PLTT + 24 + i;
  green = blue - 2;
  *color = *(const u16 *)(table + blue) << 10 |
           *(const u16 *)(table + green) << 5 | *(const u16 *)(table + red);
  *dst++ = *color;
  i++;
  blue += 6;
  red += 6;
  if (i < 8)
    goto loop;
}

// Build BG palette 6 colors 1-9 from the table.
void Func_020003d8(void) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;
  const u8 *table = (const u8 *)gData_020054b8;
  u32 i = 0;
  u16 *dst = &buffers->palette[97];
  s32 blue = 4;
  s32 red = 0;
  volatile u16 *color;
  s32 green;

loop:
  color = BG_PLTT + 97 + i;
  green = blue - 2;
  *color = *(const u16 *)(table + blue) << 10 |
           *(const u16 *)(table + green) << 5 | *(const u16 *)(table + red);
  *dst++ = *color;
  i++;
  blue += 6;
  red += 6;
  if (i < 9)
    goto loop;
}

s32 Func_02000420(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}
