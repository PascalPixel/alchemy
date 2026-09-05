#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Item_GetData(s32);
s32 Func_08077218(s32, s32);
void *Ability_GetData(s32);

s32 Item_ClassifyUseAbility(s32 arg0, s32 arg1) {
    s32 ret;
    void *p;

    if (arg1 == 0) {
        return 1;
    }
    p = Item_GetData(arg1);
    ret = 1;
    if (FIELD_AT_OFFSET(p, u8, 0xC) == 3) {
        return ret;
    }
    if (FIELD_AT_OFFSET(p, u16, 0x28) == 0) {
        return ret;
    }
    if ((FIELD_AT_OFFSET(p, u8, 2) != 0) && (Func_08077218(arg0, arg1) == 0)) {
        return ret;
    }
    if ((0x80 & FIELD_AT_OFFSET(Ability_GetData((s32) FIELD_AT_OFFSET(p, u16, 0x28)), u8, 1)) == 0) {
        return 2;
    }
    return 0;
}
