#include "item.h"

s32 Item_GetEquipmentGroup(s32 item_id) {
    s32 group;
    struct ItemDefinition *item;

    item = Item_GetDirect(item_id);
    group = 0;
    if (item->type == 1) {
        group = 1;
        goto end;
    }
    if (item->type == 2) {
        goto two;
    }
    if (item->type == 3) {
        goto two;
    }
    if (item->type == 4) {
        goto two;
    }
    if (item->type == 5) {
        goto two;
    }
    if (item->type == 9) {
        goto two;
    }
    goto end;
two:
    group = 2;
end:
    return group;
}
