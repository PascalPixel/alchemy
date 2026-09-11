#include "types.h"
#include "scene.h"

extern const u8 gRom[];

s32 Runtime_GetLowTableAddress(void)
{
    return (s32)gRom;
}
