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

void Event_Start();
void Func_02003f84();

void Func_02003620(void) {
  s32 v5;
  struct Actor *object0;

  Event_Start();
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
  s16 kind = *((struct ObjectAnimationState *)actor->sprite)->value28;
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

#include "system/vector3.h"

extern s32 gData_02004094[];
struct Actor *Func_02003940(s32 *direction, s32 *number, s32 *kind);

static __inline__ s32 RowByte(s8 *row, s32 index) { return row[index]; }

static __inline__ s32 BoxSpan(s32 kind, s32 start) {
  return (Abs(gData_0200407c[kind][start]) +
          Abs(gData_0200407c[kind][start + 2])) >>
         4;
}

static __inline__ s32 CallFunc_02003e2c(struct Actor *object,
                                        struct Vector3 *cell) {
  return Func_02003e2c(object, cell);
}

s32 Func_020037d8(struct PushTarget *push) {
  struct Actor *object;
  struct Vector3 cell;
  s32 direction, kind;
  s32 dx, dz, steps, i, j, offset, result;

  object = Func_02003940(&direction, &push->id, &push->footprint);
  if (object != 0) {
    kind = push->footprint;
    /* FAKEMATCH(dead): the address of kind taken and dropped; as kind =
     * push->footprint alone, 127 of 171 instructions differ in tla-en. */
    (void)&kind;
    {
      s32 width = BoxSpan(kind, 0);
      s32 height = BoxSpan(kind, 1);
      u8 *layer;
      s32 index;

      offset = gData_02004094[direction];
      dx = offset & 0xffff0000;
      dz = offset << 16;
      push->callback = 0;
      steps = 0;
      layer = &object->terrainId;
      *layer = 2;
      push->x = object->x + dx;
      push->z = object->z + dz;
      cell.y = object->y;
      for (;;) {
        s8 *table = gData_0200407c[0];

        index = kind * 4;
        /* FAKEMATCH(wrapper): the read through RowByte; as table[index + 1],
         * 27 of 171 instructions differ in tla-en. */
        cell.z = push->z + (RowByte(table, index + 1) << 16);
        for (j = 0; j < height; j++) {
          cell.x = push->x + (gData_0200407c[0][index] << 16);
          for (i = 0; i < width; i++) {
            /* FAKEMATCH(wrapper): the call through CallFunc_02003e2c; as a
             * direct call, 2 of 171 instructions differ in tla-en. */
            if (CallFunc_02003e2c(object, &cell) == 2)
              goto blocked;
            cell.x += 0x100000;
          }
          cell.z += 0x100000;
        }
        push->x += dx;
        steps++;
        push->z += dz;
      }
    blocked:
      *layer = 0;
    }
    result = 0;
    if (steps != 0) {
      push->x = object->x + dx * steps;
      result = 1;
      push->y = object->y;
      push->z = object->z + dz * steps;
    }
  } else
    result = 0;
  return result;
}

/// The whole-pixel half of a 16.16 coordinate.
#define HI(v) (((s16 *)&(v))[1])

struct Actor *Func_02003940(s32 *direction, s32 *slot, s32 *kind) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor **entry;
  struct Actor *object;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = leader->facing >> 12;
  i = 8;
  entry = &work->objects[8];
outer:
  object = *entry;
  k = 0;
inner:
  if (((struct ObjectAnimationState *)object->sprite)->value28[0] ==
      gData_02004070[k]) {
    x = (HI(leader->x) + (gData_02004094[*direction] >> 16)) >> 4;
    z = (HI(leader->z) + (s16)gData_02004094[*direction]) >> 4;
    left = (HI(object->x) + gData_0200407c[k][0]) >> 4;
    top = (HI(object->z) + gData_0200407c[k][1]) >> 4;
    right = (HI(object->x) + gData_0200407c[k][2]) >> 4;
    bottom = (HI(object->z) + gData_0200407c[k][3]) >> 4;
    if (left <= x && x < right && top <= z && z < bottom) {
      *kind = k;
      if (k & 1) {
        if (left != leader->x >> 20) {
          *slot = i;
          return object;
        }
      } else {
        if (top != leader->z >> 20) {
          *slot = i;
          return object;
        }
      }
    }
  }
  k++;
  if (k <= 5)
    goto inner;
  entry++;
  i++;
  if (i <= 63)
    goto outer;
  return NULL;
}

void Event_NoOp(void);
void Func_02003d0c(void);

/// The heap slots read as words: the map slot comes through u32, not its
/// pointer type, so its load stays behind the stores of the block.
struct HeapSlotWords {
  u32 slots[8];
  u32 mapWork;
};

#define ABS(x) ((x) < 0 ? -(x) : (x))

void Func_02003a5c(struct PushTarget target) {
  u8 *map = (u8 *)((struct HeapSlotWords *)Ram_HeapSlots)->mapWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_Get(target.id);
  s32 cell[3];
  s32 from[3];
  s32 footprint;
  s32 width;
  s32 height;
  s32 originX;
  s32 originZ;
  s32 direction;
  s32 speed;
  s32 acceleration;

  direction = leader->facing >> 16;
  footprint = target.footprint;
  speed = leader->speedLimit;
  acceleration = leader->acceleration;
  from[0] = object->x;
  from[2] = object->z;
  cell[0] = (object->x + (gData_0200407c[footprint][0] << 16)) >> 20;
  cell[2] = (object->z + (gData_0200407c[footprint][1] << 16)) >> 20;
  width = (ABS(gData_0200407c[footprint][0]) +
           ABS(gData_0200407c[footprint][2])) >>
          4;
  height = (ABS(gData_0200407c[footprint][1]) +
            ABS(gData_0200407c[footprint][3])) >>
           4;
  Func_02003790(0, cell[0], cell[2], width, height, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x1999);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 8);
  System_Wait(15);
  {
    s32 deltaX = (target.x - from[0]) >> 17;
    s32 deltaZ = (target.z - from[2]) >> 17;

    Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, deltaX,
                                       deltaZ);
  }
  Actor_Get(gMainRoutineWork.currentOwner)->update = Func_02003d0c;
  System_Wait(4);
  if ((u32)(direction - 6) <= 7)
    Func_02003de4(object, 3);
  else
    Func_02003de4(object, 2);
  Event_PlaySound(239);
  Actor_SetSpeed(target.id, 0x8000, 0x1999);
  FieldActor_SetMoveTarget(object, target.x, target.y, target.z);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner,
                                     (gData_02004094[direction] << 16) >> 31,
                                     (gData_02004094[direction] << 16) >> 17);
  if (target.callback != 0)
    target.callback();
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Actor_WaitForMove(object);
  Event_PlaySound(298);
  Event_PlaySound(213);
  FieldActor_SetPositionAndResetMotion(object, target.x, object->y, target.z);
  Func_02003de4(object, 1);
  target.x = target.x + (gData_0200407c[footprint][0] << 16);
  target.z = target.z + (gData_0200407c[footprint][1] << 16);
  target.x >>= 20;
  target.z >>= 20;
  originX = *(s32 *)(map + 328) >> 20;
  originZ = *(s32 *)(map + 332) >> 20;
  MapGrid_CopyCellAttributes(target.x, target.z, width, height, originX + target.x, originZ + target.z);
  Func_02003790(0, target.x, target.z, width, height, 255);
  Func_02003790(2, target.x, target.z, width, height, 255);
  cell[0] = (from[0] + (gData_0200407c[footprint][0] << 16)) >> 20;
  cell[2] = (from[2] + (gData_0200407c[footprint][1] << 16)) >> 20;
  originX += cell[0];
  originZ += cell[2];
  MapGrid_CopyCellAttributes(originX, originZ, width, height, cell[0], cell[2]);
  Func_02003790(2, cell[0], cell[2], width, height, 0);
  Event_NoOp();
}
