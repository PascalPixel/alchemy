#include "scene.h"
#include "abi/shop/sel/buy_done.h"
#include "shop.h"
#include "sound_ids.h"

s32 Audio_PlayCue(s32);
extern char Value_00000ca1;

void Shop_BuyDone(s32 unit_id, s32 item_id, s32 quantity)
{
    s32 replaced_slot;
    s32 remaining;
    s32 added_slot;
    struct ItemDefinition *item;

    remaining = quantity;
    item = Item_Get(item_id);
    added_slot = 0;
    replaced_slot = Sys_Apply(unit_id, item->type);
    Audio_PlayCue(SOUND_SHOP_PURCHASE);
    if (added_slot < remaining) {
        do {
            added_slot = Sys_Apply2(unit_id, item_id);
            Sys_Check(0 - item->price);
            remaining -= 1;
            Sys_Check2(item->price);
            Shop_DrawMoney();
        } while (remaining != 0);
    }
    Sys_Do((s32)&Value_00000ca1);
    if (Shop_ConfirmEquip(unit_id, added_slot) != 0) {
        Shop_SellOld(unit_id, replaced_slot);
    }
}
