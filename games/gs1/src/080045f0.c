#include "types.h"
#include "global_cells.h"

extern const u8 Data_0800795c[];

void Func_080045f0(u32 value)
{
    u8 *buffer = (u8 *)ADDR_03001F70;
    const u8 *digits = Data_0800795c;
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
