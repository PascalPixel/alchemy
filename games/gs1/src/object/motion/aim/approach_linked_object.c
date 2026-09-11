/*
 * Move an object toward the object linked at +0x68, stopping short of it
 * by a fixed margin.
 */
#include "types.h"

s32 FixedPoint_Ratio(s32, s32);

/*
 * Func_080072f0 names a `bx rN` slot, so the call is indirect through the
 * register that slot selects; the trailing argument is the callee address
 * loaded into that register, not a parameter of the callee. The callee
 * takes one argument and returns one; it is fed a sum of squares and its
 * result used as a length, which reads as a square root but is not
 * established.
 */
s32 Func_080072f0(s32, s32, s32, s32);
void Func_0800c300(void *object, s32 argument);
s32 Func_0800d14c(void *, s32, s32, s32);

/*
 * Copy the linked object's words at +0x30 and +0x34, then close the gap by
 * (len - 0x10) / len of it. The whole-pixel deltas feed the length call
 * while the unshifted deltas feed the ratios, and dx is taken through its
 * own local there.
 */
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
  len = Func_080072f0((dxh *dxh) + (dzh *dzh), dx, dzh, 0x030001D8);
  if (len > 0x10)
  {
    dx2 = dx;
    n = len - 0x10;
    mx = FixedPoint_Ratio(dx2 *n, len);
    mz = FixedPoint_Ratio(dz *n, len);
    Func_0800d14c(arg0, (*((s32 *)(p + 8))) + mx, *((s32 *)(p + 0xC)), (*((s32 *)(p + 0x10))) + mz);
    Func_0800c300(arg0, 2);
    *((u16 *)(p + 4)) = (u16)((*((u16 *)(p + 4))) + 1);
    return 1;
  }
  Func_0800c300(arg0, 1);
  return 0;
}
