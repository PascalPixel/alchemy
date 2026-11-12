/// @file
/// Scene code of overlay 0x697, in ROM order, from s51a0's offset up to the
/// next object the overlay links, and the overlay's data.

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
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

/// The Trial Road link display's scheduled callback, which Func_020055c4
/// starts. Its fade state falls during events or while flag 262 is set and
/// otherwise rises to 2 (unke0 holds it at 2); rising from 0 to 1 it loads
/// the palette and picture again, and at 0 it frees its VRAM. Otherwise it
/// queues two rows of sprites, each up to four pairs 36 pixels apart down from
/// the scrolled offset, sliding in 6 pixels per fade step (the right row
/// mirrored), and on frames 5 to 15 of every 16 a marker over each of the two
/// watched actors, projected at 1/16 scale from their positions and the
/// course side.
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

struct TrialRoadLink {
  u8 unk00[0xd8];
  s16 vramEntry;
  u16 unkda;
  u8 unkdc[4];
  u16 unke0;
  u16 a;
  u16 b;
  u8 unke6[6];
  s32 height;
  s32 unkf0;
};

void Func_020055c4(s32 a, s32 b) {
  struct TrialRoadLink *link = HeapSlot_AllocEwram(236, 0x3934);
  void *buffer = Heap_AllocEwram(576);
  s32 zero;
  s32 owner;
  u16 *unkda;
  u16 *unke0;
  s32 *height;
  s32 *unkf0;
  s16 *vramEntry;

  link->a = a;
  link->b = b;
  owner = gMainRoutineWork.currentOwner;
  height = &link->height;
  unkf0 = &link->unkf0;
  unkda = &link->unkda;
  unke0 = &link->unke0;
  vramEntry = &link->vramEntry;
  if (a == owner) {
    *height = 0xa00000;
    *unkf0 = 0xf9a00000;
  } else {
    *height = 0x700000;
    *unkf0 = 0xf9a00000;
  }
  Actor_GetById(a);
  Actor_GetById(b);
  zero = 0;
  *unkda = zero;
  *unke0 = zero;
  Func_02005770(gData_020062d4, buffer);
  *vramEntry = Resource_FindFreeEntry();
  VramCache_Load(*vramEntry, 576, buffer);
  Task_Add(Func_020051a0, 0x476);
  Heap_Release(buffer);
}

struct TrialRoadControl {
  s16 state;
  s16 field02;
  s16 field04;
  s16 field06;
  s16 field08;
  s16 field0a;
};

extern u8 gSceneState[];

void Func_02004ddc(void);

void Func_02005688(s32 a) {
  u8 *work = Ram_HeapSlots->unkec[0];
  struct TrialRoadControl *control = (struct TrialRoadControl *)gSceneState;

  if (GameFlag_Test(0x109) == 0) {
    /* FAKEMATCH(temporary): a local reads the work value before the three
     * stores; as the value stored directly, 4 of 24 instructions differ in
     * tla-en. */
    s32 value = *(u16 *)(work + 228);

    control->field02 = a;
    control->field04 = value;
    control->field08 = 0;
  }
  Task_Add(Func_02004ddc, 0x485);
}

void Func_020056c4(s32 a) {
  struct TrialRoadControl *control = (struct TrialRoadControl *)gSceneState;
  /* FAKEMATCH(temporary): a word local builds 0 with movs for both halfword
   * stores; as a halfword constant, which loads from the pool, 14
   * of 15 instructions differ in tla-en. */
  s32 zero = 0;

  *(u16 *)((Ram_HeapSlots->unkec[0]) + 234) = zero;
  control->field0a = a;
  control->field06 = zero;
  control->state = 1;
  Func_02004ddc();
}

s32 Func_020056e8(void) {
  s32 state = *(s16 *)gSceneState;

  return IsZero(state ^ 9);
}

void Func_02005704(void) {}

#include "scene_tables.h"

void Func_02000058(void);
void Func_02000694(void);
void Func_02001904(void);
void Func_02001974(void);
void Func_02001a70(void);
void Func_02001afc(void);
void Func_02001bdc(void);
void Func_02001bec(void);
void Func_02001c6c(void);
void Func_02001d8c(void);

s32 gData_02005a70[] = {
    46,
    (s32)Func_02000058,
    17,
};

s32 gData_02005a7c[] = {
    46,
    (s32)Func_02000694,
    17,
};

u32 gData_02005a88[] = {
    0x14151213,
};

// clang-format off
u32 gData_02005a8c[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02005ac8[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02005b04[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17, 17,
};

u32 gData_02005b44[] = {
    23, 3, 0x1000, 0, 1, 13,
    0xffff, 0,
};

void (*gData_02005b64[])(void) = {
    Func_02001904,
    Func_02001974,
    Func_02001a70,
    Func_02001afc,
    Func_02001bdc,
    Func_02001bec,
    Func_02001c6c,
    Func_02001d8c,
};

u32 gData_02005b84[] = {
    450, 451, 452, 453, 454, 455,
    456, 457, 458, 459,
};

u16 gData_02005bac[] = {
    1, 48, 15, 48, 29, 48, 1, 70,
    15, 70, 29, 70, 1, 92, 15, 92,
};

u8 gData_02005bcc[] = {
    0, 1, 2, 3, 4, 4, 4, 5,
    6, 7,
};

u8 gData_02005bd6[] = {
    0, 1, 2, 3, 4, 4, 4, 5,
    6, 7,
};

u32 gData_02005be0[] = {
    0x7030200, 0x6050401,
};

u32 gData_02005be8[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    22, 5, 0x1380000, 22, 7, 0x2a80000,
    39, 5, 4, 0x1380000, 0, 0x2780000,
    1, 4, 0x1580000, 0, 0x2680000, 1,
    4, 0x1980000, 0, 0x2680000, 1, 4,
    0x1a80000, 0, 0x2580000, 1, 4, 0x1a80000,
    0, 0x1c80000, 1, 17,
};

u32 gData_02005c88[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    22, 5, 0xe80000, 22, 7, 0x2a80000,
    39, 5, 4, 0xe80000, 0, 0x2780000,
    1, 4, 0xc80000, 0, 0x2680000, 1,
    4, 0x880000, 0, 0x2680000, 1, 4,
    0x780000, 0, 0x2580000, 1, 4, 0x780000,
    0, 0x1c80000, 1, 17,
};

u32 gData_02005d28[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    22, 5, 0x780000, 22, 7, 0x1280000,
    39, 5, 4, 0x780000, 0, 0xd80000,
    1, 4, 0x880000, 0, 0xc80000, 1,
    4, 0xd80000, 0, 0xc80000, 1, 4,
    0xe80000, 0, 0xb80000, 1, 0, 2,
    22, 3, 0xc000, 17,
};

u32 gData_02005dc8[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    39, 5, 4, 0xd80000, 0, 0xa80000,
    1, 4, 0xe80000, 0, 0x980000, 1,
    4, 0xe80000, 0, 0x880000, 1, 22,
    5, 0, 22, 7, 0, 17,
};

u32 gData_02005e40[] = {
    22, 15, 0x10000, 22, 16, 0x8000,
    22, 5, 0x1580000, 22, 7, 0xb80000,
    39, 2, 4, 0x1380000, 0, 0xa80000,
    1, 39, 1, 0, 2, 22,
    3, 0x8000, 0, 2, 22, 3,
    0x6000, 0, 2, 22, 3, 0x4000,
    17,
};

u32 gData_02005ed4[] = {
    22, 15, 0x18000, 22, 16, 0x10000,
    39, 5, 4, 0x1780000, 0, 0xc80000,
    1, 4, 0x1d80000, 0, 0xc80000, 1,
    4, 0x1e80000, 0, 0xb80000, 1, 4,
    0x1e80000, 0, 0x880000, 1, 39, 1,
    0, 2, 22, 3, 0xa000, 0,
    2, 22, 3, 0x8000, 0, 2,
    22, 3, 0x6000, 0, 2, 22,
    3, 0x4000, 17,
};
// clang-format on
