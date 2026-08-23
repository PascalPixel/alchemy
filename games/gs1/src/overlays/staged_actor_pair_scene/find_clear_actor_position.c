#include "staged_actor_pair_scene.h"

extern s32 Func_02000da6(s32 *, s32 *, s32 *);
extern s32 Func_02001d14(u8 *, s32 *);
extern s32 Data_02009718[];
extern s32 Data_020096c0[];

s32 StagedActorPairScene_FindClearPosition(s32 *search)
{
    s32 direction;
    s32 origin[3];
    u8 *actor;
    s32 advance_count;
    s32 rows, columns;
    u8 *collision_mode;
    s32 bounds_offset;
    s32 extent_a, extent_b;
    u8 *bounds;
    s32 *candidate;
    s32 moved;
    search[5] = 0;
    actor = (u8 *)Func_02000da6(&direction, search + 1, search);
    if (actor == 0)
        return 0;
    collision_mode = actor + 0x22;
    *collision_mode = 2;
    advance_count = 0;
    bounds = (u8 *)Data_02009718;
    bounds_offset = search[0] << 4;
    {
        s32 entry_offset = bounds_offset + 4;
        extent_a = *(s32 *)(bounds + entry_offset);
        if (extent_a < 0)
            extent_a = -extent_a;
        entry_offset = bounds_offset;
        entry_offset += 12;
        extent_b = *(s32 *)(bounds + entry_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        rows = (extent_a + extent_b) >> 4;
        extent_a = *(s32 *)(bounds + bounds_offset);
        if (extent_a < 0)
            extent_a = -extent_a;
        entry_offset = bounds_offset;
        entry_offset += 8;
        extent_b = *(s32 *)(bounds + entry_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        columns = (extent_a + extent_b) >> 4;
    }
    {
        u8 *actor_bytes;
        s32 height;
        s32 step_x;
        candidate = origin;
        step_x = Data_020096c0[direction] & 0xffff0000;
        actor_bytes = actor;
        candidate[0] = *(s32 *)(actor_bytes + 8) + step_x;
        height = *(s32 *)(actor_bytes + 12);
        candidate[1] = height;
        candidate[2] = *(s32 *)(actor_bytes + 16) +
                       (Data_020096c0[direction] << 16);
        search[3] = height;
    }
    for (;;) {
        s32 row, column;
        {
            u8 *bounds_row = (u8 *)Data_02009718;
            s32 row_offset = search[0] << 4;
            row_offset += 4;
            search[4] = candidate[2] +
                        (*(s32 *)(bounds_row + row_offset) << 16);
        }
        row = 0;
        while (row < rows) {
            {
                u8 *bounds_column = (u8 *)Data_02009718;
                search[2] = candidate[0] +
                            (*(s32 *)(bounds_column + (search[0] << 4)) << 16);
            }
            column = 0;
            while (column < columns) {
                s32 *position = search + 2;
                if (Func_02001d14(actor, position) == 2)
                    goto found;
                position[0] = position[0] + 0x100000;
                column++;
            }
            search[4] = search[4] + 0x100000;
            row++;
        }
        advance_count++;
        origin[0] = origin[0] +
                    (Data_020096c0[direction] & 0xffff0000);
        origin[2] = origin[2] + (Data_020096c0[direction] << 16);
    }
found:
    *collision_mode = 0;
    moved = 0;
    if (advance_count != 0) {
        s32 step = Data_020096c0[direction];
        s32 step_x = step & 0xffff0000;
        s32 offset_x = step_x * advance_count;
        s32 offset_z = (step << 16) * advance_count;
        u8 *actor_bytes = actor;
        search[2] = *(s32 *)(actor_bytes + 8) + offset_x;
        search[3] = *(s32 *)(actor_bytes + 12);
        search[4] = *(s32 *)(actor_bytes + 16) + offset_z;
        moved = 1;
    }
    return moved;
}
