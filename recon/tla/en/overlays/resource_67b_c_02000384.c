// Draft, differ 94 of 107: picks a spot for a wandering object: up to three
// times, a point 4 to 12 pixels ahead within a quarter turn of its angle;
// the first within 16 pixels of its home (the halfwords at 0x64 and 0x66)
// becomes its target (Func_02002930); after three misses it turns round and
// sets its halfword at 0x5e. Returns 0. Left: the native keeps the try count in r10 and 256 in r9, rounds the spot toward zero through a test and add, and lays the loop out with its exit tests in another order.
#include "types.h"
#include "object_runtime.h"
#include "iwram_call.h"
#include "system/vector3.h"

u16 Random16Far(void);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, struct Vector3 *v);
void Func_02002930(struct ObjectRuntime *object, s32 x, s32 y, s32 z);

s32 Func_02000384(struct ObjectRuntime *object) {
  struct Vector3 spot;
  s32 tries = 0, distance, dx, dz, a, b;

  for (;;) {
    tries++;
    if (tries > 3) {
      object->angle += 0x8000;
      *(u16 *)((u8 *)object + 0x5e) = 1;
      return 0;
    }
    spot.x = object->x;
    spot.y = object->y;
    spot.z = object->z;
    distance = Iwram_MulQ16(Random16Far(), 0x80000) + 0x40000;
    a = Random16Far();
    b = Random16Far();
    Vector_AddPolarOffsetFar(distance, object->angle + (a >> 2) - (b >> 2),
                             &spot);
    dx = spot.x / 0x10000 - object->action;
    dz = spot.z / 0x10000 - (s16)object->unk66;
    if (dx * dx + dz * dz <= 256)
      break;
  }
  Func_02002930(object, spot.x, spot.y, spot.z);
  return 0;
}
