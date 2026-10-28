// Draft, differ 606 of 497: the Trial Road link display's scheduled callback
// (Func_020055c4 starts it): its fade state rises while no event runs (at 2
// it loads the palette and picture again) and falls during events or with flag
// 262; at 0 it frees its VRAM. Otherwise it queues the two banner rows of
// sprites, each four pairs 36 pixels apart along the scrolled offset (one row
// mirrored), and on frames 5 to 15 of every 16 a marker over each of the two
// watched objects, projected from their positions and the course side.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"
#include "frame.h"
#include "dma.h"

struct ResourceTableEntry {
  u16 unk0;
  u16 offset;
};

struct TrialRoadLinkWork {
  u32 oam[54];
  s16 vramEntry;
  s16 fade;
  s16 scrollX;
  s16 scrollY;
  s16 hidden;
  u16 a;
  u16 b;
  u8 unke6[6];
  s32 originX;
  s32 originZ;
};

extern struct ResourceTableEntry gResourceTableEntries[];
extern const u16 Data_020062b4[];
extern const u8 gData_020062d4[];

void *Heap_AllocEwram(s32 size);
void Func_02005770(const void *source, void *destination);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void VramCache_ReleaseVram(s32 entry);
void Heap_Release(void *block);
void Func_02005790(u32 *entry, s32 value);
struct ObjectRuntime *ObjectTable_Get(u32 id);
s32 Func_02002ce8(s32 value);

static __inline__ void Marker(u32 **cursor, struct TrialRoadLinkWork *work,
                              struct ObjectRuntime *object, s32 which,
                              s32 tile) {
  s32 dx = object->x - work->originX;
  s32 dz;
  s32 x;
  s32 y;
  s32 lift;

  if (dx < 0)
    dx += 0xfffff;
  if (gAdventure.entrance & 1)
    x = (dx >> 20) + work->scrollX * 6 + work->scrollY +
        (which ? 189 : -19);
  else
    x = (dx >> 20) - work->scrollX * 6 - work->scrollY + (which ? 23 : 232);
  lift = Func_02002ce8(which);
  dz = object->z - work->originZ;
  if (dz < 0)
    dz += 0xfffff;
  y = (dz >> 20) + work->scrollY - lift * 36 + 4;
  (*cursor)[0] = 0;
  (*cursor)[1] = (y & 255) | ((x & 511) << 16) | 0x40000000;
  (*cursor)[2] = (tile + (which ? 14 : 10)) | 0xe400;
  Func_02005790(*cursor, 255);
  *cursor += 3;
}

void Func_020051a0(void) {
  struct TrialRoadLinkWork *work =
      *(struct TrialRoadLinkWork **)((u8 *)Ram_HeapSlots + 236);
  u8 *event = Ram_HeapSlots->eventWork;
  s32 tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  u32 *cursor = work->oam;
  struct ObjectRuntime *object;
  u32 flip;
  s32 x;
  s32 y;
  s32 row;
  s32 i;
  void *buffer;

  if (work->hidden != 0) {
    work->fade = 2;
  } else if (GameFlag_Test(262) || *(s16 *)(event + 358) != 0) {
    if (work->fade > 0)
      work->fade--;
  } else if (work->fade <= 1) {
    if (++work->fade == 2) {
      Dma_Set(Data_020062b4, (void *)0x050003c0, 0x84000010, REG_DMA3);
      buffer = Heap_AllocEwram(576);
      Func_02005770(gData_020062d4, buffer);
      VramCache_Load(work->vramEntry, 576, buffer);
      Heap_Release(buffer);
    }
  }
  if (work->fade == 0) {
    VramCache_ReleaseVram(work->vramEntry);
    return;
  }
  for (row = 0; row <= 1; row++) {
    y = (work->scrollY + 8) & 255;
    if (row == 0) {
      x = (work->scrollX * 6 + work->hidden - 12) & 511;
      flip = 0;
    } else {
      x = (236 - work->scrollX * 6 - work->hidden) & 511;
      flip = 0x10000000;
    }
    for (i = 0; y <= 159 && i <= 3; i++, y += 36) {
      cursor[0] = 0;
      cursor[1] = y | (x << 16) | flip | 0x80008000;
      cursor[2] = tile | 0xe400;
      Func_02005790(cursor, 255);
      cursor[3] = 0;
      cursor[4] = (y + 32) | (x << 16) | flip | 0x4000;
      cursor[5] = (tile + 8) | 0xe400;
      Func_02005790(cursor + 3, 255);
      cursor += 6;
    }
  }
  if ((gFrameCount & 15) <= 4)
    return;
  object = ObjectTable_Get(work->a);
  if (object != 0)
    Marker(&cursor, work, object, 0, tile);
  object = ObjectTable_Get(work->b);
  if (object == 0 || object->x == 0)
    return;
  Marker(&cursor, work, object, 1, tile);
}
