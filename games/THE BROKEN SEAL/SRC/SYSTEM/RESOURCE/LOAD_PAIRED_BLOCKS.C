#include "TYPES.H"
#include "SCENE.H"
#include "LAYOUT_GUARD.H"

struct State_080a5534 {
    u8 padding[0x392];
    s16 values[2];
};

LAYOUT_OFFSET_GUARD(
    State_080a5534_values_offset, struct State_080a5534, values, 0x392);
LAYOUT_SIZE_GUARD(State_080a5534_size, struct State_080a5534, 0x398);

extern struct State_080a5534 *Data_03001f2c_a;
extern u8 Data_080aebcc[];
extern u8 Data_080aeb4c[];
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32, s32, const u8 *);

void Resource_LoadPairedBlocks(void)
{
    struct State_080a5534 *state = Data_03001f2c_a;
    s32 value = Resource_FindFreeEntry();

    state->values[0] = value;
    VramBlock_LoadCached(value, 128, Data_080aebcc);
    value = Resource_FindFreeEntry();
    state->values[1] = value;
    VramBlock_LoadCached(value, 128, Data_080aeb4c);
}
