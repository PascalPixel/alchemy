#include "types.h"
#include "dma.h"
#include "io_reg.h"

void BattleEffect_ApplyColorToTargetBuffer(s32 value, s32 mode);
void BattleEffect_StartBufferInterpolation(s32 frames);
void Tile_ClearLeftPixels(s32 x, s32 y, s32 width);
void WaitTicks(s32 count);
void *Resource_GetTableEntry(s32 resourceId);

extern s16 gData_080f3230[];
extern char ResourceId_WindowTiles;

#define DMA3 ((volatile u32 *)0x040000d4)

/// Closes Func_080d1f20's picture window: fades the palette out while wiping
/// the picture's 26 by 5 tiles from the left by the steps in gData_080f3230
/// (runs of widths, each run ended by a negative entry and the whole by -9),
/// then turns BG0 and the window off, clears the screen block, reloads the
/// window tiles and turns BG0 back on.
void Func_080d20fc(void) {
  s32 step = 0;
  u32 x;
  u32 y;
  volatile u32 fill;

  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(24);
  for (;;) {
    for (; gData_080f3230[step] >= 0; step++) {
      for (x = 0; x < 26; x++) {
        for (y = 0; y < 5; y++)
          Tile_ClearLeftPixels(x, y, gData_080f3230[step]);
      }
    }
    WaitTicks(2);
    if (gData_080f3230[step] == -9)
      break;
    step++;
  }
  REG_DISPCNT ^= DISPCNT_BG0_ON | DISPCNT_WIN0_ON;
  Dma_Fill32Cell(fill, 0xf000, (void *)0x06002000, 0x800);
  Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowTiles),
          (void *)0x06000000, 0x84000800, DMA3);
  REG_DISPCNT ^= DISPCNT_BG0_ON;
}
