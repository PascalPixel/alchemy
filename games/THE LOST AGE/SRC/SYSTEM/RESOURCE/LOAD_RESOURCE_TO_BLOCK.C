#include "TYPES.H"

void *Resource_GetTableEntry(s32 resource_id);
void *Runtime_BumpAllocate(s32 size);
void Resource_DecodeByteLzInRam(void *source, void *destination);
s32 VramBlock_LoadCached(s32 entry_no, s32 mode, void *data);
void Sys_Free(void *block);

/* Decodes a resource into a scratch buffer and loads it into one of the 96
   VRAM block entries. */
void VramBlock_LoadResource(s32 entry_no, s32 mode, s32 resource_id)
{
    void *source;
    void *buffer;

    if (entry_no > 95)
        return;
    source = Resource_GetTableEntry(resource_id);
    buffer = Runtime_BumpAllocate(mode);
    Resource_DecodeByteLzInRam(source, buffer);
    VramBlock_LoadCached(entry_no, mode, buffer);
    Sys_Free(buffer);
}
