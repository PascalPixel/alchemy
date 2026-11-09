#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "heap_release.h"
#include "resource.h"
#include "system.h"
#include "event_work.h"

extern char ResourceId_Headings;

s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
void BattleEffect_ApplyColorToSourceBuffer(s32 value, s32 mode);
void BattleEffect_ApplyColorToTargetBuffer(s32 color, s32 amount);
void BattleEffect_StartBufferInterpolation(s32 frames);

/// Draws heading picture `heading` of the headings resource into a 26 by 5 cell
/// window and opens the window from the centre, 5 pixels a side each frame
/// for 24 frames.
void Func_080d1f20(s32 heading) {
  u16 tile;
  s32 *headings = Resource_GetTableEntry((s32)&ResourceId_Headings);
  u8 *tiles;
  u16 *cell;
  u32 row, column;
  s32 left, right;

  tile = 16;
  Event_WaitTransitionFrames();
  cell = (u16 *)0x06002000;
  tiles = Heap_AllocEwram(0x1040);
  headings += headings[heading] / 4;
  Resource_DecodeByteLzInRam(headings, tiles);
  Dma_Set(tiles, (void *)0x06000200, 0x84000410, (volatile u32 *)0x040000d4);
  Heap_Release(tiles);
  REG_DISPCNT &= 0xfeff;
  Dma_Fill32(0xf000, cell, 0x800);
  cell = (u16 *)0x060020c0;
  for (row = 0; row <= 4; row++) {
    cell += 2;
    for (column = 0; column <= 25; column++) {
      *cell++ = (u16)(tile++) | 0xf000;
    }
    cell += 4;
  }
  REG_DISPCNT &= 0x1fff;
  REG_DISPCNT |= 0x2000;
  REG_WININ = 0x3f;
  REG_WINOUT = 0x3e;
  REG_WIN0H = 0x7878;
  REG_WIN0V = 0x1848;
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10008, 0);
  BattleEffect_StartBufferInterpolation(24);
  REG_DISPCNT ^= 0x100;
  left = 120;
  right = 120;
  // column is reused for the window's frames; a local of its own differs.
  for (column = 0; column <= 23; column++) {
    left -= 5;
    right += 5;
    REG_WIN0H = (left << 8) | right;
    wait(1);
  }
}
