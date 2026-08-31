#include "types.h"

extern s32 Func_020007de(s32 *, s32 *, s32 *);
extern s32 Func_020064a0(u8 *, s32 *);
extern s32 Data_0200e1e8[];
extern s32 Data_0200e190[];

s32 MapStagedActor_FindClearPosition(s32 *request)
{
    s32 direction_index;
    s32 origin[3];
    u8 *actor;
    s32 step_count;
    s32 tiles_y, tiles_x;
    u8 *active_flag;
    s32 footprint_offset;
    s32 extent_a, extent_b;
    u8 *footprint_table;
    s32 *candidate;
    s32 found;
    request[5] = 0;
    actor = (u8 *)Func_020007de(&direction_index, request + 1, request);
    if (actor == 0)
        return 0;
    active_flag = actor + 0x22;
    *active_flag = 2;
    step_count = 0;
    footprint_table = (u8 *)Data_0200e1e8;
    footprint_offset = request[0] << 4;
    {
        s32 table_offset = footprint_offset + 4;
        extent_a = *(s32 *)(footprint_table + table_offset);
        if (extent_a < 0)
            extent_a = -extent_a;
        table_offset = footprint_offset;
        table_offset += 12;
        extent_b = *(s32 *)(footprint_table + table_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        tiles_y = (extent_a + extent_b) >> 4;
        extent_a = *(s32 *)(footprint_table + footprint_offset);
        if (extent_a < 0)
            extent_a = -extent_a;
        table_offset = footprint_offset;
        table_offset += 8;
        extent_b = *(s32 *)(footprint_table + table_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        tiles_x = (extent_a + extent_b) >> 4;
    }
    {
        u8 *actor_bytes;
        s32 actor_y;
        s32 direction_x;
        candidate = origin;
        direction_x = Data_0200e190[direction_index] & 0xffff0000;
        actor_bytes = actor;
        candidate[0] = *(s32 *)(actor_bytes + 8) + direction_x;
        actor_y = *(s32 *)(actor_bytes + 12);
        candidate[1] = actor_y;
        candidate[2] = *(s32 *)(actor_bytes + 16) + (Data_0200e190[direction_index] << 16);
        request[3] = actor_y;
    }
    for (;;) {
        s32 row, column;
        {
            u8 *table = (u8 *)Data_0200e1e8;
            s32 table_offset = request[0] << 4;
            table_offset += 4;
            request[4] = origin[2] + (*(s32 *)(table + table_offset) << 16);
        }
        row = 0;
        while (row < tiles_y) {
            {
                u8 *table = (u8 *)Data_0200e1e8;
                request[2] = origin[0] + (*(s32 *)(table + (request[0] << 4)) << 16);
            }
            column = 0;
            while (column < tiles_x) {
                s32 *probe = request + 2;
                if (Func_020064a0(actor, probe) == 2)
                    goto found;
                probe[0] = probe[0] + 0x100000;
                column++;
            }
            request[4] = request[4] + 0x100000;
            row++;
        }
        step_count++;
        origin[0] = origin[0] + (Data_0200e190[direction_index] & 0xffff0000);
        origin[2] = origin[2] + (Data_0200e190[direction_index] << 16);
    }
found:
    *active_flag = 0;
    found = 0;
    if (step_count != 0) {
        s32 direction = Data_0200e190[direction_index];
        s32 direction_x = direction & 0xffff0000;
        s32 offset_x = direction_x * step_count;
        s32 offset_z = (direction << 16) * step_count;
        u8 *actor_bytes = actor;
        request[2] = *(s32 *)(actor_bytes + 8) + offset_x;
        request[3] = *(s32 *)(actor_bytes + 12);
        request[4] = *(s32 *)(actor_bytes + 16) + offset_z;
        found = 1;
    }
    return found;
}
