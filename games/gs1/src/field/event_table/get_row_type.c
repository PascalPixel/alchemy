#include "types.h"
#include "scene.h"
#include "abi/field/event_table/get_row_type.h"

struct EventTable {
    s16 header[32];
    s16 entries[1][33];
};

extern struct EventTable gRom;

s32 EventTable_GetRowType(s32 index)
{
    return gRom.entries[index][0];
}
