/*
 * VENEER AUDIT NOTE (2026-08-01) -- COMMENT ONLY, NO CODE CHANGE.
 *
 * This file is byte-exact, so nothing below is rewritten.  The note records
 * what the call sites actually are, so the next reader is not misled.
 *
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank: fifteen four-byte
 * `bx rN; nop` entries in register order r0..lr, ending at 0x08007320.  A
 * `bl` into that range is an INDIRECT CALL through the named register, not a
 * call to a function at the branch target.  The `Func_080072f*` prototype
 * this file declares is therefore a phantom, and the register load that
 * precedes each site -- which reads like dead code -- is the callee load.
 *
 * Why the file is still byte-identical while being wrong: a direct call to a
 * declared function at 0x080072f0 emits exactly the same `bl` the real
 * indirect call emits.  Converting to a function-pointer call would require
 * the compiler to choose the same register and therefore the same veneer
 * entry, which is a byte-exact source question and is deliberately NOT attempted
 * here.
 *
 * Sites in this owner, resolved with tools/veneer_resolve.ts:
 *
 *   0x0800dd10  __call_via_r3  ->  0x030001d8
 *     a relocated IWRAM routine taking ONE argument and returning one;
 *     every site in the tree feeds it a sum of squares and consumes the
 *     result as a length. Reads as a square root; NOT asserted
 */
#include "types.h"

s32 FixedPoint_Ratio(s32, s32);
s32 Func_080072f0(s32, s32, s32, s32);
void Func_0800c300(void *object, s32 argument);
s32 Func_0800d14c(void *, s32, s32, s32);
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
  base = (u8 *) arg0;
  p = base;
  link = *((void **) (p + 0x68));
  *((s32 *) (p + 0x30)) = (s32) (*((s32 *) (((u8 *) link) + 0x30)));
  *((s32 *) (p + 0x34)) = (s32) (*((s32 *) (((u8 *) link) + 0x34)));
  dx = (*((s32 *) (((u8 *) link) + 8))) - (*((s32 *) (p + 8)));
  dz = (*((s32 *) (((u8 *) link) + 0x10))) - (*((s32 *) (p + 0x10)));
  dxh = dx >> 0x10;
  dzh = dz >> 0x10;
  len = Func_080072f0((dxh * dxh) + (dzh * dzh), dx, dzh, 0x030001D8);
  if (len > 0x10)
  {
    dx2 = dx;
    n = len - 0x10;
    mx = FixedPoint_Ratio(dx2 * n, len);
    mz = FixedPoint_Ratio(dz * n, len);
    Func_0800d14c(arg0, (*((s32 *) (p + 8))) + mx, *((s32 *) (p + 0xC)), (*((s32 *) (p + 0x10))) + mz);
    Func_0800c300(arg0, 2);
    *((u16 *) (p + 4)) = (u16) ((*((u16 *) (p + 4))) + 1);
    return 1;
  }
  Func_0800c300(arg0, 1);
  return 0;
}
