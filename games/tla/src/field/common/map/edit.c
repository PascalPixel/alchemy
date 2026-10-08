#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "mapcopy.h"
#include "game_flags.h"

void Map_SetWorkFlagBits9To11(u32 v)
{
    struct MapWork *state = Ram_HeapSlots->map_work;
    u32 mask = v & 0xe00;
    u32 flags = state->flags;

    flags = (flags & 0xf1ff) | mask;
    state->flags = flags;
}

extern u8 Data_0202e000[];
extern u8 Data_0202e002[];
u32 Map_ReadEditU16(u8 **cursor);

void Map_ApplySetFlagEdits(void)
{
    u8 *cursor = Data_0202e002;
    u32 entry;
    s32 src_x, src_y, dst_x, dst_y, width, height;

    cursor = Data_0202e000 + (u16)Map_ReadEditU16(&cursor);
next:
    entry = (u16)Map_ReadEditU16(&cursor);
    if (((entry + 1) & 0xffff) == 0)
        return;
    if (GameFlag_Test(entry & 0xfff) != 0) {
        src_x = *cursor++;
        src_y = *cursor++;
        dst_x = *cursor++;
        dst_y = *cursor++;
        width = *cursor++;
        height = *cursor++;
        if (entry & 0x1000)
            Map_CopyCellAttributeRect(src_x, src_y, width, height, dst_x, dst_y);
        else
            Map_CopyMetatileIndicesRect(src_x, src_y, dst_x, dst_y, width, height);
    } else {
        cursor += 6;
    }
    goto next;
}

u32 Map_ReadEditU16(u8 **cursor)
{
    u32 low = *(*cursor)++;
    u32 high = *(*cursor)++;

    return low | (high << 8);
}

void Map_ApplyFlagEdits(s32 flag, s32 attributes);

void Map_SetFlagAndApplyEdits(s32 flag)
{
    flag &= 0xfff;
    GameFlag_SetBit(flag);
    Map_ApplyFlagEdits(flag, 0);
}

void Map_ApplyFlagEdits(s32 flag, s32 attributes);

void Map_SetFlagAndApplyAttributeEdits(s32 flag)
{
    flag &= 0xfff;
    GameFlag_SetBit(flag);
    Map_ApplyFlagEdits(flag, 1);
}

extern u8 Data_0202e000[];
extern u8 Data_0202e002[];
u32 Map_ReadEditU16(u8 **cursor);

void Map_ApplyFlagEdits(s32 flag, s32 attributes)
{
    u8 *cursor = Data_0202e002;
    u32 entry;
    s32 src_x, src_y, dst_x, dst_y, width, height;

    cursor = Data_0202e000 + (u16)Map_ReadEditU16(&cursor);
next:
    entry = (u16)Map_ReadEditU16(&cursor);
    if (((entry + 1) & 0xffff) == 0)
        return;
    if ((entry & 0xfff) == flag) {
        src_x = *cursor++;
        src_y = *cursor++;
        dst_x = *cursor++;
        dst_y = *cursor++;
        width = *cursor++;
        height = *cursor++;
        if (attributes != 0)
            Map_CopyCellAttributeRect(src_x, src_y, width, height, dst_x, dst_y);
        else
            Map_CopyMetatileIndicesRect(src_x, src_y, dst_x, dst_y, width, height);
    } else {
        cursor += 6;
    }
    goto next;
}
