#ifndef ALCHEMY_RESOURCE_H
#define ALCHEMY_RESOURCE_H

#include "types.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

// A battle sprite id's low twelve bits; the two-part sprites' second part
// is their id + 0x2001.
#define SPRITE_ID_MASK 0xfff
#define SPRITE_SECOND_PART 0x2001

#if GAME_TLA
struct ResourceObject;
struct ResourceObjectWork;

struct ResourceObject *ResourceObject_Create(s32 resource);
void ResourceObject_Release(struct ResourceObjectWork *work);
struct ResourceObject *ResourceObject_CreateFar(s32 resource);
void ResourceObject_ReleaseFar(struct ResourceObjectWork *work);

void *Resource_GetTableEntry(s32 resourceId);

s32 Resource_FindFreeEntry(void);
s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 slot, s32 source);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Resource_ResetEntry(u32 slot);
s32 VramCache_ReleaseVram(u32 slot);
s32 VramCache_FreeBlocks(s32 slot);
s32 VramCache_AllocateBlocks(u32 slot, u32 size);

s32 Resource_DecodeType01(const void *source, void *destination);
#endif

#endif
