#include "types.h"
#include "scene.h"
#include "abi/ui/icon/prepare_transfer_from_table_entry.h"

struct State_0801a4c0 {
    u8 filler0[0x600];
    u16 first;
    u16 second;
    u32 value;
};

extern struct State_0801a4c0 *gIw;
extern u32 gRom[];

void Ui_PrepareTransferFromTableEntry(u32 index)
{
    struct State_0801a4c0 *state = gIw;

    state->value = gRom[index];
    state->first = 2;
    state->second = 2;
    Ui_Apply(state, 0);
}
