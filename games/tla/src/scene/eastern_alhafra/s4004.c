/// @file
/// Scene code of overlay 0x668, in ROM order, from s4004's offset up to the next object the overlay links.

#include "types.h"
#include "map.h"
#include "system/task.h"
#include "event_inline.h"

extern char ResourceId_FallingRock;

struct Rock {
  u8 unk00[24];
  s32 timer;
};

struct RockSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 unk05_5 : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  u16 resource;
};

struct RockWork *HeapSlot_AllocFar(s32 slot, s32 size);
void *Resource_GetTableEntry(s32 id);
void Func_020049d4(void *entry, void *destination);
s32 Resource_FindFreeEntry(void);
void Func_02004c64(struct RockSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);
void Func_02003ec4(void);

static __inline__ void StartTask(void (*func)(void), s32 priority) {
  /* FAKEMATCH(wrapper): Task_Add forwarded inline; as a direct call, 6 of 83
   * instructions differ in tla-en. */
  Task_Add(func, priority);
}

void Func_02004004(void) {
  struct RockWork *work = HeapSlot_AllocFar(220, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct RockSprite *sprite;

  Func_020049d4(Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  entry = Resource_FindFreeEntry();
  tile = Vram_Load(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--, sprite++, rock++) {
    Func_02004c64(sprite, 8, 8, 0x40000000, tile);
    sprite->unk05_5 = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    rock->timer = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  StartTask(Func_02003ec4, 0x480);
}

#include "ram_buffer.h"
#include "resource.h"
#include "heap_release.h"

struct EasternAlhafraSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

void Func_020040c0(void) {
  struct EasternAlhafraSparkWork *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(Func_02003ec4);
  Resource_ResetEntry(work->resource);
  HeapSlot_Release(220);
}

// Rewrites the map cells under a pushable actor. The actor's sprite
// shape picks one of the six footprints in gData_02004cb0 (four signed bytes
// each, in sixteenths of a cell); the cells under it are copied from the
// map's spare area (Map_CopyCellAttributeRect) and byte 2 of both collision
// layers there is filled with 255. Returns 1, or 0 for a shape with no
// footprint.

#include "actor.h"

extern u16 gData_02004ca4[];
extern s8 gData_02004cb0[][4];

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

/// Rewrites the map cells under a pushable actor. The actor's sprite shape
/// picks one of the six footprints in gData_02004cb0 (four signed bytes each,
/// in sixteenths of a cell); the cells under it are copied from the map's spare
/// area (Map_CopyCellAttributeRect) and byte 2 of both collision layers there
/// is filled with 255. Returns 1, or 0 for a shape with no footprint.
s32 Func_020040ec(s32 id) {
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
  if ((u16)kind != gData_02004ca4[(u16)i]) {
  again:
    next = (i + 1) * 65536;
    index = (u32)next >> 16;
    i = next >> 16;
    if (index <= 5 && (u16)kind != gData_02004ca4[index])
      goto again;
  }
  // fromX and b are reused for the next values they hold.
  fromX = (u16)i;
  if (fromX == 6)
    return 0;
  a = gData_02004cb0[fromX][0];
  width = (Abs(a) + Abs(gData_02004cb0[fromX][2])) >> 4;
  b = gData_02004cb0[fromX][1];
  height = (Abs(b) + Abs(gData_02004cb0[fromX][3])) >> 4;
  x = actor->x + (a << 16);
  z = actor->z + (b << 16);
  b = *(s32 *)(map + 0x148) >> 20;
  x >>= 20;
  z >>= 20;
  fromZ = *(s32 *)(map + 0x14c) >> 20;
  fromX = (u16)b;
  Map_CopyCellAttributeRect(x, z, width, height, x + fromX, z + (u16)fromZ);
  Map_FillCellRectByte2(0, x, z, width, height, 255);
  Map_FillCellRectByte2(2, x, z, width, height, 255);
  return 1;
}

#include "../../field/map/cellfill.c"

#include "system/vector3.h"

struct PushState {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
  s32 number;
  s32 unk14;
};

extern s32 gData_02004cc8[];
struct Actor *Func_020043a8(s32 *direction, s32 *number, s32 *kind);
s32 Func_02004a64(struct Actor *actor, struct Vector3 *cell);

static __inline__ s32 CallFunc_02004a64(struct Actor *actor,
                                        struct Vector3 *cell) {
  return Func_02004a64(actor, cell);
}

/// Measures a push: finds the pushable actor ahead of the leader
/// (Func_020043a8, recording its number and kind in the push state), then
/// steps the actor's box one cell at a time along the leader's direction
/// until a cell of it is blocked (Func_02004a64 returns 2); with at least one
/// free step it records the end position in the push state and returns 1,
/// else 0. The actor stays on layer 2 while it is measured. The same code as
/// overlay 674's Func_020037d8 and overlay 662's Func_020001d4.
s32 Func_02004240(struct PushState *push) {
  struct Actor *actor;
  struct Vector3 cell;
  s32 direction, kind, width, height, dx, dz, steps, i, j, offset;
  u8 *layer;

  actor = Func_020043a8(&direction, &push->number, &push->kind);
  if (actor != 0) {
    kind = push->kind;
    width = (Abs(gData_02004cb0[kind][0]) + Abs(gData_02004cb0[kind][2])) >> 4;
    height = (Abs(gData_02004cb0[kind][1]) + Abs(gData_02004cb0[kind][3])) >> 4;
    offset = gData_02004cc8[direction];
    dx = offset & 0xffff0000;
    dz = offset << 16;
    push->unk14 = 0;
    steps = 0;
    layer = &actor->terrainId;
    *layer = 2;
    push->x = actor->x + dx;
    push->z = actor->z + dz;
    cell.y = actor->y;
    for (;;) {
      cell.z = push->z + (gData_02004cb0[kind][1] << 16);
      for (j = 0; j < height; j++) {
        cell.x = push->x + (gData_02004cb0[kind][0] << 16);
        for (i = 0; i < width; i++) {
          /* FAKEMATCH(wrapper): the call through CallFunc_02004a64; as a direct
           * call, 2 of 171 instructions differ in tla-en. */
          if (CallFunc_02004a64(actor, &cell) == 2)
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
    if (steps == 0)
      return 0;
    {
      s32 move = dx * steps;

      push->x = actor->x + move;
    }
    push->y = actor->y;
    push->z = actor->z + dz * steps;
    return 1;
  }
  return 0;
}

#include "main_routine_work.h"
#include "event_work.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

struct Actor *Func_020043a8(s32 *direction, s32 *slot, s32 *kind) {
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
      gData_02004ca4[k]) {
    x = (PIXELS(leader->x) + (gData_02004cc8[*direction] >> 16)) >> 4;
    z = (PIXELS(leader->z) + (s16)gData_02004cc8[*direction]) >> 4;
    left = (PIXELS(object->x) + gData_02004cb0[k][0]) >> 4;
    top = (PIXELS(object->z) + gData_02004cb0[k][1]) >> 4;
    right = (PIXELS(object->x) + gData_02004cb0[k][2]) >> 4;
    bottom = (PIXELS(object->z) + gData_02004cb0[k][3]) >> 4;
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
