#include "trial_road.h"
#include "iwram_call.h"
#include "event_inline.h"
#include "types.h"
#include "actor.h"

struct TrialRoadGauge {
  u16 handle;
  u16 field02;
  u8 unk004[0x1c0];
  void *buffer;
};

extern const u8 gField_Tiles64D[];

void *Heap_AllocEwram(s32 size);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);
void Func_02003da0(void);
void Func_02003f68(void);
void Func_02005770(const void *source, void *destination);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void *Pool_Alloc(s32 size) { return Heap_AllocEwram(size); }

static __inline__ void Words_Clear(void *destination, s32 size) {
  Iwram_ClearWords(destination, size);
}

void Func_02004188(s32 a, s32 b) {
  struct TrialRoadGauge *gauge =
      (struct TrialRoadGauge *)(gData_020023c4 + 0x188);
  void *buffer;

  if (!GameFlag_Test(0x109))
    Words_Clear(gauge, 0x1c8);
  buffer = Pool_Alloc(0x800);
  Func_02005770(gField_Tiles64D, buffer);
  gauge->handle = Resource_FindFreeEntry();
  VramCache_Load((s16)gauge->handle, 0x800, buffer);
  Heap_Release(buffer);
  gauge->buffer = Heap_AllocEwram(0x3c0);
  gauge->field02 = b;
  Task_Add(Func_02003da0, 0x48d);
  Task_Add(Func_02003f68, 0x480);
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 0x40;
}

/// One rising platform and what rides it.
struct RideEntry {
  s16 id;
  s16 timer;
  s16 height;
  s16 period;
  s16 riseAt;
  s16 fallAt;
  s16 max;
  s16 min;
  s16 unk10;
  s16 step;
  s16 unk14;
  s16 phase;
  struct Actor *rider;
};

struct RideWork {
  s16 entry;
  s16 value;
  struct RideEntry entries[16];
  void *block;
};

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);

struct RideEntry *Func_02004224(s32 slot, s32 id, s32 timer, s32 low,
                                s32 high) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct RideEntry *entry = &work->entries[slot];
  struct Actor *object = Actor_GetById(id);
  struct Actor *rider;

  Actor_GetById(id);
  Actor_SetSingleChildFlags(object, 0);
  if (!GameFlag_Test(0x109)) {
    if (high < low) {
      s32 swap = low;

      low = high;
      high = swap;
    }
    entry->id = id;
    entry->height = 0;
    entry->timer = timer;
    entry->period = 360;
    entry->max = high;
    entry->riseAt = 60;
    entry->fallAt = 240;
    entry->min = low;
    entry->unk10 = 1;
    object->motionFlags = 0;
    object->collisionFlags = 8;
    entry->step = 0;
    entry->phase = 0;
    entry->unk14 = 0;
    object->priorityFlags |= 2;
  }
  rider = ((struct Actor *)Import_ActorCreate(0x8000, object->x, object->y,
                                              object->z));
  rider->collisionFlags = 0;
  rider->motionFlags = 0;
  rider->priorityFlags |= 128;
  entry->rider = rider;
  return entry;
}

// The trap table's setup: a trap whose second flag is set is put away; any other marks its cell, writes a shape for its height into the shape grid, and takes mode 0 when its first flag is set. Then the leader is lifted onto the ground if it stands below it.
#include "ram_buffer.h"
#include "map.h"
#include "main_routine_work.h"

extern u8 gMapShapeGrid[];

s32 Func_02005a18(struct Actor *object);
s32 Func_02005848(s32 layer, s32 x, s32 z);
void Func_02005958(s32 id);
u8 Func_02005a20(s32 layer, s32 shape, s32 height);

/// The trap table's setup: a trap whose second flag is set is put away; any
/// other marks its cell, writes a shape for its height into the shape grid, and
/// takes mode 0 when its first flag is set. Then the leader is lifted onto the
/// ground if it stands below it.
void Func_020042f0(const u16 *table) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  struct Actor *leader;
  s32 ground;

  if (*table == 0xffff)
    goto done;
next:
  id = table[0];
  object = Actor_GetById(id);
  flag = table[1];
  nextFlag = flag + 1;
  cells = map->layers[object->terrainId].cells;
  shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  if (GameFlag_Test(nextFlag)) {
    Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Func_02005a18(object);
    cell = &cells[index];
    slot = shapes + index;
    shape = Func_02005848(object->terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    Func_02005958(id);
    cell->attributeB = 128;
    *slot = Func_02005a20(object->terrainId, shape, (height >> 19) + 4);
    if (GameFlag_Test(flag))
      Func_020057c8(object, 0);
  }
  table += 2;
  if (*table != 0xffff)
    goto next;
done:
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  ground = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < ground) {
    leader->terrainHeight = ground;
    leader->y = ground;
  }
}
