#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "resource.h"
#include "types.h"

#define Resource_DecodeByteLzInRam ((void (*)(void*,void*))Resource_DecodeByteLzInRam)


void VramBlock_LoadResource(s32 entry_id, s32 size, s32 resource_id)
{
    void *source;
    void *buffer;

    if (entry_id > 95)
        return;
    source = Resource_GetTableEntry(resource_id);
    buffer = MemAlloc(size);
    Resource_DecodeByteLzInRam(source, buffer);
    VramBlock_LoadCached(entry_id, size, buffer);
    MemFree(buffer);
}
