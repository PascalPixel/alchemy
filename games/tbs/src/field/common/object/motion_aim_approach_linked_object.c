#include "types.h"
#include "object_runtime.h"
#include "fixed_math.h"
extern u8 IwramSqrt[];
s32 _call_via_r3(s32, s32, s32, s32);
void ObjectDispatch_ApplyArgumentToChildren(void *object, s32 argument);

s32 Object_ApproachLinkedObject(struct ObjectRuntime *object)
{
  s32 len;
  s32 mz;
  s32 dx;
  s32 dz;
  s32 dzh;
  s32 dxh;
  s32 n;
  s32 mx;
  s32 dx2;
  struct ObjectRuntime *link;

  link = object->linked_object;
  object->speed_limit = (s32)(link->speed_limit);
  object->acceleration = (s32)(link->acceleration);
  dx = (link->x) - (object->x);
  dz = (link->z) - (object->z);
  dxh = dx >> 0x10;
  dzh = dz >> 0x10;
  len = _call_via_r3((dxh *dxh) + (dzh *dzh), dx, dzh, (u32)IwramSqrt);
  if (len > 0x10)
  {
    dx2 = dx;
    n = len - 0x10;
    mx = dx2 *n / len;
    mz = dz *n / len;
    Object_SetMoveTarget(object, (object->x) + mx, object->y, (object->z) + mz);
    ObjectDispatch_ApplyArgumentToChildren(object, 2);
    object->step = (u16)((object->step) + 1);
    return 1;
  }
  ObjectDispatch_ApplyArgumentToChildren(object, 1);
  return 0;
}
