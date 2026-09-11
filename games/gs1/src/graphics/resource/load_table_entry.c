#include "types.h"
#include "scene.h"

void Resource_CopyData(void *, s32, void *);
extern u8 gRom;
extern u8 Value_08031864;
extern u8 gRom2;
extern u8 gRom3;

s32 RenderResource_LoadTableEntry(u32 value, s32 unused, void *destination)
{
    void *source;
    switch (value) {
    case 1:
        source = &gRom;
        break;
    case 2:
        source = &Value_08031864;
        break;
    case 3:
        source = &gRom2;
        break;
    case 0:
    default:
        source = &gRom3;
        break;
    }
    Resource_CopyData(destination, 32, source);
    return 1;
}
