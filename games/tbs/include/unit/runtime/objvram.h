#ifndef ALCHEMY_UNIT_RUNTIME_OBJVRAM_H
#define ALCHEMY_UNIT_RUNTIME_OBJVRAM_H

#include "types.h"
#include "callback_scheduler.h"
#include "irq.h"

s32 ResourceTable_AllocateBlocks(u32 id, u32 size);
s32 ResourceTable_GetLongestFreeBlockRun(void);
s32 Resource_ClearSlotReferences(s32 id);
s32 Resource_ResetEntry(u32 no);
s32 Resource_ActivateEntry(u32 no);
s32 VramBlock_LoadCached(u32 no, u32 size, const void *src);
void Resource_InitializeTable(void);
s32 Resource_FindFreeEntry(void);
s32 Resource_LoadIntoFreeSlot(s32 size);
s32 Resource_GetBuffer(s32 no, s32 src);

#endif
