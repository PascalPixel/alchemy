/*
 * Apply a value to the battle work record at 0x02002224.
 */
#include "types.h"

/*
 * Func_080072f0 names a `bx rN` slot: the call is indirect through the
 * register that slot selects, and the trailing argument is the callee
 * address at 0x03000164. That routine is reached with two arguments at
 * some sites and three at others, so its shape is not established.
 */
s16 Func_080072f0(s32, s32, s16, s32);

/*
 * The third argument reads val before val is written, so it carries
 * whatever the register already holds; it must not be respelled as a fresh
 * load. The two assignments that follow the call keep that order.
 */
char Battle_ApplyValueToWork2224(s16 arg2)
{
  s16 val;
  s16 val2;
  Func_080072f0(0x02002224, 0x10, val, 0x03000164);
  val2 = arg2;
  val = val2;
}
