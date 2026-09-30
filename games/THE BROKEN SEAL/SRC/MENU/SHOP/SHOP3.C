#include "SHOP.H"
#include "BATTLE_RUNTIME.H"
#include "PARTY_STATE.H"
#include "TBS_EDITION.H"

extern u8 MsgYouHave;
extern u8 MsgShopNoneInStock;
void UiWindow_Clear(s32 window);
s32 Item_FindSlot(s32 unit_id, s32 item_id);
void UiWork_PushValueSlotFar(s32 kosuu, s32 style);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
u8 *UiIcon_Draw(u16 no, s32 kind, s32 window, s32 x, s32 y);

extern struct ShopRuntime *gMenuWork;
extern u8 Data_03001f2c[];
extern u8 MsgHowManyToBuy;
extern void UiMessage_ShowAndWait(s32);
extern s32 Item_FindSlot(s32, s32);
extern s32 Math_DivU(s32, s32);
extern s32 Ability_GetMaximum(s32, s32);
extern s32 Shop_SelectQuantity(s32, s32, s32);

/* 所持品欄の再描画。窓を開き直し、選択中の品目に応じて見出しを差し替え、
   所持枠を左上から順に並べる。枠は5個目と10個目で折り返す。
   枠番号は0xd8からのu16列で、0が終端。 */
void Shop_DrawUnitItem(s32 window, s32 unit_id, s32 item_id)
{
    u8 *unit;
    s32 x;
    s32 y;
    s32 item_index;
    s32 slot;
    s32 off;
    s32 first_offset;
    s32 item_offset;
    s32 next_offset;
    u8 *icon;

    unit = (u8 *)Owner_GetStateFar(unit_id);
    x = 8;
    y = 8;
    if (window != 0) {
        UiWindow_Clear(window);
        slot = Item_FindSlot(unit_id, item_id);
        /* 参照は枠位置を「バイト差」として先に組み、状態先頭を基底に残す。
           足し込む順を変えると二レジスタ番地形が崩れる。 */
        if (slot != -1) {
            off = slot * 2 + 216;
            UiWork_PushValueSlotFar((*(u16 *)(unit + off) >> 11) + 1, 5);
            UiText_DrawCharacterAtOffsetFar((s32)&MsgYouHave, window, 0, 0);
        } else {
            UiText_DrawCharacterAtOffsetFar((s32)&MsgShopNoneInStock, window, 0, 0);
        }
        item_index = 0;
        first_offset = 216;
        if (*(u16 *)(unit + first_offset) != 0) {
            for (;;) {
                item_offset = item_index * 2 + 216;
                icon = UiIcon_Draw(*(u16 *)(unit + item_offset), 27,
                                     window, x, y);
                icon[15] = 252;
                x += 16;
                if (item_index == 4) {
                    x = 8;
                    y += 16;
                }
                if (item_index == 9) {
                    x = 8;
                    y += 16;
                }
                item_index++;
                if (item_index > 14)
                    break;
                next_offset = item_index * 2 + 216;
                if (*(u16 *)(unit + next_offset) == 0)
                    break;
            }
        }
    }
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define ACTION_Y 120
#else
#define ACTION_Y 128
#endif

s32 Shop_SelBuyNum(s32 unit_id, s32 item_id)
{
    struct ShopRuntime *shop;
    struct BattleUnit *unit;
    struct ItemDefinition *item;
    s32 quantity;
    s32 chance;
    s32 slot;
    s32 maximum;
    s32 result;

    shop = gMenuWork;
    unit = Owner_GetStateFar(unit_id);
    item = Item_Get(item_id);
    result = 1;
    if (item->flags & 0x10) {
        UiMessage_ShowAndWait((s32)&MsgHowManyToBuy);
        slot = Item_FindSlot(unit_id, item_id);
        if (slot != -1) {
            quantity = (unit->inventory[slot] >> 11) + 1;
        } else {
            quantity = 0;
        }

        chance = 30;
        if (item->price != 0)
            chance = Math_DivU(gGameState.coins, item->price);

        if (shop->party_action == 2) {
            maximum = Ability_GetMaximum(item_id, 0);
            if (chance > maximum)
                maximum = Ability_GetMaximum(item_id, 0);
            else
                maximum = chance;
            chance = maximum;
        }

        chance += quantity;
        if (chance > 30)
            chance = 30;

        shop->mode = 12;
        Shop_PlaceCursor(0, ACTION_Y, 0x30);
        result = Shop_SelectQuantity(quantity, chance, item->price);
    }
    return result;
}
