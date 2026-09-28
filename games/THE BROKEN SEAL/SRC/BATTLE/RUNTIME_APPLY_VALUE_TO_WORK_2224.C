/*
 * Apply a value to the battle work record at 0x02002224.
 */
#include "TYPES.H"
extern u8 IwramClearWords[];

/*
 * _call_via_r3 names a `bx rN` slot: the call is indirect through the
 * register that slot selects, and the trailing argument is the callee
 * address at 0x03000164. That routine is reached with two arguments at
 * some sites and three at others, so its shape is not established.
 */
s16 _call_via_r3(s32, s32, s16, s32);

/*
 * The third argument reads val before val is written, so it carries
 * whatever the register already holds; it must not be respelled as a fresh
 * load. The two assignments that follow the call keep that order.
 */
char Battle_ApplyValueToWork2224(s16 arg2)
{
  s16 val;
  s16 val2;
  _call_via_r3(0x02002224, 0x10, val, (u32)IwramClearWords);
  val2 = arg2;
  val = val2;
}
