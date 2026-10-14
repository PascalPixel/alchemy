#ifndef ALCHEMY_RESOURCE_H
#define ALCHEMY_RESOURCE_H

#include "types.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

struct ResourceObject;
struct ResourceObjectWork;
#if GAME_TLA
struct ResourceObject *ResourceObject_Create(s32 resource);
void				   ResourceObject_Release(struct ResourceObjectWork *work);
#endif
#if GAME_TLA
struct ResourceObject *ResourceObject_CreateFar(s32 resource);
void				   ResourceObject_ReleaseFar(struct ResourceObjectWork *work);
#endif

#if GAME_TLA
void *Resource_GetTableEntry(s32 resource_id);

s32 Resource_FindFreeEntry(void);
s32 Resource_LoadIntoFreeSlot(s32 size);
s32 Resource_GetBuffer(s32 slot, s32 source);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
s32 Resource_ResetEntry(u32 slot);
s32 Resource_ActivateEntry(u32 slot);
s32 Resource_ClearSlotReferences(s32 slot);
s32 ResourceTable_AllocateBlocks(u32 slot, u32 size);
#endif

s32 Resource_DecodeType01(const void *source, void *destination);

// 形式 01 の展開を呼ぶ層: ロゴ画面などの読み込みはこれを通す
static inline s32 Resource_Decode(const void *source, void *destination)
{
	return Resource_DecodeType01(source, destination);
}

#endif
