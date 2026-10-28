// Draft, differ 2 of 93 (was 22): sets up one rising platform entry (timer,
// limits in order, a 360-frame period) unless flag 0x109 is set, and spawns
// its rider object. Measured: the work pointer as a local keeps the entries'
// offset apart from the table address (adds r5, r3, #4), the swap saves the
// low limit, and the height and max stores sit where the native's allocation
// follows (alchemy permute found the last). Left: the native builds the
// priority byte's address (adds r1, r6, #0) after the collision byte's store,
// as Aqua Rock's Func_02002020 does; a one-pass block around that store, as
// there, moves the halfword stores before it (6), and with the stores
// reordered around it the allocation changes (25).
#include "types.h"
#include "actor.h"

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

extern u8 gData_020023c4[];

s32 GameFlag_Test(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
struct Actor *Func_02003fc0(s32 kind, s32 x, s32 y, s32 z);

struct RideEntry *Func_02003e3c(s32 slot, s32 id, s32 timer, s32 low,
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
  rider = Func_02003fc0(0x8000, object->x, object->y, object->z);
  rider->collisionFlags = 0;
  rider->motionFlags = 0;
  rider->priorityFlags |= 128;
  entry->rider = rider;
  return entry;
}
