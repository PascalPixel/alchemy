#include "TYPES.H"

extern const u8 System_BasicColorPalette[];

s32 Runtime_GetLowTableAddress(void)
{
    return (s32)System_BasicColorPalette;
}
