// Draft, differ 89 of 213: the rising platforms callback: for each of the 16
// entries it raises or lowers the platform on its timer, writes its height
// into the collision map and cell flags, and carries its rider with a
// bobbing sine. Left: register choice and reloaded timer compares.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "iwram_call.h"
#include "object_runtime.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

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
  struct ObjectRuntime *rider;
};

struct RideWork {
  s16 entry;
  s16 value;
  struct RideEntry entries[16];
  void *block;
};

extern u8 gData_020023c4[];

s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_020041f8(s32 cue);
s32 Math_Sine(s32 angle);

void Func_020039b8(void) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 value = work->value;
  u8 *terrain = map->layers[0].terrainIds;
  struct MapCell *cells = map->layers[0].cells;
  s32 *collision = (s32 *)Ram_MapCollision + value;
  struct RideEntry *entry = work->entries;
  s32 i;

  for (i = 0; i <= 15; i++, collision++, entry++) {
    struct ObjectRuntime *object;
    struct ObjectRuntime *rider;
    struct MapCell *cell;
    s32 index;

    if (entry->id == 0)
      continue;
    object = Object_GetById(entry->id);
    if (object->x == 0 && object->z == 0)
      continue;
    if (entry->timer == entry->riseAt) {
      Func_020041f8(206);
      entry->step = 4;
    }
    if (entry->timer == entry->fallAt) {
      Func_020041f8(395);
      entry->step = -8;
    }
    if (entry->step != 0) {
      entry->height += entry->step;
      if (entry->height >= entry->max) {
        entry->height = entry->max;
        entry->step = 0;
        entry->phase = 0;
      }
      if (entry->height <= entry->min) {
        entry->height = entry->min;
        entry->step = 0;
        entry->phase = 0;
      }
    }
    if (!GameFlag_Test(0x104)) {
      if (++entry->timer >= entry->period)
        entry->timer = 0;
    }
    index = (object->x >> 20) + (object->z >> 20) * 128;
    terrain[index] = (u8)work->value + i;
    *collision = ((PIXELS(object->y) + entry->height) / 8) << 8;
    cell = &cells[index];
    if (entry->step != 0)
      cell->attributeB = 0;
    else
      cell->attributeB |= 128;
    if (entry->height != 0)
      cell->attributeB |= 16;
    rider = entry->rider;
    if (rider == NULL)
      continue;
    if (entry->height == 0) {
      rider->y = object->y + -0x100000;
    } else {
      entry->phase += 0x1000;
      rider->y = object->y + (entry->height << 16) +
                 (Iwram_MulQ16(0x20000, Math_Sine(entry->phase)) + -0xe0000);
    }
    rider->x = object->x;
    rider->z = object->z;
  }
}
