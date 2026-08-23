#include "inventory.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_08079cbc(s32 arg0);
s32 Func_080022ec(s32, s32);
u32 Func_08079bc4(void);

u16 Func_08079d1c(void *arg0) {
    struct ItemDefinition *item;
    s32 value;

    if (M2C_FIELD(arg0, u8, 0x129) == 0) {
        return 1;
    }
    item = Inventory_GetEquippedDefinition(arg0, 1);
    if (item == NULL) {
        return 1;
    }
    if (M2C_FIELD(item, u16, 0xE) == 0) {
        return 1;
    }
    value = Func_080022ec(
        (Func_08079cbc(arg0) + (M2C_FIELD(item, u8, 0xB) * 5)) << 0x10,
        100);
    if (value > (s32) (Func_08079bc4() & 0xFFFF)) {
        return M2C_FIELD(item, u16, 0xE);
    }
    return 1;
}
