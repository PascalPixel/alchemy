#include "types.h"
#include "scene.h"

/*
 * resource_3bb owner at 0x0200004c: two bytes, `bx lr', with no prologue and
 * no pool.  Two data-table slots install it as a handler, so the empty body
 * is deliberate rather than padding.  Whether those slots expect a void
 * handler or an identity is not settled here -- `bx lr' leaves r0 untouched,
 * so a caller reading a result gets back whatever it passed in.
 */

void Resource3bb_NoOpHandler(void)
{
}
