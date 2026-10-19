/// @file
/// Resource animation metadata lookup.
/// Far-call interfaces; their ordered jump stubs live in
/// games/tbs/src/system/far_call/RESOURCE.S.

/// Far call to Resource_GetMetadataRecord.
struct AnimationMetadata *Resource_GetMetadataRecordFar(u32 number);
