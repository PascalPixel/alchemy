#include "BATTLE_RUNTIME.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
extern struct ShopRuntime *gMenuWork;
extern u8 Data_03001f2c[];
extern u8 Data_03001c94[];
extern u8 gKeysRepeat[];

s32 Inventory_AddItemFar(s32, s32);
s32 Inventory_FindEquippedFar(s32, u8);
s32 Party_AdjustSixDigitCounterAFar(s32);
s32 Party_AdjustSixDigitCounterBFar(s16);
void UiMessage_ShowAndRestoreState(s32 message);
void Audio_PlayCue(s32);
extern char MsgHereYouGo;

s32 Shop_SelSellNum(s32 unit_id, s32 slot)
{
    s32 result;
    s16 saved_x;
    s16 saved_y;
    s32 effect;
    s32 state;
    s32 entry_offset;
    s32 selection;
    struct ItemDefinition *item;
    struct ShopRuntime *shop;
    struct BattleUnit *unit;

    shop = gMenuWork;
    unit = Owner_GetStateFar(unit_id);
    entry_offset = (slot * 2) + 0xd8;
    item = Item_Get(*(u16 *)((u8 *)unit + entry_offset));
    result = 1;
    effect = Shop_SalePrice(*(u16 *)((u8 *)unit + entry_offset));
    state = Shop_GetSelectionState(unit_id, slot);
    selection = state;
    if ((item->flags & 0x10) && state > 1) {
        UiMessage_ShowAndWait((s32)&MsgHowManyToSell);
        saved_x = shop->cursor.target_x;
        saved_y = shop->cursor.target_y;
        shop->cursor.anchor->kind = 4;
        shop->mode = 0xc;
        Shop_PlaceCursor(NULL, EFFECT_X, 0x30);
        result = Shop_SelectQuantity(0, selection, effect);
        WaitFrames(1);
        UiIcon_PrepareObjectFar(shop->cursor.anchor);
        Shop_PlaceCursor(NULL, saved_x, saved_y);
    }
    return result;
}
