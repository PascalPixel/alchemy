// Near miss, differ 233 of 272 (tla-en, 2026-10-31): the native's calls,
// branches, loops and accesses are all here; what is left is the compiler's.
// (1) line and layer swap r10 and r8. (2) In the first rotation loop this
// compiler reduces both arrays to decrementing pointers; the native keeps the
// palette addresses as i * 2 plus a constant rebuilt on every pass, and the
// targets as one base plus two decrementing offsets (24, 26). (3) The second
// rotation loop shows no loop pass in the native (constants rebuilt each pass,
// no reduction); as a goto loop it matches but for 0x0500007e, which CSE
// derives from 0x05000080, and the whole rises to 247 through allocation. (4)
// After GameFlag_Test the native shifts red, green and blue left by 16 once, as
// the loop pass hoists invariants; here the shifts stay at each use and combine
// folds green and blue. The plain palette pointer in the two loops saves a
// callee-saved register (volatile BG_PLTT: 270). gData_0200257c needs its size:
// as [] each store becomes load, IOR and store.
#include "../../../../games/tla/src/scene/sea_of_time/sea_of_time.h"
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
  struct EventWork_02001650 *event = Ram_HeapSlots->eventWork;
  u8 *block = Ram_HeapSlots->block32;
  s32 i;

  if (event->unk166 != 0)
    return;
  if (*(s8 *)(block + 0x2a01) != 0)
    return;
  if (event->unk1b2 == 0)
    return;

  if (gFrameCount - gData_020022c8 > gData_020022cc) {
    s16 color = BG_PLTT[62];
    s16 target = gData_020026c0[13];

    for (i = 14; i != 0; i--) {
      ((u16 *)BG_PLTT)[48 + i] = ((u16 *)BG_PLTT)[47 + i];
      gData_020026c0[i - 1] = gData_020026c0[i - 2];
    }
    BG_PLTT[49] = color;
    gData_020026c0[0] = target;
    gData_020022c8 = gFrameCount;
  }

  if ((gFrameCount & 1) == 0) {
    s16 color = BG_PLTT[78];

    for (i = 14; i != 0; i--)
      ((u16 *)BG_PLTT)[64 + i] = ((u16 *)BG_PLTT)[63 + i];
    BG_PLTT[65] = color;

    if ((gFrameCount & 3) == 0) {
      u16 color = BG_PLTT[15];
      u16 red = color & 31;
      u16 green = (color >> 5) & 31;
      u16 blue = (color >> 10) & 31;

      if (!GameFlag_Test(0x200)) {
        red += gData_0200257c[0];
        if (red < 18) {
          red = 18;
          gData_0200257c[0] = 1;
        } else if (red > 31) {
          red = 31;
          gData_0200257c[0] = -1;
        }
      } else if (((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)
                     ->alarm == 200) {
        if (red != 0)
          red--;
      } else if (((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)
                     ->alarm == 201) {
        if (red < 30)
          red++;
      }
      BG_PLTT[15] = (blue << 10) | (green << 5) | (red & 31);
    }
  }

  Dma_Set((const void *)BG_PLTT, Ram_HeapSlots->block32, DMA_COPY_WORDS(112),
          REG_DMA3);
  Func_0200201c(0x10000, 0);
  *(s32 *)((u8 *)layer + 0x1c) = 0;
  for (i = 0; i != 160; i++)
    *line++ =
        (layer->x >> 16) +
        (Math_Sine((i + *(u8 *)&gFrameCount + (s16)(layer->y >> 16)) << 9) >>
         15);
  line = Data_02002580;
  REG_BG1HOFS = *line;
  Dma_Set(line + 1, (void *)&REG_BG1HOFS, 0xa2600001, (volatile u32 *)REG_DMA0);
}
