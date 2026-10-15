#include "event_work.h"
#include "object_lookup.h"
#include "types.h"

struct ObjectPairPosition {
  u8 unk00[6];
  s16 angle;
  s32 x;
  s32 y;
  s32 z;
};

s32 ArcTan2(s32, s32);

void ObjectMotion_SetAngleToward(s32 firstId, s32 secondId, s32 wait) {
  struct ObjectPairPosition *first;
  struct ObjectPairPosition *second;

  first = ObjectTable_Get(firstId);
  second = ObjectTable_Get(secondId);
  if (first != 0 && second != 0) {
    first->angle = ArcTan2(second->z - first->z, second->x - first->x);
    EventRuntime_Wait(wait);
  }
}
