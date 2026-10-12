#include "unit/system/resource/metadata_get_record.h"
#include "metadata_lookup.h"

extern struct AnimationMetadata Character_DescriptorTable[];

struct AnimationMetadata *Resource_GetMetadataRecord(u32 number)
{
	return &Character_DescriptorTable[number & 0xfff];
}
