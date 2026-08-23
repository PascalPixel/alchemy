#include "shop.h"
#include "sound_ids.h"

s32 Func_08077028(s32, s32);
s32 Func_08077228(s32, u8);
s32 Func_08077230(s32);
s32 Func_08077298(s16);
void Func_080b0574(s32 message);
s32 Func_080b1868(s32, s32);
s32 Func_080f9010(s32);
extern char Value_00000ca1;

void Shop_CompletePurchase(s32 unit_id, s32 item_id, s32 quantity) {
    s32 replaced_slot;
    s32 remaining;
    s32 added_slot;
    struct ItemDefinition *item;

    remaining = quantity;
    item = Item_Get(item_id);
    added_slot = 0;
    replaced_slot = Func_08077228(unit_id, item->type);
    Func_080f9010(SOUND_SHOP_PURCHASE);
    if (added_slot < remaining) {
        do {
            added_slot = Func_08077028(unit_id, item_id);
            Func_08077230(0 - item->price);
            remaining -= 1;
            Func_08077298(item->price);
            Shop_DrawMoney();
        } while (remaining != 0);
    }
    Func_080b0574((s32)&Value_00000ca1);
    if (Func_080b1868(unit_id, added_slot) != 0) {
        Shop_SellReplacedItem(unit_id, replaced_slot);
    }
}
