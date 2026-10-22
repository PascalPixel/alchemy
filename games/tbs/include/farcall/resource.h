/// @file
/// Resource animation metadata lookup.

/// Returns the animation metadata for the descriptor in number's low twelve
/// bits, unchecked; a slot tag above is ignored.
struct AnimationMetadata *Resource_GetMetadataRecordFar(u32 number);
