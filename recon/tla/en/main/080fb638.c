#include "TYPES.H"

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct ItemDefinition *Item_Get(s32);
s32 BattleFx_HasTriggerFar(s32);
void *BattleAction_Get(s32);
s32 Item_CanOwnerEquip(s32, s32);

s32 Item_ClassifyUseMode(s32 owner, s32 itemId)
{
    s32 masked = itemId;
    void *itemData;
    s32 result = -1;

    masked &= 0x1ff;
    itemData = Item_Get(masked);

    if (BattleFx_HasTriggerFar(masked)!= 0) {
        return 0;
    }

    {
        void *abilityData = BattleAction_Get(FIELD_AT_OFFSET(itemData, u16, 40) & 0x3fff);

        if (FIELD_AT_OFFSET(itemData, u16, 40) != 0) {
            if (FIELD_AT_OFFSET(itemData, u8, 2) != 0) {
                if (FIELD_AT_OFFSET(itemData, u8, 12) != 3) {
                    if (Item_CanOwnerEquip(owner, masked) != 0) {
                        result = 1;
                    }
                }
            } else {
                result = 1;
            }

            if (result == 1) {
                s32 mask = 0x80;
                u8 field1 = FIELD_AT_OFFSET(abilityData, u8, 1);

                if ((field1 & 0x40) != 0) {
                    u8 field8 = FIELD_AT_OFFSET(abilityData, u8, 8);
                    result = (field8 == 0xff) ? 2 : 1;
                } else {
                    result = (field1 & mask) ? -1 : 0;
                }
            }
        }
    }

    return result;
}
