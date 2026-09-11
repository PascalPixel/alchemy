#include "types.h"
#include "scene.h"
#include "abi/inn/room_price.h"

extern u8 *gIw;
extern s8 gRom[];

u8 *Runtime_GetObject(s32);

s32 Inn_RoomPrice(s32 mode)
{
    u8 *global = gIw;
    u8 *base;
    s32 active = 0;
    s32 factor = gRom[mode];
    s32 index = 0;
    s32 offset;

    if (active < *(s8 *)(global + 0x3A7)) {
        base = global + 2;
        offset = 0x36C;
        do {
            if (*(s16 *)(Runtime_GetObject(
                    *(s16 *)(base + offset)) + 56) != 0)
                active++;
            index++;
            offset += 2;
        } while (index < *(s8 *)(global + 0x3A7));
    }

    return factor *active;
}
