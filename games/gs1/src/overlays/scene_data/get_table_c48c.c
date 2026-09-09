#include "types.h"

/*
 * Overlay resource_3bb. Exported getter for an in-image table, published
 * from the overlay header.
 */

/*
 * The eight-byte owner at 0x02000044 includes its one pool word, which holds
 * the returned address; the table is returned without being dereferenced.
 */
u8 *SceneData_GetTableC48c(void)
{
    return (u8 *)0x0200c48c;
}
