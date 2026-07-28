#include "types.h"
#include "runtime_interfaces.h"

struct State_080798b4 {
    u8 padding[0x128];
    u8 record;
};

struct Record_080798b4 {
    u8 padding[0x34];
    u8 value;
};

struct Table_080798b4 {
    s32 first;
    u8 padding[0x14];
};

extern struct Table_080798b4 Data_08088e38[];

s32 Func_080798b4(struct State_080798b4 *state)
{
    const struct Record_080798b4 *record =
        (const struct Record_080798b4 *)Func_080773d8(state->record);
    u8 value = record->value;

    if ((u32)value > 43)
        value = 0;
    return Data_08088e38[value].first;
}
