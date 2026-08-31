#include "types.h"

void Func_08010424(s32 source_x, s32 source_y, s32 destination_x,
                   u32 destination_y, s32 height, s32 width);
void Func_080030f8(s32 delay);

void Func_08010560(u16 *command, s32 destination_x, u32 destination_y)
{
    u16 source = *command;

    if (source != 0xffff) {
        s16 *args = (s16 *)(command + 1);

        do {
            s32 source_y = args[0];
            s32 height = args[1];
            s32 width = args[2];
            s32 delay = args[3];

            Func_08010424(source, (u16)source_y,
                          destination_x, destination_y,
                          (u16)height, (u16)width);
            command += 5;
            Func_080030f8((u16)delay);
            args += 5;
            source = *command;
        } while (source != 0xffff);
    }
}
