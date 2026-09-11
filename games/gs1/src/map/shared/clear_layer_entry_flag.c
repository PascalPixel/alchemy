#include "types.h"
#include "scene.h"
#include "abi/map/shared/clear_layer_entry_flag.h"

extern u8 *volatile gCam;

void Map_ClearLayerEntryFlag(u32 no)
{
    u8 *base = gCam;
    u8 *entry = base + no * 12;
    u32 value = 0;
    *(u16 *)(entry + 0x22) = value;
}

void Map_Run(u32 no)
{
    u8 *base = gCam;
    u8 *entry = base + no * 12;
    u32 value = 1;
    *(u16 *)(entry + 0x22) = value;
}
