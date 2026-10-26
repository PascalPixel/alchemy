// Draft, differ 19 of 93: sets up one of the Trial Road's moving objects in its
// 28-byte slot (index) of the Trial Road work at gData_020023c4 + 0x18c: unless
// flag 265 is set, records the object, its value and the range between low and
// high (sorted), with 360, 60 and 240 and motion off; then creates its marker
// object (type 0x8000) at the object and returns the slot. The slot is
// read as the movers table at gData_020023c4 + 0x188, which keeps the
// native's separate add of 4. Left: global.c gives low r7 and high r10, the
// native the other way round, so the compare and the two range stores use the
// other registers and the low store is scheduled later. Tried: the swap
// written from either end (28 the other way), a block-local temporary, s16
// parameters (29, 48), every one-line move.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"

struct TrialRoadMover {
  s16 id;
  s16 value;
  s16 unk04;
  s16 unk06;
  s16 unk08;
  s16 unk0a;
  s16 high;
  s16 low;
  s16 unk10;
  s16 unk12;
  s16 unk14;
  s16 unk16;
  struct ObjectRuntime *marker;
};

struct TrialRoadMovers {
  u8 unk00[2];
  s16 shape;
  struct TrialRoadMover movers[16];
};

void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);

struct TrialRoadMover *Func_02004224(s32 index, s32 id, s32 value, s32 low,
                                     s32 high) {
  struct TrialRoadMovers *work =
      (struct TrialRoadMovers *)(gData_020023c4 + 0x188);
  struct TrialRoadMover *mover = &work->movers[index];
  struct ObjectRuntime *object = Object_GetById(id);
  struct ObjectRuntime *marker;
  s32 swap;

  Object_GetById(id);
  ObjectDispatch_SetSingleChildFlags(object, 0);
  if (!GameFlag_Test(265)) {
    if (high < low) {
      swap = high;
      high = low;
      low = swap;
    }
    mover->id = id;
    mover->value = value;
    mover->unk06 = 360;
    mover->unk08 = 60;
    mover->unk0a = 240;
    mover->low = low;
    mover->unk10 = 1;
    object->motionFlags = 0;
    object->collisionFlags = 8;
    mover->unk04 = 0;
    mover->high = high;
    mover->unk12 = 0;
    mover->unk16 = 0;
    mover->unk14 = 0;
    object->priorityFlags |= 2;
  }
  marker = Func_020057d8(0x8000, object->x, object->y, object->z);
  marker->collisionFlags = 0;
  marker->motionFlags = 0;
  marker->priorityFlags |= 128;
  mover->marker = marker;
  return mover;
}
