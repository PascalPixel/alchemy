#include "types.h"
#include "resource.h"
#include "ram_buffer.h"
#include "party_state.h"

struct MapTableEntry {
    u16 resource;
    u8 unknown_02[10];
};

struct MapHeader {
    u8 unknown_00[44];
    s32 cells;
    s32 shapes;
};

extern const struct MapTableEntry Data_0802f380[];
extern char ResourceId_Map000;
extern u8 gMapShapeGrid[];
void Func_0802a5e4(void);

void Map_LoadCellsAndShapes(void)
{
    struct MapHeader *header = Resource_GetTableEntry(
        Data_0802f380[gPartyState.map].resource + (u32)&ResourceId_Map000);

    Resource_DecodeType01((u8 *)header + header->cells, Ram_MapCellBuffer);
    Func_0802a5e4();
    Resource_DecodeType01((u8 *)header + header->shapes, gMapShapeGrid);
}
