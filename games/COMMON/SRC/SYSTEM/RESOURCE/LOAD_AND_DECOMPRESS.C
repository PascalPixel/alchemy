#include "RESOURCE.H"
#include "IWRAM_CALL.H"


void Resource_LoadAndDecompress(s32 resource_id, void *destination,
    s32 skip_palette, s32 copy_palette)
{
    u8 *resource = Resource_GetTableEntry(resource_id);

    if (copy_palette != 0) {
        s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

        copy((void *)0x05000000, resource, 0x80);
    }
    if (skip_palette != 0)
        resource += 0x80;
    Resource_DecodeType01(resource, destination);
}
