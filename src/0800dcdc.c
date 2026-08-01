/*
 * VENEER AUDIT NOTE (mars, 2026-08-01) -- COMMENT ONLY, NO CODE CHANGE.
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
 * entry, which is an exact-lane question and is deliberately NOT attempted
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

s32 Func_080022ec(s32, s32);
s32 Func_080072f0(s32, s32, s32, s32);
void Func_0800c300(void *object, s32 argument);
s32 Func_0800d14c(void *, s32, s32, s32);
s32 Func_0800dcdc(void *arg0)
{
  s32 temp_r0;
  s32 new_var;
  s32 temp_r1;
  s32 temp_r2_2;
  u8 *new_var3;
  s32 temp_r2_3;
  s32 temp_r3;
  s32 temp_r5;
  s32 temp_r8;
  s32 new_var2;
  u8 *new_var4;
  void *temp_r2;
  new_var4 = (u8 *) arg0;
  new_var3 = new_var4;
  temp_r2 = *((void **) (new_var3 + 0x68));
  *((s32 *) (new_var3 + 0x30)) = (s32) (*((s32 *) (((u8 *) temp_r2) + 0x30)));
  *((s32 *) (new_var3 + 0x34)) = (s32) (*((s32 *) (((u8 *) temp_r2) + 0x34)));
  temp_r1 = (*((s32 *) (((u8 *) temp_r2) + 8))) - (*((s32 *) (new_var3 + 8)));
  temp_r2_2 = (*((s32 *) (((u8 *) temp_r2) + 0x10))) - (*((s32 *) (new_var3 + 0x10)));
  temp_r3 = temp_r1 >> 0x10;
  temp_r2_3 = temp_r2_2 >> 0x10;
  temp_r0 = Func_080072f0((temp_r3 * temp_r3) + (temp_r2_3 * temp_r2_3), temp_r1, temp_r2_3, 0x030001D8);
  if (temp_r0 > 0x10)
  {
    new_var2 = temp_r1;
    temp_r5 = temp_r0 - 0x10;
    temp_r8 = Func_080022ec(new_var2 * temp_r5, temp_r0);
    new_var = Func_080022ec(temp_r2_2 * temp_r5, temp_r0);
    Func_0800d14c(arg0, (*((s32 *) (new_var3 + 8))) + temp_r8, *((s32 *) (new_var3 + 0xC)), (*((s32 *) (new_var3 + 0x10))) + new_var);
    Func_0800c300(arg0, 2);
    *((u16 *) (new_var3 + 4)) = (u16) ((*((u16 *) (new_var3 + 4))) + 1);
    return 1;
  }
  Func_0800c300(arg0, 1);
  return 0;
}

