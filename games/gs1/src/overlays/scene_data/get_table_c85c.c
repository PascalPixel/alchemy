#include "types.h"

/*
 * Table getter for resource_3a4, published from the overlay's header as an
 * entry point.
 *
 * The eight-byte owner at 0x02000204 includes its one pool word at
 * 0x02000208; the load reads that word and returns it as an address,
 * without dereferencing it.
 */

extern u8 Data_0200c85c[];   /* Image offset 0x485c. */

u8 *SceneData_GetTableC85c(void)
{
    return Data_0200c85c;
}
