#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "bg_scroll.h"
#include "palette_buffers.h"
#include "ram_buffer.h"

struct TitleScroll {
  s16 pos;
  s16 limit;
  u16 step;
  u8 *src;
  u8 *dst;
  u16 *cursor;
  u16 tile;
  s16 scroll;
  u16 rows;
  u32 frames;
};

struct TitleLogoState {
  s16 phase;
  u16 frame;
  s16 timer;
  u16 step;
  s16 delay;
  s16 white;
  s16 back;
};

extern u8 gMapCellBuffer[];
extern u8 Data_02019600[];
extern u8 Data_02017080[];
extern char ResourceId_IntroLighthousePicture;
extern char ResourceId_IntroGullSprites;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
void wait(s32 frames);
s32 Title_WaitOrPress(u32 frames);
s32 Func_081a68b4(struct TitleScroll *s);
void Func_081a6b50(void);
s32 Task_Add(void (*func)(void), s32 priority);
void Task_Remove(void (*func)(void));

s32 Func_081a6ea0(void) {
  struct TitleScroll scroll;
  volatile u32 fill;
  struct TitleLogoState *logo;
  u8 *source;
  u16 *map;
  s16 tile;
  s32 q16One;
  s32 number;
  s32 row;
  s32 i;
  u8 *buffer;

  logo = HeapSlot_Alloc(172, 112);
  REG_DISPCNT = 0;
  Dma_Fill32Cell(fill, 0, (void *)0x07000000, 0x400);
  source = Resource_GetTableEntry((s32)&ResourceId_IntroGullSprites);
  buffer = gMapCellBuffer;
  source += 32;
  Resource_DecodeByteLzInRam(source, buffer);
  Dma_Set(buffer, (void *)0x06010000, (DMA_ENABLE | DMA_32BIT) << 16 | 0x1b00,
          REG_DMA3);
  wait(1);
  source = Resource_GetTableEntry((s32)&ResourceId_IntroLighthousePicture);
  Dma_Set(source, (void *)0x05000000, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
          REG_DMA3);
  *(u16 *)0x05000000 = 0x7fff;
  wait(1);
  source += 0x200;
  Resource_DecodeByteLzInRam(source, buffer);
  Dma_Set(buffer, (void *)0x06000000, (DMA_ENABLE | DMA_32BIT) << 16 | 0x2580,
          REG_DMA3);
  wait(1);

  map = (u16 *)0x0600f800;
  tile = 0;
  q16One = 0x10000;
  for (row = 0; row < 20; row++) {
    for (i = 29; i >= 0; i--) {
      /* FAKEMATCH(temporary): the old tile kept in number, the step spelt on
       * the high halfword; as *map = tile; tile++, 97 of 325 instructions
       * differ in tla-en. */
      number = tile;
      tile = ((tile << 16) + q16One) >> 16;
      *map = number;
      map++;
    }
    *map++ = 0x1ff;
    *map++ = 0x1ff;
  }
  REG_BG1CNT = 0x1f83;
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0;
  for (i = 0; i < 4; i++) {
    gBgScroll[i].y = 0;
    gBgScroll[i].x = 0;
  }
  Dma_Set((void *)0x05000000, Ram_HeapSlots->block32,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x80, REG_DMA3);
  wait(1);
  PaletteFade_SetTarget(q16One, 0);
  REG_DISPCNT = 0x1240;
  PaletteFade_Start(10);

  scroll.pos = 0;
  scroll.limit = 176;
  scroll.step = i;
  scroll.src = Data_02019600;
  scroll.dst = (u8 *)0x06009600;
  scroll.cursor = map;
  scroll.tile = tile;
  scroll.scroll = 0;
  scroll.rows = row;
  scroll.frames = 1;
  logo->white = 10;
  logo->back = 25;
  if (Func_081a68b4(&scroll))
    goto stopped;
  scroll.limit = 200;
  scroll.step = 3;
  if (Func_081a68b4(&scroll))
    goto stopped;
  scroll.limit = 216;
  scroll.step = 2;
  /* FAKEMATCH(temporary): map reused for the result; as a separate s32 local,
   * 52 of 325 instructions differ in tla-en. */
  map = (u16 *)Func_081a68b4(&scroll);
  if (map)
    goto stopped;
  Dma_Set(Resource_GetTableEntry((s32)&ResourceId_IntroGullSprites),
          (void *)0x05000200, (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  logo->phase = 0;
  logo->frame = -5;
  logo->timer = 0;
  logo->step = -3;
  logo->delay = 0;
  scroll.step = 1;
  scroll.limit = 224;
  i = Func_081a68b4(&scroll);
  if (i)
    goto stopped;
  Task_Add(Func_081a6b50, 0x480);
  scroll.limit = 160;
  scroll.step = -1;
  scroll.scroll = 0;
  scroll.src = Data_02017080;
  scroll.dst = (u8 *)0x06007800;
  scroll.cursor = (u16 *)0x0600f9c2;
  scroll.tile = 450;
  scroll.rows = 11;
  scroll.frames = 8;
  i = Func_081a68b4(&scroll);
  if (i)
    goto stopped;
  PaletteFade_SetTarget(SPLIT_CONSTANT(SPLIT_CONSTANT(0x7f00, 255)), 0);
  PaletteFade_Start(20);
  scroll.limit = 0;
  scroll.step = -5;
  scroll.frames = 1;
  if (!Func_081a68b4(&scroll))
    Title_WaitOrPress(120);
  goto done;
stopped:
  PaletteFade_SetTarget(0, 0);
  PaletteFade_Start(10);
  wait(10);
done:
  Dma_Fill32Cell(fill, 0, (void *)0x07000000, 0x400);
  Task_Remove(Func_081a6b50);
  HeapSlot_Release(172);
  wait(1);
  return 1;
}
