/* Near miss of Func_0811a674 (47 instructions, 2 differ): the native loads the equipped item
 * before it starts the id mask 0x1ff. A one-pass block after the load swaps the registers of the
 * mask and the item (8 differ). */
#include "types.h"
#include "owner_state.h"

extern const u16 Data_081287d4[];
extern const u16 Data_081287e2[];
extern const u16 Data_081287f0[];
extern const u16 Data_081287fe[];
extern const u16 Data_0812880c[];
extern const u16 Data_0812881a[];
extern const u16 Data_08128828[];
extern const u16 Data_08128836[];

s32 Inventory_FindEquippedFar(s32 owner, s32 kind);
s32 Resource_FindFreeSlot(s32 item);

s32 Func_0811a674(s32 owner)
{
    struct OwnerInventoryState *unit = UnitGet(owner);
    s32 slot = Inventory_FindEquippedFar(owner, 1);
    s32 result = 0;

    if (slot >= 0) {
        s32 index = Resource_FindFreeSlot(unit->inventory[slot] & 0x1ff);
        switch (unit->class_id) {
        case 0:
            result = Data_081287d4[index];
            break;
        case 1:
            result = Data_081287e2[index];
            break;
        case 2:
            result = Data_081287f0[index];
            break;
        case 3:
            result = Data_081287fe[index];
            break;
        case 5:
            result = Data_0812880c[index];
            break;
        case 4:
            result = Data_0812881a[index];
            break;
        case 6:
            result = Data_08128828[index];
            break;
        case 7:
            result = Data_08128836[index];
            break;
        }
    }
    return result;
}
