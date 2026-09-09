#include "types.h"

/*
 * Shared helper for resource_3b1.  The owner at 0x0200486c is 18 bytes; the
 * halfword before the next owner at 0x02004880 is alignment, not part of it.
 */

/*
 * Both callees live inside this overlay and are declared without a prototype,
 * so each call site fixes its own arity.
 */
void Func_0200ae2a();
void Func_0200ad38();

void FieldScene_RunStepThen10(s32 a)
{
    Func_0200ae2a(a, 0);
    Func_0200ad38(10);
}
