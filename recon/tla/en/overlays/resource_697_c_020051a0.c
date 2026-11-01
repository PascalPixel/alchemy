// Draft, differ 510 of 497, skeleton gap 190 (tla-en, 2026-11-01).
// What is left is GCSE and the allocation it causes. The native computes every
// field address and value its branches need above the branch (hand-hoisted
// here into block-scoped pointers, which took the gap from 245) and keeps the
// row and pair loops' invariants inside the loops. PRE at -O2 still hoists
// work + 218 to 222 above the row loop and tile + 8 and x << 16 above the pair
// loop, and no copy or local stops it (CSE folds them back). Those pseudos then
// take the high registers, so work sits on the stack (native r11) and entry in
// r9 (native sp 32); that is most of the remaining ldr/mov skeleton lines.
// work wants about six more references to outrank entry (global.c priority).
// base stays apart from work so the markers do not reuse the row loop's hoisted
// addresses; scrollYMarker via base gains differ (499) but not gap (191).
// The pair loop is a goto with an explicit pair pointer: a for loop lets loop.c
// hoist 0 and 0xe400 as well (gap 203), and the native's two parallel pointers
// are what loop.c leaves. which sets Func_02002ce8's argument in each arm so
// that no call sits in an arm (sched2 sinks x past it); cprop still moves the
// constant to the join, where the native keeps it in the arms. 576 is rebuilt
// for VramCache_Load in the native; CSE here keeps it in a register.
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
  struct TrialRoadLinkWork *base = Ram_HeapSlots->unkec[0];
  u32 *cursor = base->oam;
  struct TrialRoadSprite *entry = (struct TrialRoadSprite *)base->oam;
  struct TrialRoadLinkWork *work = base;
  s32 tile;
  u8 *event;
  struct Actor *object;
  u32 flip;
  s32 x;
  s32 y;
  s32 lift;
  u32 row;
  u32 i;
  void *buffer;
  s16 *scrollY;
  s32 which;

  {
    s16 *vramEntry = &work->vramEntry;
    s16 *fade;
    tile = gResourceTableEntries[*vramEntry].offset >> 5;
    fade = &work->fade;
    event = Ram_HeapSlots->eventWork;
    if (work->unke0 != 0) {
      work->fade = 2;
    } else if (GameFlag_Test(262) || *(s16 *)(event + 358) != 0) {
      if (*fade > 0)
        --*fade;
    } else if (*fade <= 1) {
      if (++*fade == 1) {
        Dma_Set(Data_020062b4, (void *)0x050003c0, 0x80000010, REG_DMA3);
        buffer = Heap_AllocEwram(576);
        Func_02005770(gData_020062d4, buffer);
        VramCache_Load(*vramEntry, 576, buffer);
        Heap_Release(buffer);
      }
    }
  }
  if (((struct TrialRoadLinkWork *)cursor)->fade == 0) {
    VramCache_ReleaseVram(((struct TrialRoadLinkWork *)cursor)->vramEntry);
    return;
  }
  for (row = 0; row < 2; row++) {
    s16 *scrollX;
    s16 *fade;
    scrollY = &work->scrollY;
    scrollX = &base->scrollX;
    fade = &base->fade;
    y = (*scrollY + 8) & 255;
    if (row == 0) {
      x = *fade * 6 + *scrollX - 12;
      x &= 511;
      flip = 0;
    } else {
      x = 236 - *fade * 6 - *scrollX;
      x &= 511;
      flip = 0x10000000;
    }
    i = 0;
    if (y < 160) {
      u32 *pair = cursor;
    nextPair:
      {
        struct TrialRoadSprite *second;
        pair[0] = 0;
        pair[1] = y | (x << 16) | flip | 0x80008000;
        pair[2] = tile | 0xe400;
        second = entry + 1;
        Func_02005790(entry, 255);
        pair[3] = 0;
        pair[4] = (y + 32) | (x << 16) | flip | 0x4000;
        pair[5] = (tile + 8) | 0xe400;
        pair += 6;
        cursor += 6;
        entry += 2;
        Func_02005790(second, 255);
      }
      y += 36;
      i++;
      if (i < 4 && y < 160)
        goto nextPair;
    }
  }
  if ((gFrameCount & 15) < 5)
    return;
  object = ObjectTable_Get(work->a);
  if (object != NULL) {
    u32 side = gMainRoutineWork.entrance & 1;
    u32 *next = cursor + 1;
    struct TrialRoadSprite *nextEntry = entry + 1;
    s32 sprite = tile + 10;
    s16 *fade = &work->fade;
    s16 *scrollX = &work->scrollX;
    s32 *originX = &work->originX;
    s32 *originZ = &work->originZ;
    s32 objectX = object->x;
    if (side) {
      x = (objectX - *originX) / PIXELS(16) + *fade * 6 + *scrollX - 19;
      which = 0;
    } else {
      x = (objectX - *originX) / PIXELS(16) - *fade * 6 - *scrollX + 232;
      which = 0;
    }
    lift = Func_02002ce8(which);
    y = (object->z - *originZ) / PIXELS(16) + *scrollY - lift * 36 + 4;
    *cursor = 0;
    cursor = next;
    *cursor++ = ((x & 511) << 16) | (y & 255) | 0x40000000;
    *cursor++ = sprite | 0xe400;
    Func_02005790(entry, 255);
    entry = nextEntry;
  }
  object = ObjectTable_Get(work->b);
  if (object == NULL || object->x == 0)
    return;
  // XXX: this side's offset adds scrollX where the first marker's subtracts
  // it; the native does so, and a render of the course's far side would show
  // whether it is a slip.
  {
    u32 side = gMainRoutineWork.entrance & 1;
    u32 *next = cursor + 1;
    s32 sprite = tile + 14;
    s16 *scrollYMarker = &work->scrollY;
    s16 *fade = &work->fade;
    s16 *scrollX = &work->scrollX;
    s32 *originX = &work->originX;
    s32 *originZ = &work->originZ;
    s32 objectX = object->x;
    if (side) {
      x = (objectX - *originX) / PIXELS(16) + *fade * 6 + *scrollX + 189;
      which = 1;
    } else {
      x = (objectX - *originX) / PIXELS(16) - *fade * 6 + *scrollX + 23;
      which = 1;
    }
    lift = Func_02002ce8(which);
    y = (object->z - *originZ) / PIXELS(16) + *scrollYMarker - lift * 36 + 4;
    *cursor = 0;
    cursor = next;
    *cursor++ = ((x & 511) << 16) | (y & 255) | 0x40000000;
    *cursor = sprite | 0xe400;
    Func_02005790(entry, 255);
  }
}
