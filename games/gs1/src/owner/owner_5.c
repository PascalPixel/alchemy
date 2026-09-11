#include "types.h"
#include "scene.h"
#include "owner_state.h"

/* owner/get_record_stride84.c */
extern const u8 gRom[];

s32 Owner_GetRecordStride84(s32 arg0)
{
    return (s32)(gRom + arg0 * 0x54);
}

/* owner/refresh_derived_data.c */
struct OwnerDerivedState {
    u8 unknown_000[0x24];
    u8 data_024[0xd4];
    u8 values_f8[0x30];
    u8 value_128;
    s8 value_129;
};

u32 Owner_BuildDigitTiles(s32, void *);

void Owner_RefreshDerivedData(s32 owner_no)
{
    struct OwnerDerivedState *owner;

    owner = Owner_GetState(owner_no);
    owner->value_129 = Sys_Apply(owner->value_128, owner->values_f8);
    Sys_Check(owner_no);
    Owner_BuildDigitTiles(owner_no, owner->data_024);
}
