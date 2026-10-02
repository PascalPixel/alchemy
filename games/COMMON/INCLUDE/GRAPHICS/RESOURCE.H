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
s32 Resource_LoadIntoFreeSlot(s32 resource);
s32 Resource_GetBuffer(s32 slot, s32 data);

#endif
