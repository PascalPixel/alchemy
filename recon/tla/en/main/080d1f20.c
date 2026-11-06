// Near miss, differ 48 of 143 (tla-en, 2026-11-06): loop.c strength-reduces
// the tile counter, which the native leaves alone (below).
// Draws heading picture `heading` of resource
// 0x26 (Ui_Headings in the directory; ResourceId_Headings is not defined
// yet) into a 26x5-cell window and opens the window from the centre. The
// native keeps the cell counter's 0x10000 inside the inner loop (and passes
// it on to BattleEffect_ApplyColorToSourceBuffer), where loop.c here hoists it;
// the row and column counters then take r5 and r1 the other way round, and
// 16 is built in r2, not r0. Tried: the column counter in the row's block,
// tile as s32 with (s16) casts, tile++ inside the store (differ 66 to 78).
// 2026-11-06: the native recomputes (tile << 16) + 0x10000 from the s16
// tile every column and zero-extends the old tile before the OR; here
// loop.c takes tile as a biv and reduces the (tile + 1) << 16 giv to its own
// register stepped by 0x10000. A u32 `value = (u16)tile` before the OR gives
// the native zero-extension but the giv is still reduced (54). Also tried:
// goto loops (61 to 63), tile = (s16)(tile + 1) on s32/u32 (59), counters
// as s32 or < bounds, tile = 16 after the table lookup (52), (void)&tile
// (62 to 67). The store keeps tile++ inside it (48, from 50).
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

void Func_080d1f20(s32 heading) {
  s16 tile = 16;
  s32 *headings = Resource_GetTableEntry((s32)&ResourceId_Headings);
  u8 *tiles;
  u16 *cell;
  u32 row, column;
  s32 left, right;
  u32 frame;

  Event_WaitTransitionFrames();
  tiles = Heap_AllocEwram(0x1040);
  cell = (u16 *)0x06002000;
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
  for (frame = 0; frame <= 23; frame++) {
    left -= 5;
    right += 5;
    REG_WIN0H = (left << 8) | right;
    wait(1);
  }
}
