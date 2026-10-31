// Near miss, differ 549 of 497 (tla-en, 2026-10-31): what remains is GCSE.
// The native computes every field address and value its branches all need
// (the fade field before the first test; in each marker the pointer steps,
// the tile, five field addresses and object->x before the side test) above
// the branch, and keeps the row loop's field addresses inside the loop; this
// compiler's PRE (-O2) hoists the row loop's addresses before the loop and
// computes the rest in each arm, which moves the work pointer from r11 to the
// stack and every allocation after it. Compiled locally with -Os (classic
// GCSE plus code hoisting) the same draft hoists as the native does (a line
// diff of the two disassemblies falls from 501 to 373; the rest allocation).
// The native also sets Func_02002ce8's argument in each arm of a marker's
// side test, as if the marker were written once per arm and cross-jumped.
//
// The Trial Road link display's scheduled callback (Func_020055c4 starts it):
// its fade state falls during events or with flag 262 and otherwise rises to
// 2 (unke0 holds it at 2); rising from 0 to 1 it loads the palette and picture
// again, and at 0 it frees its VRAM. Otherwise it queues two rows of sprites,
// each up to four pairs 36 pixels apart down from the scrolled offset, sliding
// in 6 pixels per fade step (the right row mirrored), and on frames 5 to 15 of
// every 16 a marker over each of the two watched objects, projected at 1/16
// scale from their positions and the course side.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"
#include "frame.h"
#include "dma.h"
#include "heap_release.h"
#include "resource.h"
#include "vram_tab.h"

struct TrialRoadLinkWork {
  u32 oam[54];
  s16 vramEntry;
  s16 fade;
  s16 scrollX;
  s16 scrollY;
  s16 unke0;
  s16 a;
  s16 b;
  u8 unke6[6];
  s32 originX;
  s32 originZ;
};

/// One queued sprite: the three words Func_02005790 adds to OAM.
struct TrialRoadSprite {
  u32 words[3];
};

extern const u16 Data_020062b4[];
extern const u8 gData_020062d4[];

void Func_02005770(const void *source, void *destination);
void Func_02005790(void *entry, s32 priority);
s32 Func_02002ce8(s32 side);

void Func_020051a0(void) {
  struct TrialRoadLinkWork *work = Ram_HeapSlots->unkec[0];
  u32 *cursor = work->oam;
  struct TrialRoadSprite *entry = (struct TrialRoadSprite *)work->oam;
  s32 tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *object;
  u32 flip;
  s32 x;
  s32 y;
  s32 lift;
  u32 row;
  u32 i;
  void *buffer;

  if (work->unke0 != 0) {
    work->fade = 2;
  } else if (GameFlag_Test(262) || *(s16 *)(event + 358) != 0) {
    if (work->fade > 0)
      work->fade--;
  } else if (work->fade <= 1) {
    if (++work->fade == 1) {
      Dma_Set(Data_020062b4, (void *)0x050003c0, 0x80000010, REG_DMA3);
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
  for (row = 0; row < 2; row++) {
    y = (work->scrollY + 8) & 255;
    if (row == 0) {
      x = (work->fade * 6 + work->scrollX - 12) & 511;
      flip = 0;
    } else {
      x = (236 - work->fade * 6 - work->scrollX) & 511;
      flip = 0x10000000;
    }
    for (i = 0; i < 4 && y < 160; i++, y += 36) {
      *cursor++ = 0;
      *cursor++ = y | (x << 16) | flip | 0x80008000;
      *cursor++ = tile | 0xe400;
      Func_02005790(entry++, 255);
      *cursor++ = 0;
      *cursor++ = (y + 32) | (x << 16) | flip | 0x4000;
      *cursor++ = (tile + 8) | 0xe400;
      Func_02005790(entry++, 255);
    }
  }
  if ((gFrameCount & 15) < 5)
    return;
  object = ObjectTable_Get(work->a);
  if (object != NULL) {
    if (gMainRoutineWork.entrance & 1)
      x = (object->x - work->originX) / PIXELS(16) + work->fade * 6 +
          work->scrollX - 19;
    else
      x = (object->x - work->originX) / PIXELS(16) - work->fade * 6 -
          work->scrollX + 232;
    lift = Func_02002ce8(0);
    y = (object->z - work->originZ) / PIXELS(16) + work->scrollY - lift * 36 +
        4;
    *cursor++ = 0;
    *cursor++ = (y & 255) | ((x & 511) << 16) | 0x40000000;
    *cursor++ = (tile + 10) | 0xe400;
    Func_02005790(entry++, 255);
  }
  object = ObjectTable_Get(work->b);
  if (object == NULL || object->x == 0)
    return;
  // XXX: this side's offset adds scrollX where the first marker's subtracts
  // it; the native does so, and a render of the course's far side would show
  // whether it is a slip.
  if (gMainRoutineWork.entrance & 1)
    x = (object->x - work->originX) / PIXELS(16) + work->fade * 6 +
        work->scrollX + 189;
  else
    x = (object->x - work->originX) / PIXELS(16) - work->fade * 6 +
        work->scrollX + 23;
  lift = Func_02002ce8(1);
  y = (object->z - work->originZ) / PIXELS(16) + work->scrollY - lift * 36 + 4;
  *cursor++ = 0;
  *cursor++ = (y & 255) | ((x & 511) << 16) | 0x40000000;
  *cursor++ = (tile + 14) | 0xe400;
  Func_02005790(entry, 255);
}
