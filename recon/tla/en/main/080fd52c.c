#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "mem.h"

struct SaveSlotRecord {
    u8 unknown_00[0x1c];
    u8 used;
    u8 unknown_1d[0x23];
};

struct SaveWork {
    u8 unknown_0000[0x303c];
    struct SaveSlotRecord slots[3];
};

s32 Party_SetPackedEvent(s32 kind, s32 id, s32 second)
{
    u32 packed = (kind << 10) | (id & 0x3fff);

    if (second == 0)
        gPartyState.packed_events[0] = packed;
    else
        gPartyState.packed_events[1] = packed;
    return 1;
}
