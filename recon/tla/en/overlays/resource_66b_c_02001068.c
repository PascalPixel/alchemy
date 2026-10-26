// Draft, differ 2 of 93 (was 48): fills record `index` of the table at
// gData_020023c4 + 0x188 (28 bytes, from byte 4) for an actor that swings
// between two limits: unless flag 265 is set it records the actor, its start,
// the limits in order and the swing's constants (360, 60, 240) and makes the
// actor solid; it always creates the actor's shadow (type 0x8000) and keeps
// it in the record. Returns the record. The same listing as overlay 661's
// Func_02003e3c and 697's Func_02004224; written as overlay 661's draft (the
// rising platform entry). Left, as there: the native builds the priority
// byte's address (adds r1, r6, #0) after the collision byte's store; an empty
// asm after that store fixes it but lets the height and max stores rise
// above it, and moving them after it changes the allocation (23).
#include "types.h"
#include "object_runtime.h"

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
struct ObjectRuntime *Object_GetById(u32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
struct ObjectRuntime *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);

struct RideEntry *Func_02001068(s32 slot, s32 id, s32 timer, s32 low,
                                s32 high) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct RideEntry *entry = &work->entries[slot];
  struct ObjectRuntime *object = Object_GetById(id);
  struct ObjectRuntime *rider;

  Object_GetById(id);
  ObjectDispatch_SetSingleChildFlags(object, 0);
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
  rider = Engine_ObjectCreate(0x8000, object->x, object->y, object->z);
  rider->collisionFlags = 0;
  rider->motionFlags = 0;
  rider->priorityFlags |= 128;
  entry->rider = rider;
  return entry;
}
