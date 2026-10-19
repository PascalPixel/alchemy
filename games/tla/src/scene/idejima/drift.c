#include "types.h"
#include "field_event.h"
#include "ram_buffer.h"

struct MapLayer {
  u8 unk00[6];
  s16 y;
  u8 unk08[0x10];
  s32 unk18;
  s32 unk1c;
};

struct MapWork {
  u8 unk000[0x108];
  struct MapLayer layer;
};

struct FadePalette {
  u16 colors[256];
};

void Func_020032f0(s32, s32);

/// Copies colours 128 to 191 of heap block 32 over colours 0 to 63, clears
/// colour 0 and calls Func_020032f0(0x10000, 0).
void Func_0200161c(void) {
  /* FAKEMATCH: the palette pinned to r0, where ⚓️ keeps it and the counter in
   * r4; unpinned the two registers swap (differ 14). */
  register struct FadePalette *palette asm("r0") = Ram_HeapSlots->block32;
  u16 *to = palette->colors;
  u16 *from = palette->colors + 128;
  u32 i;

  for (i = 0; i < 64; i++)
    *to++ = *from++;
  palette->colors[0] = 0;
  Func_020032f0(0x10000, 0);
}

void DriftScene_Prepare(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layer;

  layer->unk18 = 0x4000;
  layer->unk1c = 0x2000;
  Engine_MapCopyCellsTo(72, 10, 75, 8, 1, 1);
  Engine_ActorSetAnimation(9, 2);
}
