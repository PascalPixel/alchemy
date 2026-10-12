#include "types.h"

extern const u16 Data_081287d4[];
extern const u16 Data_081287e2[];
extern const u16 Data_081287f0[];
extern const u16 Data_081287fe[];
extern const u16 Data_0812880c[];
extern const u16 Data_0812881a[];
extern const u16 Data_08128828[];
extern const u16 Data_08128836[];

u8 *UnitGet(s32 owner);
s32 Inventory_FindEquippedFar(s32 owner, s32 kind);
s32 Resource_FindFreeSlot(s32 item);

s32 Func_0811a674(s32 owner)
{
    u8 *unit = UnitGet(owner);
    s32 slot = Inventory_FindEquippedFar(owner, 1);
    s32 result = 0;

    if (slot >= 0) {
        s32 index = Resource_FindFreeSlot(*(u16 *)(unit + 0xd8 + slot * 2) & 0x1ff);
        const u16 *table;

        switch (*(u16 *)(unit + 0x14a)) {
        case 0:
            table = Data_081287d4;
            break;
        case 1:
            table = Data_081287e2;
            break;
        case 2:
            table = Data_081287f0;
            break;
        case 3:
            table = Data_081287fe;
            break;
        case 5:
            table = Data_0812880c;
            break;
        case 4:
            table = Data_0812881a;
            break;
        case 6:
            table = Data_08128828;
            break;
        case 7:
            table = Data_08128836;
            break;
        default:
            return result;
        }
        result = table[index];
    }
    return result;
}
