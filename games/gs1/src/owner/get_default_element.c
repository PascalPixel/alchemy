#include "types.h"
#include "runtime_interfaces.h"

struct OwnerElementState {
    u8 padding[0x128];
    u8 record;
};

struct OwnerElementRecord {
    u8 padding[0x34];
    u8 value;
};

#include "preset_table.h"

s32 Owner_GetDefaultElement(struct OwnerElementState *state)
{
    const struct OwnerElementRecord *record =
        (const struct OwnerElementRecord *)Owner_GetRecord(state->record);
    u8 value = record->value;

    if ((u32)value > 43)
        value = 0;
    return Data_08088e38[value].first;
}
