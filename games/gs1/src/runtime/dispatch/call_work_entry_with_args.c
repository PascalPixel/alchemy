/*
 * Call a work entry through the runtime's indirect-call slot.
 */
#include "types.h"

/*
 * Func_080072ec names a `bx rN` slot: the call is an indirect call through
 * the register that slot selects, not a call to a function at that
 * address. The declaration stands in for the typed slot.
 */
s32 Func_080072ec(s32, s32, s32);

/*
 * The callee word is read from 0x02004000 and carried into the call as the
 * third argument -- that load is the callee load, not dead code. What the
 * word points at depends on the caller and is not established here.
 */
void Runtime_CallWorkEntryWithArgs(s32 arg0, s32 arg1)
{
  int base;
  int fn;
  base = (fn = 0x02004000);
  fn = *((s32 *)base);
  Func_080072ec(arg0, arg1, fn);
}
