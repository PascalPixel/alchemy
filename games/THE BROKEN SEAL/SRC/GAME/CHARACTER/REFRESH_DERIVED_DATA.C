#include "OWNER_STATE.H"

struct OwnerDerivedState {
    u8 unknown_000[0x24];
    u8 data_024[0xd4];
    u8 values_f8[0x30];
    u8 value_128;
    s8 value_129;
};

void *Owner_GetState(s32);
s32 Owner_RefreshClassActions(s32);
u32 Owner_BuildDigitTiles(s32, void *);
s8 Owner_DetermineClass(u8, const u8 *);

void Owner_RefreshDerivedData(s32 owner_no)
{
    struct OwnerDerivedState *owner = Owner_GetState(owner_no);

    owner->value_129 = Owner_DetermineClass(owner->value_128, owner->values_f8);
    Owner_RefreshClassActions(owner_no);
    Owner_BuildDigitTiles(owner_no, owner->data_024);
}
