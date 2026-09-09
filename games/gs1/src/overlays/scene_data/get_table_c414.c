#include "types.h"

/*
 * Table getter for resource_3bb, published from the overlay's header.
 *
 * The eight-byte owner at 0x02000030 includes its one pool word at
 * 0x02000034; the load reads that word and returns it as an address,
 * without dereferencing it.
 */

u8 *SceneData_GetTableC414(void)
{
    return (u8 *)0x0200c414;
}
