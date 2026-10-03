#ifndef ALCHEMY_RESOURCE_H
#define ALCHEMY_RESOURCE_H

#include "TYPES.H"

/* The resource implementations own the game-specific record layouts. */
struct ResourceObject;
struct ResourceObjectWork;
struct ResourceObject *ResourceObject_Create(s32 resource);
void ResourceObject_Release(struct ResourceObjectWork *work);
struct ResourceObject *ResourceObject_CreateFar(s32 resource);
void ResourceObject_ReleaseFar(struct ResourceObjectWork *work);

void *Resource_GetTableEntry(s32 resource_id);

/* Index of a free resource table entry. */
s32 Resource_FindFreeEntry(void);
/* Reserve a cache entry of this many bytes; the result is its slot index. */
s32 Resource_LoadIntoFreeSlot(s32 size);
/* The source word may be NULL, -1 for a clear, or a RAM address.
   The result is the loaded object's VRAM tile number. */
s32 Resource_GetBuffer(s32 slot, s32 source);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
s32 Resource_ResetEntry(u32 slot);
s32 Resource_ActivateEntry(u32 slot);
s32 Resource_ClearSlotReferences(s32 slot);

/* The format 0/1 decoder returns the number of bytes written. */
s32 Resource_DecodeType01(const void *source, void *destination);

#endif
