#include "SHOP.H"

/* Offers the shop's special item, whose price rises with each purchase:
   when the party can afford it, the player picks the member who carries
   it, and a full bag sends them back to choose again. */

#define INPUT_NEW_KEYS (*(volatile u32 *)ADDR_03001C94)
#define INPUT_REPEAT_KEYS (*(volatile u32 *)ADDR_03001B04)

#define SPECIAL_ITEM 228

struct SpecialItemState {
    u8 unknown_000[0x118];
    s32 coins;
    s8 purchases;
};

extern const s16 Shop_SpecialItemPrices[];
extern u8 Value_00000cc3[];

s32 Math_Mod(s32 value, s32 divisor);
void UiWork_PushValueSlotFar(s32 value, s32 slot);
void UiMessage_ShowAndRestoreState(s32 message);
void UiMessage_ShowAndWait(s32 message);
s32 Inventory_AddItemFar(s32 unit_id, s32 item_id);
void Inventory_RemoveFar(s32 unit_id, s32 slot);
s32 Inventory_CountFar(s32 unit_id);
void Party_AdjustSixDigitCounterBFar(s32 amount);
void Func_080772a0(s32 value);
void Audio_PlayCue(s32 cue);
void WaitFrames(s32 frames);

void Shop_BuySpecialItem(void *window, s32 item_window)
{
    struct ShopRuntime *shop = SHOP_RUNTIME;
    struct SpecialItemState *state = (struct SpecialItemState *)0x02000240;
    s32 price;
    u32 saved;
    s32 redraw;
    s32 unit_id;
    s32 selected_index;
    s32 message;
    s32 slot;

    redraw = 1;
    unit_id = 0;
    saved = shop->selected_item;
    price = Shop_SpecialItemPrices[state->purchases];
    selected_index = 0;

    if (price > state->coins)
        return;
    shop->selected_item = SPECIAL_ITEM;
    UiWork_PushValueSlotFar(SPECIAL_ITEM, 2);
    message = (s32)Value_00000cc3;
    UiMessage_ShowAndRestoreState(message);
    UiWork_PushValueSlotFar(shop->selected_item, 2);
    UiMessage_ShowAndRestoreState(message + 1);

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            selected_index = Math_Mod(
                selected_index + shop->party_member_count,
                shop->party_member_count);
            unit_id = shop->party_member_ids[selected_index];
            Shop_PlaceCursor(window, selected_index * 24 - 12, 0);
            shop->mode = 3;
            Shop_UpdatePartyMemberList((s32)window, selected_index, shop->selected_item);
            Shop_DrawPartyMemberItems(item_window, unit_id, shop->selected_item);
        }
        if ((INPUT_NEW_KEYS & 1) != 0) {
            slot = Inventory_AddItemFar(unit_id, shop->selected_item);
            if (slot < 0) {
                Audio_PlayCue(0x71);
                UiWork_PushValueSlotFar(unit_id, 1);
                UiWork_PushValueSlotFar(shop->selected_item, 2);
                if (Inventory_CountFar(unit_id) == 15)
                    UiMessage_ShowAndWait(0xc9e);
                else
                    UiMessage_ShowAndWait(0xca6);
                continue;
            }
            Inventory_RemoveFar(unit_id, slot);
            Audio_PlayCue(0x65);
            UiMessage_ShowAndRestoreState(0xca1);
            Inventory_AddItemFar(unit_id, shop->selected_item);
            Party_AdjustSixDigitCounterBFar(-price);
            Func_080772a0(1);
            goto done;
        }
        if ((INPUT_NEW_KEYS & 2) != 0) {
            UiMessage_ShowAndRestoreState(0xcc5);
            Audio_PlayCue(0x71);
            goto done;
        }
        if ((INPUT_REPEAT_KEYS & 0x20) != 0) {
            Audio_PlayCue(0x6f);
            selected_index--;
            redraw = 1;
        }
        if ((INPUT_REPEAT_KEYS & 0x10) != 0) {
            Audio_PlayCue(0x6f);
            selected_index++;
            redraw = 1;
        }
        WaitFrames(1);
    }
done:
    shop->selected_item = saved;
}
