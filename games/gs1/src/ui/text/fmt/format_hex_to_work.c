#include "types.h"
#include "scene.h"
#include "abi/ui/text/fmt/format_hex_to_work.h"
#include "global_cells.h"

extern const u8 gRom[];

void Text_FormatHexToWork(u32 value)
{
    u8 *buffer = (u8 *)ADDR_03001F70;
    const u8 *digits = gRom;
    s32 index = 7;

    do {
        buffer[index] = digits[value & 0xF];
        value >>= 4;
        index--;
    } while (index >= 0);

    {
        u8 *terminator = (u8 *)ADDR_03001F70;
        terminator[8] = 0;
    }
}
