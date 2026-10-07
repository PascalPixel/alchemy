#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RUNTIME_MEM.H"
#include "RESOURCE.H"
#include "TYPES.H"

#define Resource_DecodeByteLzInRam ((void (*)(void*,void*))Resource_DecodeByteLzInRam)


void VramBlock_LoadResource(s32 entry_no, s32 size, s32 resource_id)
{
    void *source;
    void *buffer;

    if (entry_no > 95)
        return;
    source = Resource_GetTableEntry(resource_id);
    buffer = MemAlloc(size);
    Resource_DecodeByteLzInRam(source, buffer);
    VramBlock_LoadCached(entry_no, size, buffer);
    MemFree(buffer);
}
