#include "types.h"
#include "scene.h"

extern u32 gIw;
extern u32 gRom[];

void BattleFx_SetObjectAlternatingWords(u8 *object)
{
    u32 *table = gRom;
    u32 index = (gIw >> 2) & 1;
    u32 value = index[table];
    *(u32 *)(object + 0x18) = value;
    *(u32 *)(object + 0x1C) = value;
}
