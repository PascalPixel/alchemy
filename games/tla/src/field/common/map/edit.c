#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "mapcopy.h"
#include "game_flags.h"
#include "resource.h"
#include "party_state.h"

void Map_SetWorkFlagBits9To11(u32 v)
{
    struct MapWork *state = Ram_HeapSlots->mapWork;
    u32             mask = v & 0xe00;
    u32             flags = state->flags;

    flags = (flags & 0xf1ff) | mask;
    state->flags = flags;
}

extern u8 gData_0202e000[];
extern u8 gData_0202e002[];
u32       Map_ReadEditU16(u8 **cursor);

void Map_ApplySetFlagEdits(void)
{
    u8 *cursor = gData_0202e002;
    u32 entry;
    s32 srcX, srcY, dstX, dstY, width, height;

    cursor = gData_0202e000 + (u16)Map_ReadEditU16(&cursor);
next:
    entry = (u16)Map_ReadEditU16(&cursor);
    if (((entry + 1) & 0xffff) == 0)
        return;
    if (GameFlag_Test(entry & 0xfff) != 0)
    {
        srcX = *cursor++;
        srcY = *cursor++;
        dstX = *cursor++;
        dstY = *cursor++;
        width = *cursor++;
        height = *cursor++;
        if (entry & 0x1000)
            Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
        else
            Map_CopyMetatileIndicesRect(srcX, srcY, dstX, dstY, width, height);
    }
    else
    {
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

extern u8 gData_0202e000[];
extern u8 gData_0202e002[];
u32       Map_ReadEditU16(u8 **cursor);

void Map_ApplyFlagEdits(s32 flag, s32 attributes)
{
    u8 *cursor = gData_0202e002;
    u32 entry;
    s32 srcX, srcY, dstX, dstY, width, height;

    cursor = gData_0202e000 + (u16)Map_ReadEditU16(&cursor);
next:
    entry = (u16)Map_ReadEditU16(&cursor);
    if (((entry + 1) & 0xffff) == 0)
        return;
    if ((entry & 0xfff) == flag)
    {
        srcX = *cursor++;
        srcY = *cursor++;
        dstX = *cursor++;
        dstY = *cursor++;
        width = *cursor++;
        height = *cursor++;
        if (attributes != 0)
            Map_CopyCellAttributeRect(srcX, srcY, width, height, dstX, dstY);
        else
            Map_CopyMetatileIndicesRect(srcX, srcY, dstX, dstY, width, height);
    }
    else
    {
        cursor += 6;
    }
    goto next;
}

extern const u16 gData_0802f380[][6];
extern char      ResourceId_Map000;

void Map_GetHeaderSize(s32 map, s32 *size)
{
    u8 *header;
    /* FAKEMATCH: the table address is a word local added after the row offset: the native reads the row as [offset, table] and loads the table between the offset's two halves; indexed as an array the operands are [table, offset] (differ 2), and without the local the table is loaded after the offset (differ 2). */
    u32 table = (u32)gData_0802f380;

    header = Resource_GetTableEntry(*(const u16 *)(map * 12 + table) + (u32)&ResourceId_Map000);
    size[0] = header[0];
    if (header[2] == 0)
        size[1] = 256;
    else
        size[1] = header[2];
    size[2] = header[1];
    if (header[3] == 0)
        size[3] = 256;
    else
        size[3] = header[3];
}

struct MapHeader
{
    u8  unk00[44];
    s32 cells;
    s32 shapes;
};
extern u8 gMapShapeGrid[];
void      Tilemap_ConvertBuffer(void);

void Map_LoadCellsAndShapes(void)
{
    u8               *source;
    struct MapHeader *header = Resource_GetTableEntry(
        *(const u16 *)(gPartyState.map * 12 + (u32)gData_0802f380) + (u32)&ResourceId_Map000);

    source = (u8 *)header + header->cells;
    Resource_DecodeType01(source, Ram_MapCellBuffer);
    Tilemap_ConvertBuffer();
    source = (u8 *)header + header->shapes;
    Resource_DecodeType01(source, gMapShapeGrid);
}
