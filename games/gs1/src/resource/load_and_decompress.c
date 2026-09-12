#include "resource.h"

void Func_08005340(void *, void *);

void Resource_LoadAndDecompress(s32 resource_id, void *destination,
    s32 skip_palette, s32 copy_palette)
{
    u8 *resource = GetResource(resource_id);

    if (copy_palette != 0) {
        void (*copy)(void *, void *, s32) = (void (*)(void *, void *, s32))0x03001388;

        copy((void *)0x05000000, resource, 0x80);
    }
    if (skip_palette != 0)
        resource += 0x80;
    Func_08005340(resource, destination);
}
