// Draft, differ 206 of 325: the intro's lighthouse scene: loads the
// gull sprites (resource 0x1a) and the lighthouse picture (0x19) with its
// palette, numbers the first 20 rows of BG1's map, fades in, then scrolls the
// picture down in three slowing steps through Func_081a68b4, starts the logo
// animation (Func_081a6b50 as a task, its state in heap slot
// 43), scrolls back up, fades to white and waits two seconds. A key fades to
// black and stops early. Clears OAM, removes the callback and frees the
// state before it returns 1. Same calls and control flow; left: register
// allocation (the native keeps the fill word's address in r11 and the
// picture's address in r6) and the map loop's counters.
#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "bg_scroll.h"
#include "palbuf.h"
#include "ram_buffer.h"

struct TitleScroll {
  s16 pos;
  s16 limit;
  s16 step;
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
  s16 frame;
  s16 timer;
  s16 step;
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
void WaitFrames(s32 frames);
s32 Title_WaitFramesOrPress(u32 frames);
s32 Func_081a68b4(struct TitleScroll *s);
void Func_081a6b50(void);
void Task_Add(void (*func)(void), s32 priority);
void Task_Remove(void (*func)(void));

s32 Func_081a6ea0(void) {
  struct TitleScroll scroll;
  volatile u32 fill;
  struct TitleLogoState *logo;
  u8 *source;
  u16 *map;
  s16 tile;
  s32 row;
  s32 column;
  s32 i;

  logo = HeapSlot_Alloc(172, 112);
  REG_DISPCNT = 0;
  Dma_Fill32Cell(fill, 0, (void *)0x07000000, 0x400);
  source = Resource_GetTableEntry((s32)&ResourceId_IntroGullSprites);
  Resource_DecodeByteLzInRam(source + 32, gMapCellBuffer);
  Dma_Set(gMapCellBuffer, (void *)0x06010000,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x1b00, REG_DMA3);
  WaitFrames(1);
  source = Resource_GetTableEntry((s32)&ResourceId_IntroLighthousePicture);
  Dma_Set(source, (void *)0x05000000, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
          REG_DMA3);
  *(volatile u16 *)0x05000000 = 0x7fff;
  WaitFrames(1);
  source += 0x200;
  Resource_DecodeByteLzInRam(source, gMapCellBuffer);
  Dma_Set(gMapCellBuffer, (void *)0x06000000,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x2580, REG_DMA3);
  WaitFrames(1);

  map = (u16 *)0x0600f800;
  tile = 0;
  for (row = 0; row < 20; row++) {
    for (column = 0; column < 30; column++) {
      *map++ = tile;
      tile++;
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
  Dma_Set(Ram_HeapSlots->block32, (void *)0x05000000,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x80, REG_DMA3);
  WaitFrames(1);
  PaletteFade_SetTarget(0x10000, 0);
  REG_DISPCNT = 0x1240;
  PaletteFade_Start(10);

  scroll.limit = 176;
  scroll.src = Data_02019600;
  scroll.dst = (u8 *)0x06009600;
  scroll.pos = 0;
  scroll.tile = tile;
  scroll.scroll = 0;
  logo->white = 10;
  scroll.rows = row;
  scroll.step = 4;
  scroll.cursor = map;
  logo->back = 25;
  scroll.frames = 1;
  if (Func_081a68b4(&scroll))
    goto stopped;
  scroll.limit = 200;
  scroll.step = 3;
  if (Func_081a68b4(&scroll))
    goto stopped;
  scroll.limit = 216;
  scroll.step = 2;
  if (Func_081a68b4(&scroll))
    goto stopped;
  Dma_Set(Resource_GetTableEntry((s32)&ResourceId_IntroGullSprites),
          (void *)0x05000200, (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  logo->frame = -5;
  logo->phase = 0;
  logo->timer = 0;
  logo->step = -3;
  logo->delay = 0;
  scroll.step = 1;
  scroll.limit = 224;
  if (Func_081a68b4(&scroll))
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
  if (Func_081a68b4(&scroll))
    goto stopped;
  PaletteFade_SetTarget(0x7fff, 0);
  PaletteFade_Start(20);
  scroll.limit = 0;
  scroll.step = -5;
  scroll.frames = 1;
  if (!Func_081a68b4(&scroll))
    Title_WaitFramesOrPress(120);
  goto done;
stopped:
  PaletteFade_SetTarget(0, 0);
  PaletteFade_Start(10);
  WaitFrames(10);
done:
  Dma_Fill32Cell(fill, 0, (void *)0x07000000, 0x400);
  Task_Remove(Func_081a6b50);
  HeapSlot_Release(172);
  WaitFrames(1);
  return 1;
}
