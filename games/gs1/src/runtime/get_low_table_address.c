#include "types.h"
#include "scene.h"
#include "abi/runtime/get_low_table_address.h"

extern const u8 gRom[];

s32 Runtime_GetLowTableAddress(void)
{
    return (s32)gRom;
}
