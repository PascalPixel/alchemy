#include "types.h"
#include "scene.h"
#include "abi/ui/icon/prepare_transfer_for_item.h"

struct State_0801a4c0 {
    u8 filler0[0x600];
    u16 first;
    u16 second;
    u32 value;
};

extern struct State_0801a4c0 *gIw;
extern u32 gRom[];
void *Item_GetData(s32);

void Ui_PrepareTransferForItem(s32 arg0)
{
    struct State_0801a4c0 *state = gIw;
    void *result = Item_GetData(0x1FF & arg0);

    if (arg0 != 0) {
        state->value = gRom[*(u16 *)((u8 *)result + 6)];
    } else {
        state->value = gRom[0];
    }
    state->first = 2;
    state->second = 2;
    Ui_Apply(state, 0);
}
