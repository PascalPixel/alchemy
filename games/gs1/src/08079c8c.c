#include "inventory.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080798b4();

s32 Func_08079c8c(void) {
    struct ItemDefinition *item;
    void *owner;

    owner = OwnerState_Get();
    if (M2C_FIELD(owner, u8 *, 0x129) == 0) {
        return Func_080798b4();
    }
    item = Inventory_GetEquippedDefinition(owner, 1);
    if (item != NULL) {
        return M2C_FIELD(item, s32 *, 0x14);
    }
    return 4;
}
