// Twin of The Broken Seal's Title_ShowCamelotLogo: the same resource, BG2
// set-up, DMA copies, 120-frame tile cycle and A or Start exit. Ported from its
// C: the `result` local (zero, stored to gBgScroll[2].y and returned), the one
// buffer local and `* 1024 + buffer`.

#include "resource.h"
#include "types.h"
#include "io_reg.h"
#include "task.h"
#include "system.h"
#include "dma.h"
#include "ram_buffer.h"
#include "input.h"
#include "frame.h"

struct BgScroll {
  u16 x, y;
};

extern struct BgScroll gBgScroll[];
extern char ResourceId_CamelotLogo;

void Fade_ToBlack(s32 vblanks);
void Fade_FromWhite(s32 vblanks);
void Fade_Wait(void);
void Func_08014b70(void);
void Window_LoadGraphics(void);

s32 Func_081a76f8(void) {
  s32 result;
  u32 i;
  u8 *buffer;
  s32 resource;

  gRenderOamEnabled = 1;
  result = 0;
  resource = (s32)&ResourceId_CamelotLogo;
  Task_ResetAll();
  Fade_ToBlack(1);
  Func_08014b70();
  wait(1);
  REG_BG2CNT = 0x685;
  REG_DISPCNT = 0x1440;
  gBgScroll[2].y = result;
  buffer = Resource_GetTableEntry(resource);
  Resource_DecodeType01(buffer, Ram_MapCellBuffer);
  buffer = Ram_MapCellBuffer;
  Dma_Set(buffer, (void *)BG_PLTT, (DMA_ENABLE | DMA_32BIT) << 16 | 0x1c0 / 4,
          REG_DMA3);
  buffer += 0x1c0;
  Dma_Set(buffer, (void *)0x06003000,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x800 / 4, REG_DMA3);
  buffer += 0x800;
  Dma_Set(buffer, BG_CHAR_BLOCK(1), (DMA_ENABLE | DMA_32BIT) << 16 | 0x4000 / 4,
          REG_DMA3);
  buffer += 0x4000;
  for (i = 0; i < 4; i++) {
    gBgScroll[i].y = 0;
    gBgScroll[i].x = 0;
  }
  Dma_Set(gBgScroll, REG_BG0HOFS, (DMA_ENABLE | DMA_32BIT) << 16 | 4, REG_DMA3);
  Window_LoadGraphics();
  Func_08014b70();
  Fade_FromWhite(1);
  Fade_Wait();
  REG_DISPCNT = 0x1540;
  for (i = 0; i < 120; i++) {
    Dma_Set(((gFrameCount >> 3) & 3) * 1024 + buffer, BG_CHAR_BLOCK(1) + 0x100,
            (DMA_ENABLE | DMA_32BIT) << 16 | 0x340 / 4, REG_DMA3);
    if (gInput.pressed & (KEY_A | KEY_START))
      break;
    wait(1);
  }
  return result;
}

/// Unused: an empty routine in the far-call table.
void Data_081a786c(void) {}
