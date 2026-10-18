// Draft, differ 130 of 143: shows picture `index` of resource 0x26 as a framed
// window on BG0: decodes its tiles into VRAM, fills the screen block with tile
// 0 of palette 15, lays out 5 rows of 26 tiles from tile 16, opens window 0
// round it, fades the palette in and widens the window over 24 frames. No ☀️
// twin found. Left: the native keeps the tile counter in r10 from the start
// (its 16 shared with the delay's argument), the 0xf000 fill word in r4 for the
// tile loop, and 0x06002000 in r6 from before the decode; it steps the tile as
// a s16 each pass (no giv) and zero-extends it before the or, where this draft
// strength-reduces it. The native passes the source buffer call no first
// argument (r0 still holds the fill word's address); this draft passes 0. A
// 0x06002000 pointer variable is propagated as a constant (no change). The
// window loop is register choice.
#include "types.h"
#include "dma.h"
#include "io_reg.h"

void *Resource_GetTableEntry(s32 resourceId);
void EventRuntime_WaitDelay(s32 frames);
void *Runtime_BumpAllocateAlternatePool(s32 size);
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
void MemFree(void *buffer);
void BattleFx_ApplyColorToSourceBuffer(s32 value, s32 mode);
void BattleFx_ApplyColorToTargetBuffer(s32 value, s32 mode);
void BattleFx_StartBufferInterpolation(s32 frames);
void WaitFrames(s32 count);

// Resource 0x26, a pooled constant in the native, as a linker symbol.
extern char gResource_WindowPictures[];

#define DMA3 ((volatile u32 *)0x040000d4)

void Func_080d1f20(s32 index) {
  s32 *table = Resource_GetTableEntry((s32)gResource_WindowPictures);
  s16 tile = 16;
  void *buffer;
  volatile u32 fill;
  u16 *map;
  u32 row;
  u32 column;
  s32 top;
  s32 left;
  u32 i;

  EventRuntime_WaitDelay(16);
  buffer = Runtime_BumpAllocateAlternatePool(0x1040);
  Resource_DecodeByteLzInRam((u8 *)table + table[index] / 4 * 4, buffer);
  Dma_Set(buffer, (void *)0x06000200, 0x84000410, DMA3);
  MemFree(buffer);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Dma_Fill32Cell(fill, 0xf000, (void *)0x06002000, 0x800);
  map = (u16 *)0x060020c0;
  for (row = 0; row < 5; row++) {
    map += 2;
    for (column = 0; column < 26; column++)
      *map++ = tile++ | 0xf000;
    map += 4;
  }
  REG_DISPCNT &= 0x1fff;
  REG_DISPCNT |= DISPCNT_WIN0_ON;
  REG_WININ = 0x3f;
  REG_WINOUT = 0x3e;
  REG_WIN0H = 0x7878;
  REG_WIN0V = 0x1848;
  BattleFx_ApplyColorToSourceBuffer(0, 0);
  BattleFx_ApplyColorToTargetBuffer(0x10008, 0);
  BattleFx_StartBufferInterpolation(24);
  REG_DISPCNT ^= DISPCNT_BG0_ON;
  left = 120;
  top = 120;
  for (i = 0; i < 24; i++) {
    top -= 5;
    left += 5;
    REG_WIN0H = (top << 8) | left;
    WaitFrames(1);
  }
}
