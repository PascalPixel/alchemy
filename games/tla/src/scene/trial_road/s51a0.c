/// @file
/// The Trial Road link display's scheduled callback, which Func_020055c4
/// starts. Its fade state falls during events or while flag 262 is set and
/// otherwise rises to 2 (unke0 holds it at 2); rising from 0 to 1 it loads
/// the palette and picture again, and at 0 it frees its VRAM. Otherwise it
/// queues two rows of sprites, each up to four pairs 36 pixels apart down from
/// the scrolled offset, sliding in 6 pixels per fade step (the right row
/// mirrored), and on frames 5 to 15 of every 16 a marker over each of the two
/// watched actors, projected at 1/16 scale from their positions and the
/// course side.

#include "trial_road.h"
#include "frame.h"
#include "dma.h"
#include "heap_release.h"
#include "resource.h"
#include "vram_tab.h"
#include "event_work.h"

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

// The picture's buffer: 576 bytes of EWRAM, the size VramCache_Load copies.
static inline void *TrialRoadLink_AllocPicture(void) {
  return Heap_AllocEwram(576);
}

// left and right are the arms' offsets, each opening with its sign, so the
// subtractions compile in the order the native has them.
#define MARKER(left, right, which, tileAdd)                                    \
  do {                                                                         \
    s32 lift;                                                                  \
    /* FAKEMATCH(pin): originZ in r6; as a plain s32 *, 304 of 497             \
       instructions differ in tla-en. */                                       \
    register s32 *originZ asm("r6");                                           \
    /* FAKEMATCH(pin): x in r9; as a plain s32, 365 of 497 instructions        \
       differ in tla-en. */                                                    \
    register s32 x asm("r9");                                                  \
    /* FAKEMATCH(pin): arg in r0; as a plain s32, 44 of 497 instructions       \
       differ in tla-en. */                                                    \
    register s32 arg asm("r0");                                                \
    s32 side = gMainRoutineWork.entrance & 1;                                  \
    if (side) {                                                                \
      x = (actor->x - work->originX) / PIXELS(16) left;                        \
      arg = which;                                                             \
    } else {                                                                   \
      x = (actor->x - work->originX) / PIXELS(16) right;                       \
      arg = which;                                                             \
    }                                                                          \
    originZ = &work->originZ;                                                  \
    lift = Func_02002ce8(arg);                                                 \
    y = (actor->z - *originZ) / PIXELS(16) + work->scrollY - lift * 36 + 4;    \
    /* Once-only: ends the block, so y + 4 is scheduled before x & 511. */     \
    do {                                                                       \
    } while (0);                                                               \
    x &= 511;                                                                  \
    y &= 255;                                                                  \
    *cursor++ = 0;                                                             \
    y |= x << 16;                                                              \
    y |= markerSize;                                                           \
    *cursor++ = y;                                                             \
    *cursor++ = (tile + (tileAdd)) | 0xe400;                                   \
    Func_02005790(entry++, 255);                                               \
  } while (0)

void Func_020051a0(void) {
  struct TrialRoadLinkWork *work = Ram_HeapSlots->unkec[0];
  u32 *cursor = (u32 *)work;
  struct TrialRoadSprite *entry = (struct TrialRoadSprite *)work;
  s16 *vramEntry = &work->vramEntry;
  u32 row, i;
  s32 tile = gResourceTableEntries[*vramEntry].offset >> 5;
  u32 flip, word;
  s32 y;
  // One constant for its whole life: the markers' OAM size bits (attribute 1
  // size 1, 16 by 16), held in a register across both markers.
  u32 markerSize = 0x40000000;
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct Actor *actor;

  if (work->unke0 != 0) {
    work->fade = 2;
  } else if (GameFlag_Test(262) || event->unk166 != 0) {
    if (work->fade > 0)
      work->fade--;
  } else if (work->fade <= 1) {
    if (++work->fade == 1) {
      void *buffer;
      Dma_Set(Data_020062b4, (void *)0x050003c0, 0x80000010, REG_DMA3);
      buffer = TrialRoadLink_AllocPicture();
      Func_02005770(gData_020062d4, buffer);
      VramCache_Load(*vramEntry, 576, buffer);
      Heap_Release(buffer);
    }
  }
  if (work->fade == 0) {
    VramCache_ReleaseVram(work->vramEntry);
    return;
  }
  for (row = 0; row < 2; row++) {
    s32 x;
    y = (work->scrollY + 8) & 255;
    if (row == 0) {
      x = work->fade * 6 + work->scrollX - 12;
      x &= 511;
      flip = 0;
    } else {
      x = 236 - work->fade * 6 - work->scrollX;
      x &= 511;
      flip = 0x10000000;
    }
    for (i = 0; i < 4 && y < 160; i++) {
      *cursor++ = 0;
      *cursor++ = y | (x << 16) | flip | 0x80008000;
      *cursor++ = tile | 0xe400;
      Func_02005790(entry++, 255);
      word = y + 32;
      *cursor++ = 0;
      *cursor++ = word | (x << 16) | flip | 0x4000;
      *cursor++ = (tile + 8) | 0xe400;
      Func_02005790(entry++, 255);
      y += 36;
    }
  }
  if ((gFrameCount & 15) < 5)
    return;
  actor = ObjectTable_Get(work->a);
  if (actor != NULL) {
    MARKER(+work->fade * 6 + work->scrollX - 19,
           -work->fade * 6 - work->scrollX + 232, 0, 10);
  }
  actor = ObjectTable_Get(work->b);
  if (actor == NULL || actor->x == 0)
    return;
  {
    // XXX: this side's offset adds scrollX where the first marker's subtracts
    // it; the native does so, and a render of the course's far side would show
    // whether it is a slip.
    MARKER(+work->fade * 6 + work->scrollX + 189,
           -work->fade * 6 + work->scrollX + 23, 1, 14);
  }
}
