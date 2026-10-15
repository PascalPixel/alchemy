#include "types.h"

extern s16 gData_0810c3f4[][33];

s32 EventTable_GetRowType(s32 index)
{
    s32 offset = index << 5;
    u8 *table = (u8 *)gData_0810c3f4;

    /* FAKEMATCH: ⚓️ loads the table between the two halves of the row offset */
    asm volatile("" : "+l"(table), "+l"(offset));
    offset += index;
    offset <<= 1;
    offset += 64;
    return *(s16 *)(table + offset);
}
