#include "types.h"
#include "scene.h"
#include "abi/resource/load_and_decompress.h"
#include "resource.h"

void Resource_LoadAndDecompress(void *resource_id, void *destination, s32 destination_offset, s32 copy_palette)
{
    u8 *resource = GetResource((s32)resource_id);

    if (copy_palette != 0) {
        void (*copy)(void *, void *, s32) = (void (*)(void *, void *, s32))0x03001388;
        copy((void *)0x05000000, resource, 0x80);
    }
    if (destination_offset != 0) {
        resource += 0x80;
    }
    Sys_Apply(resource, destination);
}
