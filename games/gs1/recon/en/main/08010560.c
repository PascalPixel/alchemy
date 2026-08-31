#include "types.h"

struct MapCopyCommand {
    u16 source;
    s16 source_y;
    s16 height;
    s16 width;
    s16 delay;
} __attribute__((packed, aligned(2)));

void Func_08010424(u16 source_x, u16 source_y, s32 destination_x,
                   u32 destination_y, u16 height, u16 width);
void Func_080030f8(u16 delay);

void Func_08010560(const struct MapCopyCommand *command,
                   s32 destination_x, u32 destination_y)
{
    while (command->source != 0xffff) {
        s32 source_y = command->source_y;
        s32 height = command->height;
        s32 width = command->width;
        s32 delay = command->delay;

        Func_08010424(command->source, (u16)source_y,
                      destination_x, destination_y,
                      (u16)height, (u16)width);
        Func_080030f8((u16)delay);
        command++;
    }
}
