#include "RUNTIME_MEM.H"
#include "RESOURCE.H"
#include "TYPES.H"

void Resource_DecodeByteLzInRam(void *source, void *destination);
void Sys_Free(void *block);

/* Decodes a resource into a scratch buffer and loads it into one of the 96
   VRAM block entries. */
void VramBlock_LoadResource(s32 entry_no, s32 size, s32 resource_id)
{
    void *source;
    void *buffer;

    if (entry_no > 95)
        return;
    source = Resource_GetTableEntry(resource_id);
    buffer = Runtime_BumpAllocate(size);
    Resource_DecodeByteLzInRam(source, buffer);
    VramBlock_LoadCached(entry_no, size, buffer);
    Sys_Free(buffer);
}
