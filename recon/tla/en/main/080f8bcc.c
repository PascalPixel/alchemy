#include "TYPES.H"
#include "M7_INTERFACES.H"
extern u8 Data_03001f2c[];

void PsynergyMenu_RefreshOwnerEntries(s32 origin_x, s32 origin_y, s32 columns);

void PsynergyMenu_RefreshOwnerEntries(s32 origin_x, s32 origin_y, s32 columns)
{
    struct RenderOutput **slot;
    struct RenderOutput **scan;
    s32 index;

    index = 0;
    slot = (struct RenderOutput **)(*(s32 *)((u32)&Data_03001f2c) + 0x48);
    scan = slot;
    do {
        if (*scan++ != NULL) {
            PsynergyMenu_PositionOwnerEntry(slot, index, origin_x, origin_y, columns);
        }
        index++;
        slot++;
    } while (index <= 0x1F);
}
