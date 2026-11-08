/// @file
/// The Sea of Time's frame callback: rotates two runs of the background palette, pulses
/// the red of colour 15 (or steps it with the alarm), copies the palette and ripples the
/// layer's horizontal scroll line by line.
#include "../../../../../../games/tla/src/scene/sea_of_time/sea_of_time.h"
#include "map.h"
#include "io_reg.h"

/// The event work's fields this function reads that SeaOfTimeEventWork lacks.
struct EventWork_02001650 {
  u8 unk000[0x166];
  s16 unk166;
  u8 unk168[0x4a];
  u16 unk1b2;
};

extern u16 Data_02002580[160];
extern u16 gData_020026c0[14];
extern s16 gData_0200257c[1];

void Func_0200201c(s32 colors, s32 mode);

void Func_02001650(void) {
  u16 *line = Data_02002580;
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[2];
  u8 *block = Ram_HeapSlots->block32;
  struct EventWork_02001650 *event = Ram_HeapSlots->eventWork;
  s32 i;
  s16 color;

  if (event->unk166 != 0)
    return;
  if (*(s8 *)(block + SPLIT_CONSTANT(0x2a00, 1)) != 0)
    return;
  if (event->unk1b2 == 0)
    return;

  if (gFrameCount - gData_020022c8 > gData_020022cc) {
    s16 target;

    color = BG_PLTT[62];
    target = gData_020026c0[13];

    for (i = 14; i != 0; i--) {
      BG_PLTT[48 + i] = BG_PLTT[47 + i];
      gData_020026c0[i - 1] = gData_020026c0[i - 2];
    }
    BG_PLTT[49] = color;
    gData_020026c0[0] = target;
    gData_020022c8 = gFrameCount;
  }

  if ((gFrameCount & 1) == 0) {
    color = BG_PLTT[78];

    for (i = 14; i != 0; i--)
      BG_PLTT[64 + i] = BG_PLTT[63 + i];
    BG_PLTT[65] = color;

    if ((gFrameCount & 3) == 0) {
      u16 color = BG_PLTT[15];
      s32 red = color & 31;
      s32 green = (color >> 5) & 31;
      s32 blue = (color >> 10) & 31;
      s32 flag = GameFlag_Test(0x200);
      /* FAKEMATCH(temporary): each channel shifted up after the flag test; as
       * (u16)green << 5 and so on in the store, 167 of 272 instructions differ in
       * tla-en. */
      u32 greenBits = green << 16;
      u32 redBits = red << 16;
      u32 blueBits = blue << 16;

      if (flag == 0) {
        u32 step = (u16)gData_0200257c[0];
        u32 sum = (u16)(redBits >> 16) + step;
        red = (s16)sum;
        if ((u16)sum < 18) {
          red = 18;
          gData_0200257c[0] = 1;
        } else if ((u16)sum > 31) {
          red = 31;
          gData_0200257c[0] = -1;
        }
      } else {
        if (((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)->alarm ==
            200) {
          u16 current = redBits >> 16;
          if (current != 0)
            red = (s16)(u16)(current - 1);
        } else if (((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)
                       ->alarm == 201) {
          u16 current = redBits >> 16;
          if (current < 30)
            red = current + 1;
        }
      }
      {
        u16 last = red;
        BG_PLTT[15] = (blueBits >> 6) | (greenBits >> 11) | (last & 31);
      }
    }
  }

  Dma_Set((const void *)BG_PLTT, Ram_HeapSlots->block32, DMA_COPY_WORDS(112),
          REG_DMA3);
  Func_0200201c(0x10000, 0);
  *(s32 *)((u8 *)layer + 0x1c) = 0;
  for (i = 0; i != 160; i++)
    *line++ =
        (s16)(layer->x >> 16) +
        (Math_Sine((i + *(u8 *)&gFrameCount + (s16)(layer->y >> 16)) << 9) >> 15);
  line = Data_02002580;
  REG_BG1HOFS = *line;
  Dma_Set(line + 1, (void *)&REG_BG1HOFS, 0xa2600001, (volatile u32 *)REG_DMA0);
}
