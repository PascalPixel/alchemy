#include "METADATA_LOOKUP.H"

extern struct AnimationMetadata Character_DescriptorTable[];

struct AnimationMetadata *Resource_GetMetadataRecord(u32 number)
{
    return &Character_DescriptorTable[number & 0xfff];
}
