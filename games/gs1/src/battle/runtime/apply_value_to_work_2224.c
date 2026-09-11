/*
 * Apply a value to the battle work record at 0x02002224.
 */
#include "types.h"
#include "scene.h"
#include "abi/battle/runtime/apply_value_to_work_2224.h"

/*
 * Battle_SetMode names a `bx rN` slot: the call is indirect through the
 * register that slot selects, and the trailing argument is the callee
 * address at 0x03000164. That routine is reached with two arguments at
 * some sites and three at others, so its shape is not established.
 */

/*
 * The third argument reads val before val is written, so it carries
 * whatever the register already holds; it must not be respelled as a fresh
 * load. The two assignments that follow the call keep that order.
 */
char Battle_ApplyValueToWork2224(s16 arg2)
{
  s16 val;
  s16 val2;
  Battle_SetMode(0x02002224, 0x10, val, 0x03000164);
  val2 = arg2;
  val = val2;
}
