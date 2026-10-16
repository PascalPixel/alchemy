#include "unit/system/resource/metadata_get_record.h"
#include "metadata_lookup.h"

extern struct AnimationMetadata gCharacter_DescriptorTable[];

// Looks up animation metadata using the descriptor ID in the low twelve bits.
struct AnimationMetadata *Resource_GetMetadataRecord(u32 descriptorId) {
  return &gCharacter_DescriptorTable[descriptorId & 0xfff];
}
