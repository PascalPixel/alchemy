/*
 * Draft: Item_GetEquipmentGroup does not yet match; the reference keeps the
 * type in r1 and rebuilds the constant 1 for the final type == 10 test,
 * where this reuses r0.
 * Links as recon/tla/raw/080aec68.s.
 */
#include "TYPES.H"
#include "ITEM.H"

struct ItemDefinition *Item_GetDirect(s32 item_id);

/* Sorts an item by its type into equipment groups: 1 for types 1, 7 and 10,
   2 for types 2 to 5 and 9, and 0 for everything else. */
s32 Item_GetEquipmentGroup(s32 item_id)
{
    s32 type = Item_GetDirect(item_id)->type;

    if (type == 1)
        return 1;
    if (type == 2)
        return 2;
    if (type == 3)
        return 2;
    if (type == 4)
        return 2;
    if (type == 5)
        return 2;
    if (type == 9)
        return 2;
    if (type == 7)
        return 1;
    return type == 10;
}
