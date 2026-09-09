#include "types.h"

/*
 * Overlay resource_3ad. Exported getter for an in-image table, published
 * from the overlay header.
 */

/*
 * The eight-byte owner at 0x0200003c includes its one pool word, which holds
 * the returned address; the table is returned without being dereferenced.
 */
u8 *SceneData_GetTable9CAC(void)
{
    return (u8 *)0x02009cac;
}
