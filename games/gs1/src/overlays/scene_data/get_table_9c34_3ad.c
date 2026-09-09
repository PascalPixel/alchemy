#include "types.h"

/*
 * Overlay resource_3ad. Exported getter for an in-image table, published
 * from the overlay header.
 */

/*
 * The eight-byte owner at 0x02000030 includes its one pool word, which holds
 * the returned address; the table is returned without being dereferenced.
 * Another overlay exports the same address, but only one overlay is resident
 * at 0x02000000 at a time, so that is a different table -- do not merge the
 * two getters.
 */
u8 *SceneData_GetTable9c34(void)
{
    return (u8 *)0x02009c34;
}
