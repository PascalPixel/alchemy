/// @file
/// Scene code of overlay 0x674, in ROM order, from s3410's offset up to the next object the overlay links.

#include "types.h"
#include "heap_release.h"
#include "palette_snapshots.h"
#include "field.h"

void Func_02003410(void) {
  struct PaletteSnapshotWork *queue;
  struct PaletteSnapshot *entry;
  s16 i, j;

  queue = HeapSlot_AllocEwram(112, sizeof(struct PaletteSnapshotWork));
  entry = queue->entries;
  i = 0;
outer:
  entry->source = 0;
  entry->position = 0;
  entry->timer = 0;
  entry->delay = 0;
  entry->count = 0;
  j = 0;
inner:
  entry->colors[(u16)j] = 0;
  {
    s32 next = ((u16)j + 1) << 16;

    j = next >> 16;
    if (next != 16 << 16)
      goto inner;
  }
  entry++;
  {
    s32 next = (i << 16) + 0x10000;

    i = next >> 16;
    if (next != 4 << 16)
      goto outer;
  }
  queue->count = 0;
}

// Overlay 674 keeps its own palette queue between its allocation and step
// functions; its PaletteQueue_Add is the main image's.
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"

#define gPaletteWork ((struct PaletteSnapshotWork *)Ram_HeapSlots->unk70[0])

#include "../../field/palqueue_add.c"
#undef gPaletteWork

#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_020034fc(void);

void Func_020034d8(void) {
  Task_AddFrame(&Func_020034fc, 0x480);
  return;
}

void Func_020034ec(void) {
  FieldTask_Remove(Func_020034fc);
  return;
}

struct PaletteCycle {
  u16 *source;
  s16 position;
  u16 timer;
  s16 delay;
  s16 count;
  u16 colors[16];
};

struct PaletteCycleWork {
  struct PaletteCycle entries[4];
  u16 count;
};

#define gPaletteWork ((struct PaletteCycleWork *)Ram_HeapSlots->unk70[0])
#define EVENT_S16(offset) (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + (offset)))

void Func_020034fc(void) {
  if (EVENT_S16(0x166) == 0 && EVENT_S16(0x15a) == 0 && EVENT_S16(0x15e) == 0) {
    struct PaletteCycleWork *work = gPaletteWork;
    u8 i;
    u16 count = work->count & 3;
    u16 buf[16];

    for (i = 0; i < count; i++) {
      struct PaletteCycle *cycle = &work->entries[i];
      if (cycle->timer == 0) {
        u16 start = cycle->position;
        u16 len = cycle->count;
        void *dest = cycle->source;
        u16 *src = cycle->colors;
        u8 j;
        for (j = len - start; j < len; j++)
          buf[j] = *src++;
        for (j = 0; j < len - start; j++)
          buf[j] = *src++;
        Dma_Set(buf, dest, DMA_COPY_HALFWORDS(len), REG_DMA3);
        start++;
        if (start >= len)
          start = 0;
        cycle->position = start;
        cycle->timer = cycle->delay;
      } else {
        cycle->timer--;
      }
    }
  }
}

void Func_02003e64();
void Func_02003f84();

void Func_02003620(void) {
  s32 v5;
  struct Actor *object0;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(0x226);
  v5 = &gMainRoutineWork;
  v5 = (v5 + 0x214);
  object0 = Actor_Get(*(s32 *)((u8 *)v5 + 0));
  Actor_SetSingleChildFlags((s32)object0, 0);
  Actor_SetAnimation(*(s32 *)((u8 *)v5 + 0), 18);
  FieldEvent_Wait(40);
  *(s32 *)((u8 *)((s32)Ram_HeapSlots->eventWork + 0x1ac) + 0) = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(3);
  return;
}

void Func_0200367c(void) {
  Func_02003f84();
  return;
}

// Rewrites the map cells under a pushable actor. The actor's sprite
// shape picks one of the six footprints in gData_0200407c (four signed bytes
// each, in sixteenths of a cell); the cells under it are copied from the
// map's spare area (Map_CopyCellAttributeRect) and byte 2 of both collision
// layers there is filled with 255 (Func_02003790). Returns 1, or 0 for a shape
// with no footprint. The twin of overlay 668's Func_020040ec.

#include "islets.h"

extern s8 gData_0200407c[][4];
void Func_02003790(s32 layer, s32 x, s32 z, u32 width, u32 height, s32 value);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

/// Rewrites the map cells under a pushable actor. The actor's sprite shape
/// picks one of the six footprints in gData_0200407c (four signed bytes each,
/// in sixteenths of a cell); the cells under it are copied from the map's spare
/// area (Map_CopyCellAttributeRect) and byte 2 of both collision layers there
/// is filled with 255 (Func_02003790). Returns 1, or 0 for a shape with no
/// footprint. The twin of overlay 668's Func_020040ec.
s32 Func_02003684(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *actor = Actor_GetById(id);
  s16 kind = *((struct ObjectAnimationState *)actor->animation)->value28;
  s16 i = 0;
  s32 next;
  u32 index;
  s32 a, b;
  s32 fromX, fromZ;
  s32 width, height, x, z;

  // A goto loop: a C loop is strength-reduced, which the original's is not.
  if ((u16)kind != gData_02004070[(u16)i]) {
  again:
    next = (i + 1) * 65536;
    index = (u32)next >> 16;
    i = next >> 16;
    if (index <= 5 && (u16)kind != gData_02004070[index])
      goto again;
  }
  // fromX and b are reused for the next values they hold.
  fromX = (u16)i;
  if (fromX == 6)
    return 0;
  a = gData_0200407c[fromX][0];
  width = (Abs(a) + Abs(gData_0200407c[fromX][2])) >> 4;
  b = gData_0200407c[fromX][1];
  height = (Abs(b) + Abs(gData_0200407c[fromX][3])) >> 4;
  x = actor->x + (a << 16);
  z = actor->z + (b << 16);
  b = *(s32 *)(map + 0x148) >> 20;
  x >>= 20;
  z >>= 20;
  fromZ = *(s32 *)(map + 0x14c) >> 20;
  fromX = (u16)b;
  Map_CopyCellAttributeRect(x, z, width, height, x + fromX, z + (u16)fromZ);
  Func_02003790(0, x, z, width, height, 255);
  Func_02003790(2, x, z, width, height, 255);
  return 1;
}

void Func_02003790(s32 layer, s32 x, s32 z, u32 width, u32 height, s32 value) {
  struct MapCell *cells = Ram_HeapSlots->mapWork->layers[layer].cells;
  struct MapCell *cell;
  u32 i;
  u32 j;

  cells += x + z * 128;
  for (i = 0; i < height; i++) {
    cell = cells + i * 128;
    for (j = 0; j < width; j++) {
      cell->collisionCode = value;
      cell++;
    }
  }
}
