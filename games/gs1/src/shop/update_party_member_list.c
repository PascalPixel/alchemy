#include "shop.h"

void Func_08009020(void *, s32);
s32 Func_08077220(s16, s32);

extern u8 *Data_03001f2c;

union ShopPartyMemberId {
    s32 word;
    s16 half[2];
};

void Shop_UpdatePartyMemberList(s32 window, s32 selected, s32 requirement)
{
    u8 *shop;
    u8 *party_member_base;
    s32 *icon_entry;
    s32 index;
    s32 offset;
    s16 unit_id;

    shop = Data_03001f2c;
    if (window != 0) {
        index = 0;
        if (index < *(s8 *)(shop + 0x3a7)) {
            party_member_base = shop + 2;
            offset = 0x36c;
            icon_entry = (s32 *)(shop + 0x114);
            do {
                if (index == selected)
                    Func_08009020((void *)*icon_entry, 30);
                else
                    Func_08009020((void *)*icon_entry, 1);
                icon_entry[16] = 0x10000;
                unit_id = ((union ShopPartyMemberId *)(
                    party_member_base + offset))->half[0];
                if (Func_08077220(unit_id, requirement) == 0)
                    icon_entry[16] = 0xcccc;
                index++;
                offset += 2;
                icon_entry++;
            } while (index < *(s8 *)(shop + 0x3a7));
        }
    }
}
