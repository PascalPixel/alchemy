#include "types.h"
#include "scene.h"
#include "fixed_math.h"
extern u8 IwramSqrt[];
s32 _call_via_r3(s32, s32, s32, s32);
s32 Object_SetMoveTarget(void *, s32, s32, s32);
void ObjectDispatch_ApplyArgumentToChildren(void *object, s32 argument);

s32 Object_ApproachLinkedObject(void *arg0)
{
  s32 len;
  s32 mz;
  s32 dx;
  s32 dz;
  u8 *p;
  s32 dzh;
  s32 dxh;
  s32 n;
  s32 mx;
  s32 dx2;
  u8 *base;
  void *link;
  base = (u8 *)arg0;
  p = base;
  link = *((void **)(p + 0x68));
  *((s32 *)(p + 0x30)) = (s32)(*((s32 *)(((u8 *)link) + 0x30)));
  *((s32 *)(p + 0x34)) = (s32)(*((s32 *)(((u8 *)link) + 0x34)));
  dx = (*((s32 *)(((u8 *)link) + 8))) - (*((s32 *)(p + 8)));
  dz = (*((s32 *)(((u8 *)link) + 0x10))) - (*((s32 *)(p + 0x10)));
  dxh = dx >> 0x10;
  dzh = dz >> 0x10;
  len = _call_via_r3((dxh *dxh) + (dzh *dzh), dx, dzh, (u32)IwramSqrt);
  if (len > 0x10)
  {
    dx2 = dx;
    n = len - 0x10;
    mx = __divsi3(dx2 *n, len);
    mz = __divsi3(dz *n, len);
    Object_SetMoveTarget(arg0, (*((s32 *)(p + 8))) + mx, *((s32 *)(p + 0xC)), (*((s32 *)(p + 0x10))) + mz);
    ObjectDispatch_ApplyArgumentToChildren(arg0, 2);
    *((u16 *)(p + 4)) = (u16)((*((u16 *)(p + 4))) + 1);
    return 1;
  }
  ObjectDispatch_ApplyArgumentToChildren(arg0, 1);
  return 0;
}
