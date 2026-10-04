/*
 * Draft: Stream_ReadS16, the signed twin of Stream_ReadU16. Score 2: the
 * second cursor increment fills the high byte load's delay in the listing.
 */
#include "TYPES.H"

s16 Stream_ReadS16(u8 **cursor)
{
    u32 value;

    value = *(*cursor)++;
    value |= *(*cursor)++ << 8;
    return value;
}
