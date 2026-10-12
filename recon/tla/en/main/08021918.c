/* Near miss, differ 4 of 17: the native loads the table address after the id
   is scaled by 4 and before the add (lsls r0, r2, #2; ldr r3; adds r0, r0,
   r2); with the table in a local the load moves above the compare, without it
   below the add. */
#include "types.h"

struct ResourceMetadataRecord {
    u8 unknown_00[20];
};

extern struct ResourceMetadataRecord Resource_Data012[];

struct ResourceMetadataRecord *Resource_GetMetadataRecordFar(u32 id)
{
    struct ResourceMetadataRecord *table;

    id &= 0xfff;
    if (id >= 716)
        id = 0;
    table = Resource_Data012;
    return &table[id];
}
