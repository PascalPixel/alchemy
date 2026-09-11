#include "types.h"
#include "scene.h"
#include "abi/djinn/get_definition.h"

extern const u16 gRom[];

const u16 *Djinn_GetDefinition(u32 group, u32 index)
{
    s32 entry;

    entry = 0;
    if ((group <= 3U) && (index <= 0x13U)) {
        entry = (group * 0x14) + index;
    }
    return (const u16 *)((u8 *)gRom + entry * 0xC);
}
