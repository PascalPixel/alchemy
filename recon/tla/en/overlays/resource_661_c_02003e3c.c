// Draft, differ 24 of 93: sets up one rising platform entry (timer, limits
// in order, a 360-frame period) unless flag 0x109 is set, and spawns its
// rider object. Left: register choice; the native folds the entries' offset
// apart from the table address and stores motionFlags from a pool zero.
#include "types.h"
#include "object_runtime.h"

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
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
struct ObjectRuntime *Func_02003fc0(s32 kind, s32 x, s32 y, s32 z);

struct RideEntry *Func_02003e3c(s32 slot, s32 id, s32 timer, s32 low,
                                s32 high) {
  struct RideEntry *entry =
      &((struct RideWork *)(gData_020023c4 + 0x188))->entries[slot];
  struct ObjectRuntime *object = Object_GetById(id);
  struct ObjectRuntime *rider;

  Object_GetById(id);
  ObjectDispatch_SetSingleChildFlags(object, 0);
  if (!GameFlag_Test(0x109)) {
    if (high < low) {
      s32 swap = high;

      high = low;
      low = swap;
    }
    entry->id = id;
    entry->timer = timer;
    entry->period = 360;
    entry->riseAt = 60;
    entry->fallAt = 240;
    entry->min = low;
    entry->unk10 = 1;
    object->motionFlags = 0;
    object->collisionFlags = 8;
    entry->height = 0;
    entry->max = high;
    entry->step = 0;
    entry->phase = 0;
    entry->unk14 = 0;
    object->priorityFlags |= 2;
  }
  rider = Func_02003fc0(0x8000, object->x, object->y, object->z);
  rider->collisionFlags = 0;
  rider->motionFlags = 0;
  rider->priorityFlags |= 128;
  entry->rider = rider;
  return entry;
}
