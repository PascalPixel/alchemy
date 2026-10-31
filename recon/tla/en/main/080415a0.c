// Near miss, differ 4 of 144 (tla-en, 2026-10-31): scheduler order. In the
// search loop the native masks the next cursor and stores it before it
// zero-extends the slot (ands; lsls; strh; lsrs, where this gives lsls; ands;
// lsrs; strh). sched2 ranks the slot's lsls (priority 7) over the mask (6)
// and its lsrs (6) over the store (5); the native's order needs the store to
// rank higher, as the ROM's wait before a load that follows a store would
// make it, but -mthumb-slow-loads models that wait only for whole loads and
// the next ldrb is a zero-extended byte. The two shifts come from one
// zero-extension, so no barrier can fall between them. The plain C reaches 4
// with no device; a device search (each statement followed by an empty asm or
// a one-pass block, wrapped in one, or swapped with the next) found nothing
// closer (2026-10-26).
// The Broken Seal's Window_OverlayTilePattern; this game finds the free tile
// in a do-while (as a for loop the first pass is peeled).

#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "dma.h"
#include "io_reg.h"

extern char ResourceId_WindowTiles;
void *Resource_GetTableEntry(s32 id);

void Func_080415a0(u16 *entry, u16 *mirror, s32 pattern, u8 *remap) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u8 pixels[128];
  u8 *sheet = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
  u32 slot = *(u8 *)entry;
  u8 *src;
  u8 *dst;
  u32 n;
  u32 i;

  dst = pixels;
  src = (u8 *)(0x06000000 + 32 * slot);
  for (i = 0; i < 32; i++) {
    n = *src++;
    *dst++ = n & 15;
    *dst++ = n >> 4;
  }

  dst = pixels;
  src = sheet + pattern * 32;
  for (i = 0; i < 32; i++) {
    u32 value = *src++;
    u32 color = remap[value & 15];

    if (color != 0)
      *dst = color;
    dst++;
    color = remap[value >> 4];
    if (color != 0)
      *dst = color;
    dst++;
  }

  {
    u8 *p;

    dst = pixels;
    for (i = 0, p = dst; i < 32; i++) {
      u32 value = p[0] | p[1] << 4;

      p += 2;
      *dst++ = value;
    }
  }

  if ((s8)slot >= 0) {
    n = 0;
    do {
      u32 value = work->tileCursor;
      u32 following = (value + 1) % 128;

      slot = (u8)value;
      work->tileCursor = following;
      if (work->tileAttributes[slot] == 0)
        break;
      n++;
    } while (n < 128);
    work->tileAttributes[slot] = 1;
    slot |= 0x80;
    *entry = slot | 0xf000;
    *mirror = slot | 0xf000;
  }
  Dma_Set(pixels, (void *)(0x06000000 + slot * 32), 0x84000008, REG_DMA3);
}
