#include "owner_state.h"
#include "types.h"

struct Owner_08079ae8 {
    u8 unknown_000[0x24];
    u8 data_024[0xd4];
    u8 values_f8[0x30];
    u8 value_128;
    s8 value_129;
};

s32 Func_08078bf0(s32);
s32 Func_080798e0(s32, void *);
s8 Func_080799b0(u8, const u8 *);

void Func_08079ae8(s32 owner_no)
{
    struct Owner_08079ae8 *owner;

    owner = Func_08077394(owner_no);
    owner->value_129 = Func_080799b0(owner->value_128, owner->values_f8);
    Func_08078bf0(owner_no);
    Func_080798e0(owner_no, owner->data_024);
}
