#include "types.h"
#include "scene.h"
#include "abi/menu/entry/update_first_object_row_positions.h"

extern u8 *gIw;

void Menu_UpdateFirstObjectRowPositions(void)
{
    u8 *base = gIw;
    s16 *offsets = (s16 *)(base + 0x134);
    s32 *entries = (s32 *)(base + 0x114);
    s32 source[2];
    s32 request[4];
    s32 index = 0;
    s32 handle;

    while (1) {
        handle = entries[index];
        if (handle != 0) {
            source[0] = entries[index + 0x10];
            source[1] = entries[index + 0x10];
            request[0] = offsets[index] << 16;
            request[1] = 0x01F40000;
            request[2] = (offsets[index + 8] << 16) + 0x01F40000;
            request[3] = 0;
            Menu_SetMode(handle, request, source, 0x4000);
        }
        index += 1;
        if (index > 3) {
            break;
        }
    }
}
