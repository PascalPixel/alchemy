#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_08077018(s32);
s32 Func_0808a490(s32);
void *Func_08077080(s32);
s32 Func_08077218(s32, s32);

#define Item_ClassifyUseMode Func_080a46b4

s32 Item_ClassifyUseMode(s32 owner, s32 itemId)
{
    s32 masked = itemId;
    void *itemData;
    s32 result = -1;

    masked &= 0x1ff;
    itemData = Func_08077018(masked);

    if (Func_0808a490(masked) != 0) {
        return 0;
    }

    {
        void *abilityData = Func_08077080(M2C_FIELD(itemData, u16, 40) & 0x3fff);

        if (M2C_FIELD(itemData, u16, 40) != 0) {
            if (M2C_FIELD(itemData, u8, 2) != 0) {
                if (M2C_FIELD(itemData, u8, 12) != 3) {
                    if (Func_08077218(owner, masked) != 0) {
                        result = 1;
                    }
                }
            } else {
                result = 1;
            }

            if (result == 1) {
                s32 mask = 0x80;
                u8 field1 = M2C_FIELD(abilityData, u8, 1);

                if ((field1 & 0x40) != 0) {
                    u8 field8 = M2C_FIELD(abilityData, u8, 8);
                    result = (field8 == 0xff) ? 2 : 1;
                } else {
                    result = (field1 & mask) ? -1 : 0;
                }
            }
        }
    }

    return result;
}
