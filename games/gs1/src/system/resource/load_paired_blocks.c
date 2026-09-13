#include "types.h"
#include "scene.h"
#include "layout_guard.h"

struct State_080a5534 {
    u8 padding[0x392];
    s16 values[2];
};

LAYOUT_OFFSET_GUARD(
    State_080a5534_values_offset, struct State_080a5534, values, 0x392);
LAYOUT_SIZE_GUARD(State_080a5534_size, struct State_080a5534, 0x398);

extern struct State_080a5534 *gIw;
extern u8 gRom[];
extern u8 gRom2[];
s32 find_free_slot(void);
void Resource_CopyData(s32, s32, const u8 *);

void Resource_LoadPairedBlocks(void)
{
    struct State_080a5534 *state = gIw;
    s32 value = find_free_slot();

    state->values[0] = value;
    Resource_CopyData(value, 128, gRom);
    value = find_free_slot();
    state->values[1] = value;
    Resource_CopyData(value, 128, gRom2);
}
