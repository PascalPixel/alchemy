/*
 * Move an object toward the object linked at +0x68, stopping short of it
 * by a fixed margin.
 */
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "FIXED_MATH.H"
extern u8 IwramSqrt[];
s32 _call_via_r3(s32, s32, s32, s32);
void ObjectDispatch_ApplyArgumentToChildren(void *object, s32 argument);

/*
 * _call_via_r3 names a `bx rN` slot, so the call is indirect through the
 * register that slot selects; the trailing argument is the callee address
 * loaded into that register, not a parameter of the callee. The callee
 * takes one argument and returns one; it is fed a sum of squares and its
 * result used as a length, which reads as a square root but is not
 * established.
 */

/*
 * Copy the linked object's words at +0x30 and +0x34, then close the gap by
 * (len - 0x10) / len of it. The whole-pixel deltas feed the length call
 * while the unshifted deltas feed the ratios, and dx is taken through its
 * own local there.
 */
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
