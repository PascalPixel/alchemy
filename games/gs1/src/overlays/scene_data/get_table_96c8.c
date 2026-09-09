#include "types.h"

/*
 * Overlay resource_3ca. Exported getter for an in-image table, published
 * from the overlay header.
 */

/*
 * The eight-byte owner at 0x02000030 includes its one pool word, which holds
 * the returned address; the table is returned without being dereferenced.
 */
u8 *SceneData_GetTable96c8(void)
{
    return (u8 *)0x020096c8;
}
