#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Map_RenderAnimatedTileFrame(u8 *object, u32 position);
void WaitFrames(u32);

void Func_0800bf34(u8 **objects, s32 count) {
    u8 **first;
    u8 **cursor;
    s32 remaining;
    u32 position;
    u32 second;
    u32 third;
    u32 fourth;

    first = objects;
    position = 0;
    do {
        if (count > 0) {
            second = position + 1;
            third = position + 2;
            cursor = first;
            fourth = position + 3;
            remaining = count;
            do {
                Map_RenderAnimatedTileFrame(*cursor, position);
                Map_RenderAnimatedTileFrame(*cursor, second);
                Map_RenderAnimatedTileFrame(*cursor, third);
                remaining -= 1;
                Map_RenderAnimatedTileFrame(*cursor++, fourth);
            } while (remaining != 0);
        }
        WaitFrames(1U);
        position += 4;
    } while (position <= 0x7FU);
}
